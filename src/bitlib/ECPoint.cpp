/**
 * ECPoint.cpp - Bitcoin poscript generator
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

#include "ECPoint.h"
#include "bitcoinEC.h"
//#include "util.h"


//Decodes a raw point in binary form
ECPoint ECPoint::parsePointBin(const std::string& raw, bool& compressed)
{	
	//std::cout << "The orig string: " << printBin(raw) << std::endl;
	
	if(raw.empty())
	{
		throw std::string("Error in raw format");
	}
	
	const unsigned char c = raw[0];
	const std::string xStr = raw.substr(1);
	
	if(c == 0x02 || c == 0x03)
	{
		compressed = true;
		
		const bool parityOdd = (c == 0x03);
		
		if(xStr.size() != 32)
		{
			throw std::string("Error in x value");
		}
	
		const BigInt<1024> x(xStr, 'c');
	
		BitcoinEllipticCurve ec;
		const ECPoint point = ec.recoverPointFromXAndParityOdd(x, parityOdd);
		if(!ec.verifyECPoint(point))
		{
			throw std::string("Error, not a valid EC point");
		}
		
		return point;
	}
	else if(c == 0x04)
	{
		compressed = false;
		
		const BigInt<1024> xy(xStr, 'c');
		
		const BigInt<1024> x = xy >> 256;
		const BigInt<1024> y = xy.returnMaskHighBits(256);
	
		const ECPoint ret(x, y);
		BitcoinEllipticCurve ec;
		if(!ec.verifyECPoint(ret))
		{
			throw std::string("Error, not a valid EC point");
		}

	
		return ret;		
	}
	else
	{
		throw std::string("Error in raw format, first byte error");
	}	
}




//Todo: Make member of ECPoint
//049844DD14FFD74509620FCEABEBB2EF3DEEB451B77C4EF02025D9C0BC34CFAD03ACC804D425550F085096BBC46054C03EC82FD0AE8EE3BDFA8D228F2A3ECB0297
ECPoint ECPoint::parsePointHex(const std::string& pubKey, bool& compressed)
{
	if(pubKey.size() != 130 && pubKey.size() != 66)
	{
		//std::cout << "pubkey size: " << pubKey.size() << std::endl;
		throw std::string("Incorrect public key size");
	}
	
	const std::string zeroVal = pubKey.substr(0, 2);
	if(zeroVal != "02" && zeroVal != "03" && zeroVal != "04")
	{
		throw std::string("Error, pubkey does not start with 02, 03 or 04");
	}

	if(zeroVal == "04")
	{
		if(pubKey.size() != 130)
		{
			throw std::string("Incorrect public key size for 04");
		}
		const std::string xStr = pubKey.substr(2, 64);
		const std::string yStr = pubKey.substr(66, 64);
		
		const BigInt<1024> x(BigInt<1024>::fromHex(xStr)); //.c_str());
		const BigInt<1024> y(BigInt<1024>::fromHex(yStr)); //yStr.c_str());


		compressed = false;	
		const ECPoint ret(x, y);
		
		BitcoinEllipticCurve ec;		
		if(!ec.verifyECPoint(ret))
		{
			std::cout << "x: " << xStr << std::endl;
			std::cout << "y: " << yStr << std::endl;
			
			throw std::string("Error, uncompressed EC point not on curve");
		}
		
		return ret;		
	}
	else
	{
		if(pubKey.size() != 66)
		{
			throw std::string("Incorrect public key size for 02/03");
		}

		const bool parityOdd = (zeroVal == "03");
		
		const std::string xStr = pubKey.substr(2);

		const BigInt<1024> x(BigInt<1024>::fromHex(xStr));

		compressed = true;
		BitcoinEllipticCurve ec;
		const ECPoint ret = ec.recoverPointFromXAndParityOdd(x, parityOdd);
		
		if(!ec.verifyECPoint(ret))
		{
			throw std::string("Error, compressed EC point not on curve");
		}		
		
		return ret;
	}	
}




bool operator==(const ECPoint& a, const ECPoint& b)
{
	return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const ECPoint& a, const ECPoint& b)
{
	return !(a == b);
}



std::ostream& operator<<(std::ostream& s, const ECPoint& point)
{
	s << "(" << point.x << "\n" << point.y << ")";
	return s;
}

/*
std::string ECPoint::getNonCompressedHex() const
{
	return printBin(getRawNonCompressed());
}
*/

/*
std::string ECPoint::getCompressedHex() const
{
	return printBin(getRawCompressed());
}
*/


/*
std::string ECPoint::getKeyHex(const bool compressed) const
{	
	if(compressed)
	{
		const std::string str = getRawCompressed();
		return printBin(str);		
	}
	else
	{
		const std::string str = getRawNonCompressed();	
		return printBin(str);
	}
}
*/


std::string ECPoint::getRaw(const bool compressed) const
{
	if(compressed)
	{
		return getRawCompressed();
	}
	else
	{
		return getRawNonCompressed();
	}
}


std::string ECPoint::getRawCompressed() const
{
	BigInt<264> px(x);
	
	const bool isOdd = y.odd();
	
	BigInt<264> pubkey;	
	
	if(isOdd)
	{
		const BigInt<264> three(3);
		pubkey += three;
	}
	else
	{
		const BigInt<264> two(2);
		pubkey += two;
	}
	
	pubkey <<= 256;	
	pubkey += px;	
	
	
	const std::string binStr = pubkey.getBinaryLong(); //getBinaryVal();	
	return binStr;
}





std::string ECPoint::getRawNonCompressed() const
{

	BigInt<520> px(x);
	BigInt<520> py(y);
	
	BigInt<520> pubkey;	
	const BigInt<520> four(4);
	pubkey += four;	
	pubkey <<= 256;	
	pubkey += px;	
	pubkey <<= 256;
	pubkey += py;
	
	const std::string binStr = pubkey.getBinaryLong(); //getBinaryVal();
	
	return binStr;
}









