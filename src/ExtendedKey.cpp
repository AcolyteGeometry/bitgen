/**
 * privKey.cpp- Bitcoin poscript generator
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
 
 
#include "ExtendedKey.h"

#include "hmac.h"

#include <stdlib.h>




bool isHardIndication(const std::string& numStr)
{
	if(!numStr.empty() && numStr[numStr.size() - 1] == 'H')
	{
		//std::cout << "Hardened!!!!!!!!!!!!!!!" << std::endl;
		return true;
	}
	
	return false;
}




ChildNum derivationStringToNum(const std::string& numStr)
{
	const bool isHardened = isHardIndication(numStr);
	if(isHardened)
	{
		//std::cout << "Hardened!!!!!!!!!!!!!!!" << std::endl;
	}
	
	const std::string onlyNum = isHardened ? numStr.substr(0, numStr.size() - 1) : numStr;
	
	//std::cout << "ONLY NUM: " << onlyNum << std::endl;
	
	const BigInt<32> addNum(isHardened ? "80000000" : "0");

	
	
	const BigInt<32> numOnly = BigInt<32>(BigInt<32>::fromDec(onlyNum));
	const BigInt<32> num = numOnly + addNum;
	
	const ChildNum ret(num);
	//std::cout << "Num: " << numOnly << std::endl;
	//std::cout << "TotNum: " << num << std::endl;
	
	return ret;
}




std::string DerivationPath::getCurrencyDerivationPath()
{
	std::string derivationStr("m/44H/");
	derivationStr += intToString(getCurrencyBip44Pos());
	derivationStr += "H/0H";	

	return derivationStr;
}


DerivationPath DerivationPath::getDerivationPath(const std::string& path)
{
	if(path.empty())
	{
		throw std::string("Incorrect derivation path");
	}
	
	std::list<ChildNum> ret;
	const unsigned char firstChar = path[0];
	if(firstChar != 'm' && firstChar != 'M')
	{
		std::string errMsg;
		errMsg += "Incorrect derivation path without m or M: ";
		errMsg += path;
		throw errMsg; //std::string("Incorrect derivation path without m or M");
	}

	const bool isPublic = (firstChar == 'M');
	//std::cout << "THIS IS: " << (isPublic ? "PUBLIC" : "PRIVATE") << std::endl;

	int pos(1);
	for(;;)
	{		
		const ChildNum part = getPart(path, pos);
		if(pos == -1)
		{
			//std::cout << "Nothing more" << std::endl;
			break;
		}
		
		//std::cout << "This part: " << part.toString() << std::endl;
		
		ret.push_back(part);
		
		
	}
	
	const DerivationPath derivationPath(ret, isPublic);
	
	return derivationPath;
}



//Todo: Move to util
bool isDigit(const unsigned char c)
{
	if(c >= '0' && c <= '9')
	{
		return true;
	}
	
	return false;
}





ChildNum DerivationPath::getPart(const std::string& path, int& pos)
{
	//std::cout << "Getting part in: " << path << std::endl;
	
	if(pos >= path.size())
	{
		ChildNum childNum(0, false);
		//std::cout << "Found E" << std::endl;
		pos = -1;		
		return childNum;		
	}
	
	const unsigned char c = path[pos];
	if(c != '/')
	{
		pos = -1;
		ChildNum childNum(0, false);
		//std::cout << "Found D: " << c << std::endl;					
		return childNum;
	}
	pos++;
	
	std::string part;	
	for(int i = pos ; i < path.size() ; i++)
	{
		const unsigned char ch = path[pos];	
		pos++;
		
		//std::cout << "Got char: " << c << std::endl;
		
		if(ch == 'H')
		{
			if(part.empty())
			{
				throw std::string("Incorrect child path");
			}
			
			const int num = atoi(part.c_str());
			ChildNum childNum(num, true);
			pos = i + 1;
			//std::cout << "Found A" << std::endl;
			return childNum;
		}
		else if(isDigit(ch))
		{
			part += ch;
		}
		else if(ch == '/')
		{
			if(part.empty())
			{
				throw std::string("Incorrect child path");
			}
			
			const int num = atoi(part.c_str());
			ChildNum childNum(num, false);
			pos = i;
			//std::cout << "Found B" << std::endl;			
			return childNum;
		}
		else
		{
			throw std::string("Incorrect child path char");			
		}
		
	}

	if(part.empty())
	{
		throw std::string("Incorrect child path");
	}
			
	const int num = atoi(part.c_str());
	ChildNum childNum(num, false);
	//pos = -1;
	pos = path.size();	
	//std::cout << "Found C" << std::endl;	
	return childNum;	
}






ExtendedPrivateKey ExtendedPrivateKey::getExtended(const std::string xpriv, const bool genericFormat)
{
	const ExtendedKey extendedKey = decodeHD(xpriv, genericFormat);
	if(!extendedKey.isPrivate)
	{
		throw std::string("Internal error, not extended private");
	}

	const ExtendedPrivateKey privKey = decodePrivateHD(extendedKey);
	return privKey;
}



ExtendedPublicKey ExtendedPublicKey::getExtended(const std::string xpub, const bool genericFormat)
{
	const ExtendedKey extendedKey = decodeHD(xpub, genericFormat);
	if(extendedKey.isPrivate)
	{
		throw std::string("Internal error, not extended publiv");
	}

	const ExtendedPublicKey pubKey = decodePublicHD(extendedKey);
	return pubKey;
}



ExtendedPrivateKey ExtendedPrivateKey::derive(const DerivationPath& derivationPath) const
{
	std::list<ExtendedPrivateKey> derivedKeys;
	derivedKeys.push_back(*this);
	
	for(std::list<ChildNum>::const_iterator it = derivationPath.path.begin() ; 
		it != derivationPath.path.end() ; 
		++it)
	{
		const ChildNum& num = (*it);
		const ExtendedPrivateKey thisKey = derivedKeys.back();
		const ExtendedPrivateKey derived = CKDpriv(thisKey, num);
		derivedKeys.push_back(derived);
		//std::cout << "Derived priv key: \n" << derived.toString() << std::endl;
		//std::cout << "Derived priv key ext format: \n" << derived.toExtFormat() << std::endl;
	
	}


	const ExtendedPrivateKey resultKey = derivedKeys.back();
	//std::cout << "Result priv key: " << resultKey.toExtFormat() << std::endl;
	//std::cout << std::endl;
	//std::cout << "Result priv key info: \n" << resultKey.toString() << std::endl;

	return resultKey;
}



ExtendedPublicKey ExtendedPublicKey::derive(const DerivationPath& derivationPath) const
{
	if(!derivationPath.isPublic)
	{
		throw std::string("Derivation path must be public");
	}
	
	std::list<ExtendedPublicKey> derivedKeys;
	derivedKeys.push_back(*this);
	
	for(std::list<ChildNum>::const_iterator it = derivationPath.path.begin() ; 
		it != derivationPath.path.end() ; 
		++it)
	{
		const ChildNum& num = (*it);
		const ExtendedPublicKey thisKey = derivedKeys.back();
		const ExtendedPublicKey derived = CKDpub(thisKey, num);
		derivedKeys.push_back(derived);	
	}


	const ExtendedPublicKey resultKey = derivedKeys.back();

	return resultKey;
}







//    Generate a seed byte sequence S of a chosen length (between 128 and 512 bits; 256 bits is advised) from a (P)RNG.
//    Calculate I = HMAC-SHA512(Key = "Bitcoin seed", Data = S)
//    Split I into two 32-byte sequences, IL and IR.
//    Use parse256(IL) as master secret key, and IR as master chain code.
//In case IL is 0 or ≥n, the master key is invalid. 	

//Calculate I = HMAC-SHA512(Key = "Bitcoin seed", Data = S)	
ExtendedPrivateKey calculateMasterKey(const std::string& privKey)
{	
	//std::cout << "Calculating master key" << std::endl;
	//std::cout << "PrivKey: " << hexString(privKey) << std::endl;
	//std::cout << "PrivKey size: " << privKey.size() << std::endl;

	
	const std::string k("Bitcoin seed");
	//std::cout << "privKey: " << privKey << std::endl;
	//std::cout << "k: " << k << std::endl;
	//std::cout << std::endl;
	const std::string result = calcHmacSha512(k, privKey);
	if(result.size() != 64)
	{
		throw std::string("Master key hmac error");
	}
	
	const std::string left = result.substr(0, 32);
	const std::string right = result.substr(32);
	
	//std::cout << "HMAC: " << hexString(result) << std::endl;

	//std::cout << "left : " << hexString(left) << std::endl;
	//std::cout << "right: " << hexString(right) << std::endl;

	const BigInt<256> privateNum(left, 'c');
	const PrivKey newPrivKey(privateNum, true);
	
	const BigInt<256> chainCode(right, 'c');
	
	const BigInt<32> zero;
	const ExtendedPrivateKey pk(0, zero, zero, chainCode, newPrivKey);
	
	//std::cout << std::endl;
	//std::cout << std::endl;
	//std::cout << "Extended: " << pk.toString() << std::endl;
	
	const ExtendedPublicKey pub = pk.getPub();
	//std::cout << "Public ext : " << pub.toExtFormat() << std::endl;

	//std::cout << "Private ext: " << pk.toExtFormat() << std::endl;	
	
	return pk;
}







std::string ExtendedKeyData::toInfoString() const
//const int depth, const BigInt<32>& fingerprint,
//	const BigInt<32>& childNum, const BigInt<256>& chainCode)
{
	std::string ret;


	
	ret += "Depth      : ";
	ret += intToString(depth);
	
	const BigInt<32> hardened("80000000");
	const BigInt<32> realChildnum = (childNum >= hardened) ? (childNum - hardened) : childNum;
	
	ret += "\nChild      : ";
	ret += realChildnum.strDec();

	if(isHard())
	{
		ret += " Hard ";		
	}
	else
	{
		ret += " Soft ";
	}
	
	ret += " Child data   : ";
	ret += childNum.strHex();
	//ret += " (";
	//ret += childNum.strDec();
	//ret += ")";

	ret += "\nChainstring: ";
	ret += chainString();

	ret += "\nChaincode  : ";
	ret += hexString(chainCode.getBinaryLong());
		
	ret += "\nParentFprin: ";
	ret += fingerprint.strHex();
		
	ret += "\n";

	return ret;
}





BigInt<32> ExtendedPublicKey::getFingerPrint() const
{
	//const BitcoinEllipticCurve ec;
	const PubAddress pubaddr = pubKey.getAddress();
	
	const BigInt<512> num = pubaddr.getNumeric();
	const BigInt<512> fingerPrint = (num >> 160);
	
	const BigInt<32> ret(fingerPrint);
	return ret;
}



BigInt<32> ExtendedPrivateKey::getFingerPrint() const
{
	const BitcoinEllipticCurve ec;
	const PubAddress pubaddr = privKey.getAddress(ec);
	
	const BigInt<512> num = pubaddr.getNumeric();
	const BigInt<512> fingerPrint = (num >> 160);
	
	const BigInt<32> ret(fingerPrint);
	return ret;
}


std::string ExtendedPrivateKey::infoString() const
{
	std::string ret;
	
	ret += extendedData.toInfoString();

	const BigInt<32> fingerPrint = getFingerPrint();
	ret += "Fingerprint: ";
	ret += fingerPrint.strHex();
		
		
	ret += "\nPrivkey    : ";
	ret += privKey.toString();
	ret += "\n";

	ret += "PrivkeyHEX : ";
	ret += hexString(privKey.key.getBinaryLong());
	ret += "\n";


	BitcoinEllipticCurve ec;
	
	ret += "Pubaddress : ";
	ret += privKey.getAddress(ec).toString();
	ret += "\n";

	return ret;
}





std::string ExtendedPublicKey::infoString() const
{
	std::string ret;
		
	ret += extendedData.toInfoString();
				

	const PubAddress pubaddr = pubKey.getAddress();
	ret += "FChild     : ";			
	//const BigInt<32> fingerPrint = getFingerPrint();
	
	const BigInt<512> num = pubaddr.getNumeric();
	const BigInt<512> fingerPrint = (num >> 160);

	ret += fingerPrint.strHex();

	ret += "\nPubkey     : ";		
	ret += pubKey.toString();

	ret += "\nPubaddress : ";		
	ret += pubaddr.toString();
	
	return ret;
}






ExtendedPublicKey ExtendedPrivateKey::getPub() const
{
	const BitcoinEllipticCurve ec;
	const PubKey pubKey = privKey.getPubkey(ec);

	const ExtendedPublicKey publicKey(extendedData.depth, extendedData.fingerprint, extendedData.childNum, extendedData.chainCode, pubKey);
		
	return publicKey;
}



std::string ExtendedPrivateKey::toString(const bool currencySpecific) const
{
	std::string buf;

	const BigInt<1024> versionPriv(currencySpecific ? getExtendedPrivPrefixHex().c_str() : "488ADE4");
	
	buf += versionPriv.getBinaryShort();
	//buf += "_";
	
	const BigInt<8> depthBig(extendedData.depth);
	buf += depthBig.getBinaryLong();
	//buf += "_";

	const BigInt<32> fingerprintBig(extendedData.fingerprint);
	buf += fingerprintBig.getBinaryLong();
	//buf += "_";

	buf += extendedData.childNum.getBinaryLong();
	//buf += "_";
	
	buf += extendedData.chainCode.getBinaryLong();	
	//buf += "_";

	buf += '\0';
	
	buf += privKey.key.getBinaryLong();
	//buf += "_";
	
	const std::string theSha = doubleSha256(buf);
	//std::cout << "sha1 " << printBin(theSha) << std::endl;	
	const std::string checkSum = theSha.substr(0, 4);
	//std::cout << "The Generated checksum: " << printBin(checkSum) << std::endl;
	const std::string cStrWithChecksum = buf + checkSum;
	
	
	//std::cout << "extended with checksum: " << hexString(cStrWithChecksum) << std::endl;


	BigInt<656> myval(cStrWithChecksum, 'c');	
	//std::cout << "Myval: " << myval << std::endl;
	
	const std::string b58Str = Base58Encode(myval);
	
		
	//std::cout << "BUF: " << b58Str << std::endl;
	
	return b58Str;
}





std::string ExtendedPublicKey::toString(const bool currencySpecific) const
{
	std::string buf;
	
	const BigInt<1024> versionPub(currencySpecific ? getExtendedPubPrefixHex().c_str() : "488B21E");	
	buf += versionPub.getBinaryShort();
	//buf += "_";
	
	const BigInt<8> depthBig(extendedData.depth);
	buf += depthBig.getBinaryLong();
	//buf += "_";

	const BigInt<32> fingerprintBig(extendedData.fingerprint);
	buf += fingerprintBig.getBinaryLong();
	//buf += "_";

	buf += extendedData.childNum.getBinaryLong();
	//buf += "_";
	
	buf += extendedData.chainCode.getBinaryLong();	
	//buf += "_";

	//buf += '\0';
	
	buf += pubKey.getRaw(); //   .key.getBinaryLong();
	//buf += "_";
	
	const std::string theSha = doubleSha256(buf);
	//std::cout << "sha1 " << printBin(theSha) << std::endl;	
	const std::string checkSum = theSha.substr(0, 4);
	//std::cout << "The Generated checksum: " << printBin(checkSum) << std::endl;
	const std::string cStrWithChecksum = buf + checkSum;
	
	
	//std::cout << "!!!!!!!!!!!!!!!!!extended with checksum: " << hexString(cStrWithChecksum) << std::endl;


	BigInt<656> myval(cStrWithChecksum, 'c');	
	//std::cout << "Myval: " << myval << std::endl;
	
	const std::string b58Str = Base58Encode(myval);
	
		
	//std::cout << "BUF: " << b58Str << std::endl;
	
	return b58Str;
}





ExtendedKey decodeHD(const std::string& extended, const bool genericFormat)
{	
	const BigInt<1024> decoded = Base58Decode<1024>(extended);
	//std::cout << "Decoded xpriv: " << decoded << std::endl;
	
	const BigInt<1024> lastFour = decoded.returnMaskHighBits(32);
	const std::string givenChecksum = lastFour.getLastBytes(4);	
	//std::cout << "Last four: " << lastFour << std::endl;
	
	const BigInt<1024> fourBytesDropped = decoded >> 32;
	//std::cout << "Four bytes dropped: " << fourBytesDropped << std::endl;

	//Calculate the checksum for this
	const std::string bufWithoutChecksum = fourBytesDropped.getLastBytes(78);
	const std::string theSha = doubleSha256(bufWithoutChecksum);
	//std::cout << "sha1 " << printBin(theSha) << std::endl;	
	const std::string computedCheckSum = theSha.substr(0, 4);
	
	if(givenChecksum != computedCheckSum)
	{
		std::cout << std::endl;
		std::cout << "Incorrect checksum" << std::endl;
		std::cout << "Given checksum   : " << hexString(givenChecksum) << std::endl;
		std::cout << "Computed checksum: " << hexString(computedCheckSum) << std::endl;
		
		throw std::string("Checksum error for extended key");
	}
	
	const BigInt<1024> versionPriv(genericFormat ? "488ADE4" : getExtendedPrivPrefixHex().c_str()); //"488ADE4");
	const BigInt<1024> versionPub(genericFormat ? "488B21E" : getExtendedPubPrefixHex().c_str()); //"488B21E");
	
	const BigInt<1024> versionBytes = (fourBytesDropped >> 592).returnMaskHighBits(32);		
	
	//std::cout << "VersionBytes: " << versionBytes << std::endl;
	
	const bool isPrivate = (versionBytes == versionPriv);
	const bool isPublic = (versionBytes == versionPub);
	
	if(isPrivate)
	{
		//std::cout << "This is a private key" << std::endl;
	}
	else if(isPublic)
	{
		//std::cout << "This is a public key" << std::endl;
	}
	else
	{
		std::cout << "Unknown xpub/prv format for: " << extended << std::endl;
		std::cout << "VersionBytes: " << versionBytes << std::endl;
		std::cout << "VersionPriv: " << versionPriv << std::endl;
		std::cout << "VersionPub: " << versionPub << std::endl;
		std::cout << "Using " << (genericFormat ? "generic" : "specific") << " format" << std::endl;
		
		
		throw std::string("Unknown xpub/xpriv key format");
	}
	
	
	
	const BigInt<1024> depth1024 = (fourBytesDropped >> 584).returnMaskHighBits(8);
	const BigInt<8> depth(depth1024);
	//std::cout << "depth: " << depth << std::endl;

	const BigInt<1024> fingerprint1024 = (fourBytesDropped >> 552).returnMaskHighBits(32);
	const BigInt<32> fingerprint(fingerprint1024);
	
	//std::cout << "fingerprint: " << fingerprint << std::endl;

	const BigInt<1024> childNum1024 = (fourBytesDropped >> 520).returnMaskHighBits(32);
	const BigInt<32> childNum(childNum1024);	
	//std::cout << "childNum: " << childNum << std::endl;

	const BigInt<1024> chainCode1024 = (fourBytesDropped >> 264).returnMaskHighBits(256);
	const BigInt<256> chainCode(chainCode1024);	
	//std::cout << "chainCode: " << chainCode << std::endl;

	const BigInt<1024> keyData1024 = (fourBytesDropped >> 0).returnMaskHighBits(264);
	const BigInt<264> keyData(keyData1024);	
	
	//std::cout << "keyData: " << keyData << std::endl;
	
	if(isPublic)
	{
		const PubKey pubKey(PubKey::fromBin(keyData.getBinaryShortest()));
		//std::cout << "Pubkey: " << pubKey.toString() << std::endl;
	}
	else if(isPrivate)
	{
		const BigInt<256> priv(keyData);
		const PrivKey privKey(priv, true);
		//std::cout << "Priv: " << privKey << std::endl;
	}
	

	const ExtendedKey extendedKey(depth, fingerprint, childNum, chainCode, keyData, isPrivate);
	return extendedKey;
}









ExtendedPrivateKey decodePrivateHD(const ExtendedKey& extKey)
{
	const BigInt<256> priv(extKey.keyData);
	const PrivKey privKey(priv, true);	
	
	const ExtendedPrivateKey extPrivKey(extKey.depth.intVal(), extKey.fingerprint, extKey.childNum, extKey.chainCode, privKey);
	return extPrivKey;
}



ExtendedPublicKey decodePublicHD(const ExtendedKey& extKey)
{
	//std::cout << "Will get the pub" << std::endl;
	const PubKey pubKey(PubKey::fromBin(extKey.keyData.getBinaryShortest()));
	//std::cout << "Has gotten the pub" << std::endl;	
	
	const ExtendedPublicKey extPubKey(extKey.depth.intVal(), extKey.fingerprint, extKey.childNum, extKey.chainCode, pubKey);
	return extPubKey;
	
}

/*
ExtendedPrivateKey CKDpriv(const ExtendedPrivateKey& extPriv)
{
	//If not (normal child): let 
	//I = HMAC-SHA512(Key = cpar, Data = serP(point(kpar)) || ser32(i)).
}
*/


