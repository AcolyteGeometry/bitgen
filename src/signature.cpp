/**
 * signature.h - Bitcoin address generator
 *
 * Copyright (C)2015 The bitgen developers <bitgen@openmailbox.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
 
#include "signature.h"
#include "bigint.h"
#include "pubAddress.h"
#include "addressConversion.h"
#include "sigFile.h"
#include "bitsigConfig.h"

#include "util.h"


#include <list>




IndexAndParity getIndexAndParity(const RSVal& rs, const ECPoint& pubkey, const BigInt<256>& hashNum)
{
	BigInt<1024> N(BitcoinEllipticCurve::Nval());
	BigInt<1024> P(BitcoinEllipticCurve::Pval());
	const BigInt<1024> rr(rs.r);

	BigInt<1024> xFromR = rr + N;
	const bool indexZero  = (xFromR > P) ? true : false;
	

	//Try the two possible paritys
	const bool parityOdd = SignatureImplementation::getParity(pubkey, indexZero, rs, hashNum);

	IndexAndParity iap(indexZero, parityOdd);
	return iap;
}


bool SignatureImplementation::checkSigLowS(const BigInt<256>& hash, const ECPoint& pubkey, const RSVal& rs, const bool requireLowS/* = false*/)
{
	//Does this public key correspond to the bitcoin address?

	if(!verifySign(rs, pubkey, hash, requireLowS))
	{
		//std::cout << "Error in the signature!" << std::endl;
		throw std::string("Error in the signature!");
	}
	
	//std::cout << "Verify OK for alias: " << std::endl;
	
	return true;
}



void SignatureImplementation::verifyPubaddrSpecifiedByUser(const std::string & pubadd, const PubAddress& bitaddress, std::string& alias)
{	
	if(!pubadd.empty()) 	//Address given on command line
	{			
		const PubAddress givenPub(pubadd);
		std::cout << "Using public address : " << givenPub << std::endl;
		//givenPub.validate();

		if(givenPub != bitaddress)
		{
			std::cout << "Error, the address does not match" << std::endl;
			
			throw std::string("Address does not match");
		}
	}
	else
	{
		std::cout << "No public address given, looking in keychain" << std::endl;
				
		if(!BitsigConfig::findStoredPubAddr(bitaddress, alias))
		{
			std::cout << std::endl;
			std::cout << "Can not find any pub addr match" << std::endl;
			
			throw std::string("Public addr not found in keychain");
		}
		
		std::cout << "Found the address in the keychain" << std::endl;		
	}
}



std::string SignatureImplementation::verifySignatureForFile(const std::string& signedFile,const std::string& pubadd, std::string& alias, 
	std::string& verifiedAddress, const bool rawContent)
{
	std::string signFile;	
	const SigFileContent sigParam = parseSignedWithIncludedFile(signedFile, signFile, rawContent);
	
	//std::cout << "Content: \"" << signFile << "\"" << std::endl;
	
	//Calculate the hash of the file		
	
	const int fileSize = signFile.size();
	//std::cout << "File size: " << fileSize << std::endl;
	const std::string hashStart = SignatureImplementation::hashStringStart(fileSize);	
	const std::string totalFile = hashStart + signFile;
	
	const std::string firstHash = sha256(totalFile);		//Todo: use doubleSha256()		
	const std::string theHash = sha256(firstHash); 		
	const BigInt<256> hashNum(theHash, 'c');

	//std::cout << "Hash: " << hashNum << std::endl;

	const ECPoint pubkey = recoverThePublicKey(sigParam.firstX, sigParam.oddY, sigParam.rs, hashNum);

	const PubKey p(pubkey, sigParam.compressed);
	const PubAddress bitaddress = p.getAddress();
	//Does this pubkey give the bitcoin address?			
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, sigParam.compressed);			
	verifiedAddress = bitaddress.toString(); //.address;
	
	//std::cout << "Calculated address   : " << bitaddress << std::endl;
	SignatureImplementation::verifyPubaddrSpecifiedByUser(pubadd, bitaddress, alias);
	
					
	std::cout << "Verifying signature...." << std::endl;
		
	//Require lowS value
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, sigParam.rs, true))
	{		
		//std::cout << "Verify OK for address: " << bitaddress << std::endl;
		if(!alias.empty())
		{
			//std::cout << "Address alias: " << alias << std::endl;
		}
		
		return signFile;
		
		//Save embedded file 
		//const std::string saveFilename = std::string("out_") + removeEnding(signedFile, ".bitfsig");
		
		//std::cout << "Saving to: " << saveFilename << std::endl;
		//fileWrite(saveFilename, signFile);
	}


	throw std::string("Signture fail");
	return "error";
}





