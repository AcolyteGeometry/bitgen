/**
 * wif.cpp - Bitcoin poscript generator
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

#include "wif.h"


#include "hash.h"
#include "base58.h"
#include "privKey.h"
#include "currencySpecific.h"




bool operator<(const Wif& a, const Wif& b)
{
	return (a.m_wif < b.m_wif);
}




PrivKey Wif::getPriv() const
{
	bool compressed(false);
	const BigInt<256> priv = wifToPrivKey(compressed);
	const PrivKey privKey(priv, compressed);
	return privKey;
}






Wif Wif::privkeyToWIF(const BigInt<256>& privkey, bool compressed)
{
	const std::string binStr = privkey.getBinaryLong();
	//std::cout << "Binstr length: " << binStr.size() << std::endl;	
	//std::cout << "Binstr: " << printBin(binStr) << std::endl;

	const char wifPrefix = getWifPrefix();

	const std::string wifPrefixByte(1, wifPrefix);
	
	std::string extendedBinStr = wifPrefixByte + binStr;
	//std::cout << "Extended private " << printBin(extendedBinStr) << std::endl;
	if(compressed)	
	{
		const char zeroOne(0x01);
		
		extendedBinStr += zeroOne;
	}
		
	//std::cout << "||||||||| Will compute CS for: " << printBin(extendedBinStr) << std::endl;
	const std::string theSha = sha256(extendedBinStr);
	//std::cout << "sha1 " << printBin(theSha) << std::endl;
	
	const std::string theSha2 = sha256(theSha);
	//std::cout << "sha2 " << printBin(theSha2) << std::endl;

	const std::string checkSum = theSha2.substr(0, 4);
	//std::cout << "The Generated checksum: " << printBin(checkSum) << std::endl;

	const std::string extendedWithChecksum = extendedBinStr + checkSum;
	//std::cout << "extended with checksum: " << printBin(extendedWithChecksum) << std::endl;

	
	const BigInt<300> checksumInt(extendedWithChecksum, 'c');
	//std::cout << "extended with checksum2: " << checksumInt << std::endl;

	const std::string b58 = Base58Encode(checksumInt);
	
	//std::cout << "Will return a wif" << std::endl;
	return Wif(b58);
}



const BigInt<256> Wif::wifToPrivKey(bool& compressed) const
{
	//std::cout << "???????WIF size: " << address.size() << std::endl;
	compressed = (m_wif.size() == 52);
	
	if(compressed)
	{
		//std::cout << "Compressed wif" << std::endl;
		
	}
	else
	{
		//std::cout << "Noncompressed wif" << std::endl;
	}

	
	const BigInt<512> decoded = Base58Decode<512>(m_wif);
	
	//std::cout << "Decoded: " << decoded << std::endl;
	
		
	//Take the last four bytes
	const BigInt<512> lastFour = decoded.returnMaskHighBits(32);
	
	const BigInt<512> fourBytesDropped = decoded >> 32;
	//std::cout << "Four bytes dropped: \n" << fourBytesDropped << std::endl;


	std::string toHashStr;
	if(compressed)
	{
		const BigInt<272> toHash(fourBytesDropped); 
		//std::cout << "Num bits TO HASH: " << toHash.numberBitsNeeded() << std::endl;
		toHashStr = toHash.getBinaryLong(); //getBinaryVal(true);
		//std::cout << "LEN: " << toHashStr.size() << std::endl;
	}
	else
	{
		const BigInt<264> toHash(fourBytesDropped); 
		//std::cout << "Num bits TO HASH: " << toHash.numberBitsNeeded() << std::endl;
		toHashStr = toHash.getBinaryLong(); //getBinaryVal(true);
		//std::cout << "LEN: " << toHashStr.size() << std::endl;
		
	}
	
	//std::cout << "|||||||||||Will verify CS for: " << printBin(toHashStr) << std::endl;

	
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
		//std::cout << "Last: " << lastFour << std::endl;
		//std::cout << "CS: " << checkSum << std::endl;
		
		
		std::string errMsg;
		errMsg += "Incorrect wif checksum: ";
		errMsg += m_wif;
		errMsg += " Last: ";
		errMsg += lastFour.strHex();
		errMsg += " CS: ";
		errMsg += checkSum.strHex();		
		throw errMsg;
	}

	//std::cout << "Higest bitnum: " << fourBytesDropped.highestBitsNum() << std::endl;
	//std::cout << "Num bits: " << fourBytesDropped.numberBitsNeeded() << std::endl;
	
	
	
	if(compressed)
	{
		const BigInt<512> one(1);
		const BigInt<512> oneByte = fourBytesDropped.returnMaskHighBits(8);
		if(oneByte != one)
		{
			std::cout << "Byte val: " << oneByte << std::endl;
			throw std::string("Error, Wif compressed byte should equal one");
		}
	}	

	const BigInt<512> eightAndPrivBytes = compressed ? (fourBytesDropped >> 8) : fourBytesDropped; 


	//const int shiftLen = compressed ? 264 : 256;
	const BigInt<512> highByte = eightAndPrivBytes >> 256;
	//std::cout << "High byte: " << highByte << std::endl;
	
	const char* wifPrefix = getWifHighBytePrefix();
	const BigInt<512> highByteShould(wifPrefix);

	if(highByte != highByteShould)
	{
		//std::cout << "High byte value: " << highByte << " should be: " << highByteShould << std::endl;
		
		std::string errMsg("High byte value: ");
		errMsg += highByte.strHex();
		errMsg += " should be ";		
		errMsg += highByteShould.strHex();
		
		throw errMsg; //std::string("Incorrect wif high byte");
	}



	
	const BigInt<512> afterMask = eightAndPrivBytes.returnMaskHighBits(256);
	//std::cout << "???????????Masked: " << afterMask << std::endl;

	
	const BigInt<256> ret(afterMask);
	return ret;
}


std::ostream& operator<<(std::ostream& stream, const Wif& addr)
{
	stream << addr.m_wif;
	return stream;
}