/*
bool isHardened(const BigInt<32>& iNum)
{
	const BigInt<32> limit("80000000");
	
	if(iNum >= limit)
	{
		return true;
	}
	
	return false;
}
*/	


//The function CKDpriv((kpar, cpar), i) → (ki, ci) computes a child extended private key from the parent extended private key:
//    Check whether i ≥ 231 (whether the child is a hardened key).
//        If so (hardened child): let I = HMAC-SHA512(Key = cpar, Data = 0x00 || ser256(kpar) || ser32(i)). (Note: The 0x00 pads the private key to make it 33 bytes long.)
//        If not (normal child): let I = HMAC-SHA512(Key = cpar, Data = serP(point(kpar)) || ser32(i)).
//    Split I into two 32-byte sequences, IL and IR.
//    The returned child key ki is parse256(IL) + kpar (mod n).
//    The returned chain code ci is IR.
//    In case parse256(IL) ≥ n or ki = 0, the resulting key is invalid, and one should proceed with the next value for i. (Note: this has probability lower than 1 in 2127.)
ExtendedPrivateKey CKDpriv(const ExtendedPrivateKey& ek, const ChildNum& iNum)
{
	const BitcoinEllipticCurve e;
	
	const std::string serPoint = ek.privKey.point(e).getRaw(true);
	//std::cout << "Serialized point: " << hexString(serPoint) << std::endl;
	//std::cout << "Serialized point size: " << serPoint.size() << std::endl;
	
	//std::cout << "================================Serialized num: " << hexString(iNum.getBinaryLong()) << std::endl;
	std::string chainCodeSerial = ek.extendedData.chainCode.getBinaryLong();
	//std::cout << "ChainCodeSerial: " << hexString(chainCodeSerial) << std::endl;
	//std::cout << "ChainCodeSerial size: " << chainCodeSerial.size() << std::endl;
	
	std::string myHmac;
	if(iNum.isHardened())
	{
		//std::cout << "IT IS HARD" << std::endl;
		//If so (hardened child): let I = HMAC-SHA512(Key = cpar, Data = 0x00 || ser256(kpar) || ser32(i)). (Note: The 0x00 pads 	
		//HMAC-SHA512(Key = cpar, Data = 0x00 || ser256(kpar) || ser32(i)). (Note: The 0x00 pads the private key to make it 33 bytes long.)
		
		//Data = 0x00 || ser256(kpar) || ser32(i)
		std::string zeroStr; //('\0');
		zeroStr += '\0';
		
		const std::string serPriv = ek.privKey.key.getBinaryLong();
		const std::string data = zeroStr + serPriv + iNum.val.getBinaryLong();
		//std::cout << "Will calc HMAC for: " << hexString(data) << std::endl;
		//std::cout << "Size of data: " << data.size() << std::endl;
		myHmac = calcHmacSha512(chainCodeSerial, data);
	}
	else
	{
		//std::cout << "IT IS SOFT" << std::endl;
		//HMAC-SHA512(Key = cpar, Data = serP(point(kpar)) || ser32(i)).	
		//const std::string 
		const std::string data = serPoint + iNum.val.getBinaryLong();
		
		myHmac = calcHmacSha512(chainCodeSerial, data);
	}

	const std::string hmacLeft = myHmac.substr(0, 32);		
	const std::string hmacRight = myHmac.substr(32);
		
	const BigInt<1024> hmacLeftNum(hmacLeft, 'c');
	const BigInt<1024> kpar(BigInt<1024>::asPositive(ek.privKey.key));

	//std::cout << "Adding    : " << hmacLeftNum << std::endl;
	//std::cout << "   and    : " << hexString(kpar.getBinaryLong()) << std::endl;
	//std::cout << "modulus   : " << hexString(e.n.getLastBytes(32)) << std::endl;

//The returned child key ki is parse256(IL) + kpar (mod n).
	const BigInt<1024> childKeyNumBig((hmacLeftNum + kpar) % e.n);
	const BigInt<256> childKeyNum(childKeyNumBig);
	const PrivKey childKey(childKeyNum, true);	
	
	//std::cout << "Old childnum: " << ek.extendedData.childNum << std::endl;
	//std::cout << "Increase: " << iNum << std::endl;
	
	const ChildNum childNum(/*ek.extendedData.childNum +*/ iNum);
	//std::cout << "New childnum: " << childNum << std::endl;
	
	
	const BigInt<256> chainCode(hmacRight, 'c');

		
	const int newDepth = ek.extendedData.depth + 1; // + i
	//std::cout << "Setting the depth to: " << newDepth << std::endl;
	
	const ExtendedPrivateKey derived(newDepth, ek.getFingerPrint(), childNum, 	chainCode, childKey);
		
	return derived;
}









