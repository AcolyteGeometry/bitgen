#ifndef BITCOIN_EC
#define BITCOIN_EC

/**
 * bitcoinEC.h - Bitcoin generator
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
 
#include "bigint.h"
#include <iostream>

#include "ECPoint.h"



class BitcoinEllipticCurve
{
public:


	BitcoinEllipticCurve() : 
		n(Nval()),
		p(Pval()),
		Gx("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"),
		Gy("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"),
		genPoint(Gx, Gy),
		one1024(1),
		PplusOneDivFour( (p + one1024)  >> 2 ),
		two(2),
		three(3),
		seven(7),
		oneSmall(1)
	{	
	}

	
//Todo: Rename to recoverPointFromXAndParity()
	
	ECPoint fromCoordinates(const BigInt<1024>& x, const BigInt<1024>& y)
	{
		const ECPoint point(x, y);
		
		if(!verifyECPoint(point))
		{
			throw std::string("Incorrect point");
		}
		
		return point;
	}

	
	ECPoint recoverPointFromXAndParityOdd(const BigInt<1024>& x, const bool parityOdd);

	
	BigInt<1024> modinv(const BigInt<1024>& a, const BigInt<1024>& n) const;
	
	BigInt<1024> pow_mod(BigInt<1024> x, const BigInt<1024>& y);
	
	BigInt<1024> modNsqrt(const BigInt<1024>& x);
	
	
	ECPoint add(const ECPoint& a, const ECPoint& b) const;
	ECPoint dodouble(const ECPoint& a) const;
	ECPoint multiplyGenPoint(const BigInt<1024>& ScalarHex) const;
	ECPoint multiply(const ECPoint& genPoint, const BigInt<1024>& val) const;

	ECPoint addGenPoint(const ECPoint& p) const;
	
	static const char* Nval() { return "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"; }
	static const char* Pval() { return "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"; }
	
	const BigInt<1024> n;
	const BigInt<1024> p;
	
	const BigInt<1024> Gx;
	const BigInt<1024> Gy;

	const ECPoint genPoint;
	
	const BigInt<1024> one1024;
	const BigInt<1024> PplusOneDivFour;
	const BigInt<1024> zero;
	const BigInt<1024> two;
	const BigInt<1024> three;
	const BigInt<1024> seven;

	const BigInt<257> oneSmall;

	bool verifyECPoint(const ECPoint& p);

private:	

	bool getParityFromCompressed(const BigInt<1024>& compKey);
	ECPoint decompressKey(const BigInt<1024>& compKey);	
};



#endif

