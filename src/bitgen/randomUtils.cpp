/**
 * randomUtils.cpp - Bitcoin poscript generator
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
 
 
 
#include "randomUtils.h"
#include "simpleFile.h"
#include "nonblockFile.h"
#include "privKey.h"
#include "WinRandom.h"


std::list<BigInt<256> > getTempKeys(const int num, const bool fixednounce)
{
	std::list<BigInt<256> > tempKeys;
	
	for(int i = 0 ; i < num ; i++)
	{
		if(fixednounce)
		{
			std::cout << "WARNING, USING FIXED NOUNCE" << std::endl;
		
			const BigInt<256> tempKey("2342342342423423477788887637485683948394839383738493827485948372");
			//std::cout << "Fixed tempkey: " << tempKey << std::endl;
			tempKeys.push_back(tempKey);			
			continue;
		}
		
		//std::cout << "Random gen" << std::endl;
		std::cout << "Creating random data for key " << i << "/" << num << ". Add entropy if needed (Move the mouse etc)" << std::endl;
		
		const std::string random = getFromDevRandom(32);				
		const BigInt<256> tempKey(random, 'c');
		//std::cout << "The temp key: " << tempKey.strHex() << std::endl;
		tempKeys.push_back(tempKey);
	}
	
	return tempKeys;
}


std::string getFromDevURandom(const int numVal)
{
	SimpleFile file("/dev/urandom");
	
	std::string ret;

	while(ret.size() < numVal)
	{
		const int remain = numVal - ret.size();
		const std::string chunk = file.getStr(remain);		
		ret += chunk;		
	}

	//std::cout << std::endl;
	
	return ret;
}



unsigned char getNonblockRandomChar(bool& gotChar)
{
	NonblockFile file("/dev/random");
	
	unsigned char buf = file.readOneChar(gotChar);
	return buf;
}


unsigned char getURANDNonblockRandomChar(bool& gotChar)
{
	NonblockFile file("/dev/urandom");
	
	unsigned char buf = file.readOneChar(gotChar);
	return buf;
}



#ifdef __linux
std::string getFromDevRandom(const int numVal)
{
	SimpleFile file("/dev/random");

	std::string ret;

	for(int i = 0 ; i < numVal ; i++)
	{
		unsigned char buf = file.readOneChar();

		std::cout << (numVal - i) << " " << std::flush;
		
		if(i % 16 == 15)
		{
			std::cout << std::endl;
		}
		
		ret += buf;
	}

	std::cout << std::endl;
	
	return ret;
}
#else
std::string getFromDevRandom(const int numVal)
{	
	WinRandom wr;
	const std::string ret = wr.generate(numVal);
	
	return ret;
}
#endif



void checkPassphraseEntropy(const std::string& passphrase)
{
	const int minChar(50);	
	if(passphrase.size() < minChar)
	{
		std::cout << "--------------------------------------------------------------------" << std::endl;		
		std::cout << "ERROR: PASSPHRASE TOO SHORT!"<< std::endl;
		std::cout << "Use at least " << minChar << " characters, got: " << passphrase.size() << " chars" << std::endl;
		std::cout << "--------------------------------------------------------------------" << std::endl;
		
		throw std::string("Passphrase input too short");		
	}
}





bool verifyHashEntropy(const std::string& hashval, const int minChar)
{	
	if(hashval.size() < minChar)
	{
		return false;
	}	
		
	return true;
}



void checkHashEntropy(const std::string& hashval, const int minChar)
{	
	if(hashval.size() < minChar)
	{
		std::cout << "--------------------------------------------------------------------" << std::endl;		
		std::cout << "ERROR: HASHVAL TOO SHORT!"<< std::endl;
		std::cout << "Need at least " << minChar << " characters, got: " << hashval.size() << " chars" << std::endl;
		std::cout << "--------------------------------------------------------------------" << std::endl;
		throw std::string("Hash input too short");
	}	
	
	if(hashval.size() < 50)
	{
		std::cout << "--------------------------------------------------------------------" << std::endl;		
		std::cout << "WARNING: HASHVAL TOO SHORT!"<< std::endl;
		std::cout << "Use at least 50 random characters"<< std::endl;
		std::cout << "--------------------------------------------------------------------" << std::endl;		
	}
}


BigInt<256> getRandomNumber(const bool longbuf)
{	
	if(longbuf)
	{
		const std::string buf = getFromDevRandom(128);
		const std::string random = sha256(buf);
		const BigInt<256> privKey(random, 'c');
		return privKey;
	}
	else
	{
		const std::string random = getFromDevRandom(32);
		
		const BigInt<256> privKey(random, 'c');
		return privKey;
	}
}


BigInt<256> getBlockCalcRandomNumber()
{	
	//Get a 1MB block from /dev/urandom, and get sha256 hash of that
	const std::string block = getFromDevURandom(1000000);
	const std::string random = sha256(block);
	const BigInt<256> privKey(random, 'c');
	return privKey;
}



//Todo: Remove this function
BitcoinKeyPair generateRandomAddress(const BigInt<256>& privKey, std::list<std::string>& generationExtra, const bool compressed)
{		
	const std::string hexStr = hexString(privKey.getBinaryLong() /*getBinaryVal(true)*/);

	generationExtra.push_back("Raw hex: ");
	generationExtra.push_back(hexStr.substr(0, 32));
	generationExtra.push_back(hexStr.substr(32));
		
	//const std::string pkif = publicKeyPoint.createPublicKeyImportFormat(compressed);
	const PrivKey myPrivKey(privKey, compressed);	
	
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = myPrivKey.getPubkey(ec);
	std::cout << "PKIF: " << pubKey.toString() << std::endl;
		
		
	return BitcoinKeyPair::calcKeys(myPrivKey); //calcDisplayKeys(publicKeyPoint, privKey, compressed);		
}



void testGeneration(const BigInt<256>& privKey, const std::string& privVal, const std::string& pubVal)
{
	Wif wif(privVal);
	
	const PrivKey myPrivKey = wif.getPriv();
	
	//bool compressed(false);
	//wif.wifToPrivKey(compressed);
		
	std::cout << "Test for " << (myPrivKey.compressed ? "comp" : "noncomp") << " address: " << pubVal << std::flush;
	
	
	const Wif privatekey = Wif::privkeyToWIF(privKey, myPrivKey.compressed);
	//std::cout << "Private key (WIF): " << privatekey << std::endl;	
	if(privatekey.toString() != privVal)
	{
		throw std::string("Error, test failed!");
	}
	
	//std::cout << " Compare wif ok" << std::endl;
		
	//const ECPoint publicKeyPoint = getPublicPointFromPrivateKey(privKey);
	const PrivKey privateKey(privKey, myPrivKey.compressed);
	BitcoinEllipticCurve ec;		
	const ECPoint publicKeyPoint = privateKey.point(ec);
	const PubKey p(publicKeyPoint, myPrivKey.compressed);
	const PubAddress pubkey = p.getAddress();
		
	//const PubAddress pubkey = PubAddress::bitcoinAddresFromPubkey(publicKeyPoint, myPrivKey.compressed);
			
	//const PubAddress pubkey = PubAddress::getPublicFromPrivateBinary(compressed, privKey);
	if(pubkey.toString() /*.address*/ != pubVal)
	{
		std::cout << "        Error" << std::endl;
		std::cout << std::endl;
		throw std::string("Error, test failed!");
	}
	
	std::cout << "        OK" << std::endl;
}