ExtendedPublicKey CKDpub(const ExtendedPublicKey& pubKey, const ChildNum& iNum)
{
	if(iNum.isHardened())
	{
		throw std::string("Hardened child not permitted");
	}
	
	const BitcoinEllipticCurve ec;
	
	const std::string serPoint = pubKey.pubKey.point.getRaw(true);	
	const std::string data = serPoint + iNum.val.getBinaryLong();

	const std::string chainCodeSerial = pubKey.extendedData.chainCode.getBinaryLong();

	const std::string myHmac = calcHmacSha512(chainCodeSerial, data);

	const std::string hmacLeft = myHmac.substr(0, 32);		
	const std::string hmacRight = myHmac.substr(32);

	const BigInt<256> chainCode(hmacRight, 'c');

	const BigInt<256> hmacLeftNum(hmacLeft, 'c');	
	const PrivKey privKey(hmacLeftNum, true);	
	
	const ECPoint point = privKey.point(ec);
	
	const ECPoint newPoint = ec.add(point, pubKey.pubKey.point);
	const PubKey newPubKey(newPoint, true);

	const ChildNum childNum(iNum);	//Todo: Not needed
			
	const int newDepth = pubKey.extendedData.depth + 1;
	
	const ExtendedPublicKey derived(newDepth, pubKey.getFingerPrint(), childNum, chainCode, newPubKey);
		
	return derived;
}



//Todo: move to DerivationPath cpp file
std::string DerivationPath::expandShorthands(const std::string& givenpath)
{
	std::string path(givenpath);
	
	if(givenpath == "coinomiRec" || givenpath == "copayRec" || givenpath == "jaxxRec" ||givenpath == "exodus")
	{
		path = "m/44H/";
		path += intToString(getCurrencyBip44Pos());
		path += "H/0H/0/0";
	}
	else if(givenpath == "coinomiCha" || givenpath == "copayCha" || givenpath == "jaxxCha")
	{
		path = "m/44H/";
		path += intToString(getCurrencyBip44Pos());
		path += "H/0H/1/0";
	}	
	else if(givenpath == "electrumRec")
	{
		path = "m/0/0";
	}
	else if(givenpath == "electrumCha")
	{
		path = "m/1/0";
	}
	else if(givenpath == "coinomiBase")
	{
		path = "m/44H/";
		path += intToString(getCurrencyBip44Pos());
		path += "H/0H";
	}
	

	//std::cout << "Using " << givenpath << " path: " << path << std::endl;
	
	return path;
}




