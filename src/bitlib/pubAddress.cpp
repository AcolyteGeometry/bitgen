/**
 * pubAddress.cpp - Bitcoin poscript generator
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

#include "pubAddress.h"
#include "base58.h"
#include "currencySpecific.h"


bool operator<(const PubAddress& a, const PubAddress& b)
{
	return (a.address < b.address);
}







PubAddress PubAddress::bitcoinAddresFromPubkey(const ECPoint& pubk, const bool compressed)
{	
	const std::string uncompStr = pubk.getRaw(compressed);
	return pubkeyToBitcoinAddress(uncompStr);		
}


std::string PubAddress::getBin() const
{
	BigInt<512> numeric = getNumeric();
	const BigInt<512> fourBytesDropped = numeric >> 32;
	const BigInt<512> ripe = fourBytesDropped & 160;
	const BigInt<160> ripeSmall(ripe);
	return ripeSmall.getBinaryLong();
}



bool PubAddress::isMultisig() const
{
	const BigInt<512> decoded = Base58Decode<512>(address);
	
	//std::cout << "Decoded pub address: " << decoded << std::endl;
	
	const BigInt<512> lastFour = decoded.returnMaskHighBits(32);
	
	const BigInt<512> fourBytesDropped = decoded >> 32;
	//std::cout << "Four bytes dropped: \n" << fourBytesDropped << std::endl;

	const int netVersionInt = getNetVersion();
	const BigInt<512> netVersionBig(netVersionInt);

	const int multiVersionInt = getMultisigVersion();
	const BigInt<512> multiVersionBig(multiVersionInt);


	const BigInt<512> netVersion = fourBytesDropped >> 160;		//160 bit hash
	if(netVersion != netVersionBig && netVersion != multiVersionBig)
	{
		std::string errMsg("Net ver mismatch: ");
		errMsg += netVersion.strHex();
		errMsg += " != ";
		errMsg += netVersionBig.strHex();
		errMsg += " or ";		
		errMsg += multiVersionBig.strHex();
		
		throw errMsg;
	}

	if(netVersion == multiVersionBig)
	{
		return true;
	}
	
	return false;
}





//Throws on error
//Returns checksum
BigInt<32> PubAddress::validatePubAddress(const std::string& address)
{
	const BigInt<512> decoded = Base58Decode<512>(address);
	
	//std::cout << "Decoded pub address: " << decoded << std::endl;
	
	const BigInt<512> lastFour = decoded.returnMaskHighBits(32);
	
	const BigInt<512> fourBytesDropped = decoded >> 32;
	//std::cout << "Four bytes dropped: \n" << fourBytesDropped << std::endl;

	const int netVersionInt = getNetVersion();
	const BigInt<512> netVersionBig(netVersionInt);

	const int multiVersionInt = getMultisigVersion();
	const BigInt<512> multiVersionBig(multiVersionInt);


	const BigInt<512> netVersion = fourBytesDropped >> 160;		//160 bit hash
	//std::cout << "Net version: \n" << netVersion << std::endl;
	if(netVersion != netVersionBig && netVersion != multiVersionBig)
	{
		std::string errMsg("Given net version mismatch: ");
		errMsg += netVersion.strHex();
		errMsg += " != ";
		errMsg += netVersionBig.strHex();
		errMsg += " or ";		
		errMsg += multiVersionBig.strHex();
		
		throw errMsg;
	}
	
/*	
	if(netVersion != netVersionBig)
	{
		std::string errMsg("Net version mismatch: ");
		errMsg += netVersion.strHex();
		errMsg += " != ";
		errMsg += netVersionBig.strHex();
		
		throw errMsg;
	}
*/

	const BigInt<168> toHash(fourBytesDropped);
	//std::cout << "Num bits TO HASH: " << toHash.numberBitsNeeded() << std::endl;
	const std::string toHashStr = toHash.getBinaryLong(); //getBinaryVal(true);
	//std::cout << "LEN: " << toHashStr.size() << std::endl;
	const std::string hashed = sha256(toHashStr);
	const BigInt<512> hash1(hashed, 'c');
	//std::cout << "Hash1: " << hash1 << std::endl;
	
	//std::cout << "Hashed: " << hashed << std::endl;
	const std::string hashedAgain = sha256(hashed);
	const std::string firstFourBytes = hashedAgain.substr(0,4);
	const BigInt<512> checkSum(firstFourBytes, 'c');
	//std::cout << "Checksum: " << checkSum << std::endl;

	if(lastFour != checkSum)
	{
		//std::cout << "Incorrect checksum for: " << address << std::endl;
		throw std::string("Incorrect checksum for public address");
	}

	//std::cout << "Checksum MATCH!!" << std::endl;
	
	//return "";
	const BigInt<32> shortChecksum(checkSum);
	return shortChecksum;
}




PubAddress PubAddress::pubkeyToBitcoinAddress(const std::string& addressStringRepresentation)
{
	const char netVer = getNetVersion();
	return stringToBitcoinAddress(addressStringRepresentation, netVer);
}


PubAddress PubAddress::multisigToBitcoinAddress(const BinaryScript& addressStringRepresentation)
{
	const char netVer = getMultisigVersion();
	return stringToBitcoinAddress(addressStringRepresentation.raw(), netVer);
}






PubAddress PubAddress::fromHex(const std::string& hex)
{	
	const BigInt<160> theRipeInt(BigInt<160>::fromHex(hex));

	const std::string theRipe = theRipeInt.getBinaryLong();

	const char netVersion = getNetVersion();

	return fromBin(theRipe, netVersion);
}
	


PubAddress PubAddress::fromBin(const std::string& theRipe, const char netVersion)
{	
	const std::string netVersionByte(1, netVersion);
	
	const std::string ripeWithVersion = netVersionByte + theRipe;
	
	const std::string ripeShaSha = doubleSha256(ripeWithVersion);

	const std::string checkSum = ripeShaSha.substr(0, 4);
	
	const std::string ripeWithChecksum = ripeWithVersion + checkSum;
	
	BigInt<200> ripeChecksumInt(ripeWithChecksum, 'c');
	
	std::string b58 = Base58Encode(ripeChecksumInt);
	
	return PubAddress(b58);		
}



PubAddress PubAddress::stringToBitcoinAddress(const std::string& addressStringRepresentation, const char netVersion)
{		
	//std::cout << "Will convert this: " << printBin(addressStringRepresentation) << std::endl;
	const std::string theSha = sha256(addressStringRepresentation);
	//std::cout << "Length the sha: " << theSha.size() << std::endl;
	
	//std::cout << "The sha256: " << printBin(theSha) << std::endl;
			
	const std::string theRipe = ripemd160(theSha);
	
	return fromBin(theRipe, netVersion);	
}





std::ostream& operator<<(std::ostream& stream, const PubAddress& addr)
{
	stream << addr.address;
	return stream;
}


bool operator==(const PubAddress& a, const PubAddress& b)
{
	return (a.address == b.address);
}

bool operator!=(const PubAddress& a, const PubAddress& b)
{
	return (a.address != b.address);
}


