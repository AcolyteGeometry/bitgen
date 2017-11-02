/**
 * bitcoinEC.cpp - Bitcoin generator
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

#include "bitcoinEC.h"


//Todo: Unit tests!!!!!!!





ECPoint BitcoinEllipticCurve::addGenPoint(const ECPoint& p) const
{
	const ECPoint point = add(p, genPoint);
	return point;
}





//Verify that the point is located on the curve
bool BitcoinEllipticCurve::verifyECPoint(const ECPoint& point)
{	
	const BigInt<1024> ySquare = (point.y * point.y) % p;
	//std::cout << "yS: " << ySquare << std::endl;

	const BigInt<1024> xCubePlusSeven = (point.x * point.x * point.x + seven) % p;
	//std::cout << "xC: " << xCubePlusSeven << std::endl;

	return ySquare == xCubePlusSeven;
}




//uncompressed_key	
//y^2 = x^3+ 7
//y = sqrt(x^3+ 7);
//sqrt(a) = x ^ ((q+1)/4)	


//sqrt(a) = x ^ ((q+1)/4)
BigInt<1024> BitcoinEllipticCurve::modNsqrt(const BigInt<1024>& x)
{	
	const BigInt<1024> result = pow_mod(x, PplusOneDivFour);
		
	return result;
}



//Calculates (x ^ y) % z
BigInt<1024> BitcoinEllipticCurve::pow_mod(BigInt<1024> x, const BigInt<1024>& y)
{
	//std::cout << "Calc: " << x << " ^" << y << std::endl;
	BigInt<1024> result(1);
	//BigInt<1024> P(BitcoinEllipticCurve::Pval());	
			
	const int loopMax = y.numberBitsNeeded();
	//std::cout << "Will loop to: " << loopMax << std::endl;

	
    for(int i = 0 ; i < loopMax ; i++)
    {
		if(y[i])
		{
			//std::cout << "multiplying: " << i << std::endl;
			result = (result * x) % p;
			//std::cout << "r: " << result << std::endl;
			
		}
		
		x = x * x % p; //P;
	}
	        
    return result;
}








bool BitcoinEllipticCurve::getParityFromCompressed(const BigInt<1024>& compKey)
{
	const BigInt<1024> parityIndicator = compKey >> 256;
	//std::cout << "ParityIndicator: " << parityIndicator << std::endl;
		
	if(parityIndicator == two)
	{
		return true;
	}
	else if(parityIndicator == three)
	{
		return false;
	}
	else
	{
		throw std::string("Error, incorrent parity indicator");
	}
}




ECPoint BitcoinEllipticCurve::decompressKey(const BigInt<1024>& compKey)
{
	const bool parity = getParityFromCompressed(compKey);

	const BigInt<1024> x(compKey.returnMaskHighBits(256));
	std::cout << "x is " << x << std::endl;
	
	//x * x * x + 7
	const BigInt<1024> a = (pow_mod(x, three) + seven) % p;
	//std::cout << "a: " << a << std::endl;
	BigInt<1024> y = BitcoinEllipticCurve::modNsqrt(a); 
	//std::cout << "y1: " << y << std::endl;
	
	if(!y.odd() != parity)
	{
		//std::cout << "Using neg value" << std::endl;
		y = (zero - y) % p; //P;	
	}
	//std::cout << "y2: " << y << std::endl;

	
	ECPoint ret(x, y);
	
	return ret;
}


//Todo: Rename to recoverPointFromXAndParity()
ECPoint BitcoinEllipticCurve::recoverPointFromXAndParityOdd(const BigInt<1024>& x, const bool parityOdd)
{
	//std::cout << "Want Y to be: " << (parityOdd ? "odd" : "even") << std::endl;
	
	
	//x * x * x + 7
	const BigInt<1024> a = (pow_mod(x, three) + seven) % p;
	//std::cout << "alpha: " << a << std::endl;
	BigInt<1024> y = modNsqrt(a); 
	//std::cout << "other beta: " << y << std::endl;
	
	//std::cout << "The Y is now: " << (y.odd() ? "odd" : "even") << std::endl;
	//std::cout << "YYYYYYYYY1: " << y << std::endl;
	
	if(y.odd() != parityOdd)
	{
		//std::cout << "Using neg value" << std::endl;
		y = (zero - y) % p; //P;	
	}
	else
	{
		//std::cout << "Keeping y val" << std::endl;
	}
	
	//std::cout << "other y2: " << y << std::endl;
	
	//std::cout << "y2: " << y << std::endl;

	
	ECPoint ret(x, y);
	
	if(!verifyECPoint(ret))
	{
		throw std::string("Error, point not on curve");
	}
	
	return ret;
}











BigInt<1024> BitcoinEllipticCurve::modinv(const BigInt<1024>& inA, const BigInt<1024>& inN) const
{	
	const BigInt<257> a(inA);
	const BigInt<257> n(inN);
	
	//const BigInt<257> oneSmall(1);
	
    BigInt<257> lm(1);
    BigInt<257> hm(0);
    
    BigInt<257> low  = a % n;
    BigInt<257> high = n;

    
    while(low > oneSmall)
    {
        const BigInt<257> ratio = high / low;
        
        const BigInt<257> nm = hm - lm * ratio;
        
        const BigInt<257> theNew = high - low * ratio;
        
        const BigInt<257> lmCopy(lm);
        const BigInt<257> lowCopy(low);
        
        lm = nm;
        low = theNew;
        hm = lmCopy;
        high = lowCopy;        
	}

	const BigInt<257> result(lm % n);
	const BigInt<1024> res(result);
	
    return res;
}





ECPoint BitcoinEllipticCurve::add(const ECPoint& a, const ECPoint& b) const
{
	const BigInt<1024> LamAdd = ((b.y-a.y) * modinv(b.x - a.x, p)) % p;
    const BigInt<1024> x = (LamAdd * LamAdd - a.x - b.x) % p;
    const BigInt<1024> y = (LamAdd * (a.x - x) - a.y) % p;
    
    return ECPoint(x,y);
}



ECPoint BitcoinEllipticCurve::dodouble(const ECPoint& a) const
{	
	const BigInt<1024> firstpart = (three * a.x * a.x);
	
	const BigInt<1024> inv = modinv((two * a.y), p);
	
    const BigInt<1024> Lam = (firstpart * inv) % p;
    
    
    const BigInt<1024> x = (Lam * Lam - 2 * a.x) % p;
	const BigInt<1024> y = (Lam * (a.x - x) - a.y) % p;
    return ECPoint(x,y);
}



ECPoint BitcoinEllipticCurve::multiplyGenPoint(const BigInt<1024>& val) const
{	
    if(val == zero)
    {
		throw std::string("Private Key cannot be zero");
	}
	
    if(val >= n)
    {
		std::string errMsg("Private Key cannot be larger than N.\n");
		errMsg += val.strHex();
		errMsg += " larger than \n";
		errMsg += n.strHex();
		
		throw errMsg;
	}

	
    ECPoint q = genPoint;
    
    const int startBit = val.numberBitsNeeded() - 2; //val.highestBitsNum() - 1; //PPPP
    
    for(int i = startBit ; i >= 0 ; i--)
    {
		q = dodouble(q);

        const int bitNum = i;
        if(val[bitNum])
        {
			q = add(q, genPoint);
		}
	}
		
    return q;
}



ECPoint BitcoinEllipticCurve::multiply(const ECPoint& genPoint, const BigInt<1024>& val) const
{		
    if(val == zero)
    {
		throw std::string("Private Key cannot be zero");
	}
	
    if(val >= n)
    {
		std::cout << "Error, the val " << val << " is larger than " << n << std::endl;
		throw std::string("Private Key cannot be larger than N");
	}

	
    ECPoint q = genPoint;
    
    
    const int startBit = val.numberBitsNeeded() - 2; //val.highestBitsNum() - 1; //PPPP
    
    for(int i = startBit ; i >= 0 ; i--)
    {
		q = dodouble(q);

        const int bitNum = i;
        if(val[bitNum])
        {
			q = add(q, genPoint);
		}
	}


    return q;
}