std::string SignatureImplementation::hashStringStart(const int size)
{	
	const std::string sigStart("Bitcoin Signed File ");
	const std::string lengthStr = intToString(size);
	const std::string strStart(sigStart + lengthStr + " bytes:");
	
	return strStart;
}



std::string SignatureImplementation::signString(const std::string& content, const BigInt<256>& privKey, 
					const BigInt<256>& tempKey, const bool& compressed, const bool rawContent)
{
	//Find the length of the file
	const int fileSize = content.size();
	//std::cout << "File size: " << fileSize << std::endl;
	const std::string hashStart = SignatureImplementation::hashStringStart(fileSize);
			
	
	const std::string totalFile = hashStart + content;
	
	const std::string hashNumOne = sha256(totalFile);
	const std::string theHash = sha256(hashNumOne);
	const BigInt<256> hashNum(theHash, 'c');

	//std::cout << "Hash: " << hashNum << std::endl;

	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);
		
	const BigInt<256> r(rs.r);
	const BigInt<256> s(rs.s);
		
	BitcoinEllipticCurve ec;
		
	//Verify signature to make sure it is valid
	const PrivKey p(privKey, compressed);
	const PubKey pub = p.getPubkey(ec);	
	const ECPoint pubkey = pub.point;
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	if(!verifySign(rs, pubkey, hashNum))
	{
		std::cout << "Error in signature for string sign!!!!!" << std::endl;
		throw std::string("Error in signature for string sign!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pubkey, hashNum);



	std::string sigResult = createFileSignatureContent(hashNum, r, s, iap.indexZero, iap.parityOdd, compressed, 
														content, rawContent);
														
	return sigResult;
}






bool SignatureImplementation::getParity(const ECPoint& pubkey, const bool indexZero, const RSVal& rs, const BigInt<256>& hashNum)
{
	std::cout << "Trying even..." << std::endl;
	const ECPoint test1 = SignatureImplementation::recoverThePublicKey(indexZero, false, rs, hashNum);
	if(test1 == pubkey)
	{
		std::cout << "Parity is even" << std::endl;
		return false;
	}

	std::cout << "Trying odd..." << std::endl;
	const ECPoint test2 = SignatureImplementation::recoverThePublicKey(indexZero, true, rs, hashNum);
	if(test2 == pubkey)
	{
		std::cout << "Parity is odd" << std::endl;
		return true;
	}
	
	throw std::string("Internal error, could not get parity");
}



//Todo: Make static member in some class
RSVal SignatureImplementation::sign(const BigInt<256>& myPrivKey, const BigInt<256>& myTempRand, const BigInt<256>& myhashToSign)
{
	const BigInt<1024> privKey(BigInt<1024>::asPositive(myPrivKey));		//Using 1024 bits during conversion
	const BigInt<1024> tempRand(BigInt<1024>::asPositive(myTempRand));	//Using 1024 bits during conversion
	const BigInt<1024> hashToSign(BigInt<1024>::asPositive(myhashToSign));	//Using 1024 bits during conversion
	 
	 
	BitcoinEllipticCurve ec;
					
	const ECPoint PublicKey = ec.multiplyGenPoint(privKey);	
	
	const ECPoint randSignPoint = ec.multiplyGenPoint(tempRand);	
	
	const BigInt<1024> r = randSignPoint.x % ec.n;
	const BigInt<1024> s = ((hashToSign + r * privKey) * ec.modinv(tempRand, ec.n)) % ec.n;
	
	//The computed (r,s) values is a valid signature.
	//However, we want to always use the "low-s" signature
	
	const BigInt<1024> nHalf = (ec.n >> 1);
	if(s > nHalf)
	{
		//std::cout << "CALCULATING THE LOW S VALUE" << std::endl;
		const BigInt<1024> s2 = ec.n - s;
		const RSVal rs2(r, s2);
		return rs2;
	}	
	
	RSVal rs(r, s);
	return rs;
}



bool SignatureImplementation::verifySign(const RSVal& rs, const ECPoint& pubkey, const BigInt<256>& myhashToSign, const bool requireLowS)
{	
	//Using 1024 bits during conversion
	const BigInt<1024>& r = rs.r;	
	const BigInt<1024>& s = rs.s;
	const BigInt<1024> hashToSign(BigInt<1024>::asPositive(myhashToSign));

	
	BitcoinEllipticCurve ec;

	const BigInt<1024> nHalf = (ec.n >> 1);
	if(s > nHalf)
	{
		std::cout << "Signature does not use low-s value" << std::endl;
		if(requireLowS)
		{
			return false;
		}
	}
	
	const BigInt<1024> w = ec.modinv(s, ec.n);
	const ECPoint u1 = ec.multiplyGenPoint((hashToSign * w) % ec.n);	
	const ECPoint u2 = ec.multiply(pubkey, (r * w) % ec.n);	
	const ECPoint xy = ec.add(u1, u2);
	return (r == xy.x);	
}





//recId : 0 or 1
//parity: true or false
//  (Totaly 4 different combinations)
ECPoint SignatureImplementation::recoverThePublicKey(const bool firstX, const bool oddY, const RSVal& rs,const BigInt<256>& hash)
{
	//std::cout << "Recovering public key firstX: " << (firstX ? "true" : "false") << std::endl;
	//std::cout << "Recovering public key oddY: " << (oddY ? "true" : "false") << std::endl;

	BigInt<1024> r = rs.r;
	BigInt<1024> s = rs.s;

	BigInt<1024> zero;

	if(r < zero)
	{
		throw std::string("Error, r must be positive");
	}

	if(s < zero)
	{
		throw std::string("Error, s must be positive");
	}


	BigInt<1024> N(BitcoinEllipticCurve::Nval());
	BigInt<1024> P(BitcoinEllipticCurve::Pval());
	
	//BigInt<1024> i(recId);		//Todo: only 0 and 1 used - use bool instead?
	//BigInt<1024> iN(N * i);
	
	
	BigInt<1024> x = firstX ? r : (r + N);
	
	
	if(x > P) 
	{
		//std::cout << "x too large" << std::endl;
		//ECPoint err(0,0);
		//return err;
		
		throw std::string("Error, x too large for recovery");
	}
	
	//std::cout << "Internal x: " << x << std::endl;

	BitcoinEllipticCurve ec;

	
	const ECPoint R = ec.recoverPointFromXAndParityOdd(x, oddY); //i == 0);
	
	//std::cout << ": " << R << std::endl;
	
	const BigInt<1024> e(BigInt<1024>::asPositive(hash));
	
	const BigInt<1024> eInv = (zero - e) % N;
	//std::cout << "eInv: " << eInv << std::endl;
	const BigInt<1024> rInv = ec.modinv(r, ec.n);
	const BigInt<1024> srInv = (rInv * s) % N;

	const BigInt<1024> eInvrInv = (rInv * eInv) % N;
	const ECPoint p1 = ec.multiplyGenPoint(eInvrInv);
	const ECPoint p2 = ec.multiply(R, srInv);
	const ECPoint result =  ec.add(p2, p1);
	
	//if(result.y.odd() != oddY)
	//{
		//std::cout << "Not the right value, change parity" << std::endl;
	//	continue;
	//}
		
	return result;	//OK, return value	
	//}
	
	//throw std::string("Could not find value with the right Y parity");
}





/*
std::list<ECPoint> recoverPossiblePubKeys(const RSVal& rs,const BigInt<256>& hash)
{
	std::list<ECPoint> ret;
	
	for(int i = 0 ; i < 2 ; i++)	//Todo: Why iterate over 0 and 1? Only 0 gives solution!?!?!?
	{
		for(int j = 0 ; j < 2; j++)
		{ 
		const ECPoint point = recoverThePublicKey(i == 0, (j % 2 == 0), rs, hash);
		if(point == ECPoint(0, 0))
		{
			continue;
		}
		
		//std::cout << "Success for: " << i << " " << ((j % 2 == 0) ? "true": "false") << std::endl;
		//std::cout << "Point: " << point << std::endl;
		
		ret.push_back(point);
		}
	}
	
	return ret;
}SignatureImplementation::
*/




std::list<std::string> SignatureImplementation::getPossibleAdrFromPossiblePoints(const std::list<ECPoint>& possiblePoints)
{
	std::list<std::string> ret;
	
	for(std::list<ECPoint>::const_iterator it = possiblePoints.begin() ;
		it != possiblePoints.end() ;
		++it)
	{
		ECPoint pubkey = (*it);
		
		const PubKey pComp(pubkey, true);
		
		//Try compressed
		//PubAddress pubad = PubAddress::bitcoinAddresFromPubkey(pubkey, true);
		const PubAddress pubad = pComp.getAddress();
		std::cout << "Recovered comp address: " << pubad.toString() /*.address*/ << std::endl;
		ret.push_back(pubad.toString() /*.address*/);

		const PubKey pUnComp(pubkey, false);
		
		const PubAddress pubadUncomp = pUnComp.getAddress();
		//Try un-compressed
		//PubAddress pubadUncomp = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		std::cout << "Recovered uncomp address: " << pubadUncomp.toString() /*.address*/ << std::endl;
		ret.push_back(pubadUncomp.toString() /*.address*/);
	}

	return ret;
}


/*
ECPoint getPointForGivenAddress(const std::list<ECPoint>& possiblePoints, const PubAddress& givenPub)
{	
	for(std::list<ECPoint>::const_iterator it = possiblePoints.begin() ;
		it != possiblePoints.end() ;
		++it)
	{
		ECPoint pubkey = (*it);
		
		
		//Try compressed
		PubAddress pubad = PubAddress::bitcoinAddresFromPubkey(pubkey, true);
		std::cout << "Recovered comp address: " << pubad << std::endl;
		if(pubad == givenPub)
		{
			std::cout << "This comp address was used when signing!" << std::endl;
			return pubkey;
		}
		
		//Try un-compressed
		PubAddress pubadUncomp = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		std::cout << "Recovered uncomp address: " << pubadUncomp << std::endl;
		if(pubadUncomp == givenPub)
		{
			std::cout << "This uncomp address was used when signing!" << std::endl;
			return pubkey;
		}
		
	}

	return ECPoint(0,0);
}
*/


std::string SignatureImplementation::possiblePointsStr(const std::list<ECPoint>& possiblePoints) 
{
	std::string ret;
	
	for(std::list<ECPoint>::const_iterator it = possiblePoints.begin() ;
		it != possiblePoints.end() ;
		++it)
	{
		const ECPoint& p = (*it);
		const PubKey pUnComp(p, false);
		const PubAddress bitaddress = pUnComp.getAddress();	
		//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(p, false);
		ret += bitaddress.toString(); //.address;
		ret += " ";

		const PubKey pComp(p, true);
		const PubAddress bitaddresscomp = pComp.getAddress();
		//const PubAddress bitaddresscomp = PubAddress::bitcoinAddresFromPubkey(p, true);			
		ret += bitaddresscomp.toString(); //.address;
		ret += " ";
		
	}
	
	return ret;
}



