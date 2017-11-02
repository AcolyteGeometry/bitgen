#ifndef BIG_INT_UTIL_H
#define BIG_INT_UTIL_H
/**
 * bigint - Big integer class
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


#include <string>
#include "bigint.h"

//Use these
std::string hexStr(const std::string& binStr);
std::string hexStrFull(const std::string& binStr);

std::string binString(const std::string& binStr);


//These are deprecated
std::string hexString(const std::string& binStr);		//Todo: call hexStringShort(), not Full()
std::string hexStringShort(const std::string& binStr);	//Do not use, use hexStr() instead
std::string hexStringFull(const std::string& binStr);	//Do not use, use hexStrFull() instead


	//BigInt power(const int exp) const;
	//BigInt powerLarge(const BigInt<NUM_BITS>& exp) const;
template <int NUM_BITS>
BigInt<NUM_BITS> power(const BigInt<NUM_BITS>& base, const int exp)
{
	const BigInt<NUM_BITS> multiplicator(base);
	BigInt<NUM_BITS> currentVal(1);	

	for(int i = 0 ; i < exp ; i++)
	{
		currentVal *= multiplicator;
	}

	return currentVal;
}


template <int NUM_BITS>
BigInt<NUM_BITS> powerLarge(const BigInt<NUM_BITS>& base, const BigInt<NUM_BITS>& exp)
{
	//std::cout << "Calc: " << x << " ^" << y << std::endl;
	BigInt<NUM_BITS> result(1);
	
		
	const int loopMax = exp.numberBitsNeeded();
	//std::cout << "Will loop to: " << loopMax << std::endl;
	BigInt<NUM_BITS> multiplicator(base);

	
    for(int i = 0 ; i < loopMax ; i++)
    {
		if(exp[i])
		{
			//std::cout << "multiplying: " << i << std::endl;
			result = (result * multiplicator);
		}
		
		multiplicator = multiplicator * multiplicator;
	}
	        
    return result;
}


/*
template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::power(const int exp) const
{
	const BigInt multiplicator((*this));
	BigInt currentVal(1);	

	for(int i = 0 ; i < exp ; i++)
	{
		currentVal *= multiplicator;
	}

	return currentVal;
}
*/



/*
template <int NUM_BITS>
BigInt<NUM_BITS> BigInt<NUM_BITS>::powerLarge(const BigInt<NUM_BITS>& exp) const
{
	//std::cout << "Calc: " << x << " ^" << y << std::endl;
	BigInt result(1);
	
		
	const int loopMax = exp.numberBitsNeeded();
	//std::cout << "Will loop to: " << loopMax << std::endl;
	BigInt multiplicator((*this));

	
    for(int i = 0 ; i < loopMax ; i++)
    {
		if(exp[i])
		{
			//std::cout << "multiplying: " << i << std::endl;
			result = (result * multiplicator);
		}
		
		multiplicator = multiplicator * multiplicator;
	}
	        
    return result;
}
*/



#endif
