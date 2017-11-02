#ifndef BASE58_H
#define BASE58_H



/**
 * base58.h - Bitcoin address generator
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
#include "base58.h"
#include "bigint.h"
//#include "pubAddress.h"
 

bool isBase58Char(const char c);




template <int NUM_BITS>
BigInt<NUM_BITS> Base58Decode(const std::string& val)
{	
	static const char* base58Chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";	
	const std::string b58c(base58Chars);
	
	BigInt<NUM_BITS> zero;
	BigInt<NUM_BITS> fiftyEight(58);
	
	BigInt<NUM_BITS> ret;
	
	for(int i = 0 ; i < val.size() ; i++)
	{
		const char c = val[i];
		const int pos = b58c.find(c);
		if(pos == std::string::npos)
		{
			throw std::string("Incorrect base58");
		}
		
		const int val = pos;
		//std::cout << "VAL: " << val << std::endl;
		
		ret *= fiftyEight;
		ret += val;
		
	}
	
	
	return ret;
}


//static const char* base58Chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";	


template <int NUM_BITS>
std::string Base58Encode(BigInt<NUM_BITS> val)
{	
	std::string ret;
	static const char* base58Chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";	
	
	BigInt<NUM_BITS> zero;
	BigInt<NUM_BITS> fiftyEight(58);
	
	const int numBits = val.numberBitsNeeded();
	
	while(val > zero)
	{
		BigInt<NUM_BITS> remainder = val % fiftyEight;
		const unsigned int index = remainder.intVal();
		
		const char c = base58Chars[index];
		ret = c + ret;
		
		val = val / fiftyEight;
		
	}
	
	const int numZeroBits = NUM_BITS - numBits;
	//std::cout << "Num zero bits: " << numZeroBits << std::endl;
	
	const int numZeroBytes = numZeroBits / 8;
	
	for(int i = 0 ; i < numZeroBytes ; i++)
	{
		ret = '1' + ret;
	}
	
	return ret;
}



#endif

