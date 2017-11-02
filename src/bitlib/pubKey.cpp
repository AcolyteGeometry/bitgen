/**
 * pubKey.cpp - Bitcoin poscript generator
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


#include "pubKey.h"
#include "base58.h"


bool operator<(const PubKey& a, const PubKey& b)
{
	return (a.point.x < b.point.x);
}




ECPoint PubKey::parseVerbosePubKey(const BigInt<1024>& xy)
{
	const BigInt<1024> x = xy >> 256;
	const BigInt<1024> y = xy.returnMaskHighBits(256);
	
	const ECPoint ret(x, y);
	
	BitcoinEllipticCurve ec;
		
	if(!ec.verifyECPoint(ret))
	{
		throw std::string("Error, point not on curve");
	}
	
	return ret;
	
}





std::string PubKey::getChecksum() const
{	
	const std::string stringToMakeHashOf = getRawPKIFWithoutChecksum();
	const std::string theSha = doubleSha256(stringToMakeHashOf);
		
	const std::string checkSum = theSha.substr(0, 4);
	return checkSum;
}



std::string PubKey::getRawPKIFWithoutChecksum() const
{	
	const bool isOdd = point.y.odd();
	
	const BigInt<256> xVal(point.x);
	
	std::string xStr = xVal.getBinaryLong(); //getBinaryVal(true);
		
	if(xStr.size() != 32)
	{
		throw std::string("Incorrect public key len");
	}
	
	
	//std::cout << "Len: " << cStrWithChecksum.size() << std::endl;
	BigInt<264> val(xStr, 'c');	
	//std::cout << "VAL1      : " << val << std::endl;
	BigInt<264> num(3);	
	num <<= 262;
	val += num;		//Todo: OR!
	

	//Indicates if encryption of sign verification must be followed
	//0 => Any use permitted
	//1 => Only specifiedy use permitted	
	
	if(onlyIndicatedUsage)
	{
		BigInt<264> compNum(1);
		compNum <<= 259;
		val += compNum;	//Todo: OR!
	}
	
	//Indicates encryption or signature verification	
	//0 => Encrypt
	//1 => Signature verification	
	
	if(signVerify)
	{
		BigInt<264> compNum(1);
		compNum <<= 258;
		val += compNum;	//Todo: OR!
	}	
	
	if(compressed)	
	{
		BigInt<264> compNum(1);
		compNum <<= 257;
		val += compNum;	//Todo: OR!
	}	

	if(isOdd)	
	{
		BigInt<264> compNum(1);
		compNum <<= 256;
		val += compNum;	//Todo: OR!
	}	


	
	//std::cout << "VAL2    : " << val << std::endl;
	const std::string stringToMakeHashOf = val.getBinaryLong(); //getBinaryVal(true);
	return stringToMakeHashOf;
}




//Calculates the PKIF
std::string PubKey::createPublicKeyImportFormat() const
{	
	const std::string stringToMakeHashOf = getRawPKIFWithoutChecksum();
	
	const std::string theSha = doubleSha256(stringToMakeHashOf);
	//std::cout << "sha1 " << printBin(theSha) << std::endl;	
	const std::string checkSum = theSha.substr(0, 4);
	//std::cout << "The Generated checksum: " << printBin(checkSum) << std::endl;
	const std::string cStrWithChecksum = stringToMakeHashOf + checkSum;
	//std::cout << "extended with checksum: " << printBin(extendedWithChecksum) << std::endl;

	BigInt<296> myval(cStrWithChecksum, 'c');	
	
	const std::string b58Str = Base58Encode(myval);
	//std::cout << "B58:      : " << b58Str << std::endl;

	return b58Str;
}

