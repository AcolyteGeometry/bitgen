/**
 * base64.cpp - Bitcoin address generator
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
 
 
#include "base64.h"
#include "bigint.h"

static const std::string base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";







std::string base64_encode(const std::string& str)
{
	std::string ret;

	for(int i = 0 ; i < str.size() ; i+= 3)
	{
		
		std::string s = str.substr(i, 3);
		const int origSize = s.size();
		const int quadSize = s.size() + 1;
		
		for(int k = 0 ; k < (3 - origSize) ; k++)
		{
			s += char('\0');
		}
		
		const int lastStop = 4 - quadSize;

		BigInt<64> val(s, 'c');
		//std::cout << "Created val: " << val << std::endl;
		
		std::string quadStr;
		
		for(int j = s.size() ; j >= lastStop  ; j--)
		{
			BigInt<64> thisVal(val); 
			thisVal >>= (j * 6);
			thisVal &= 6; //.maskHighBits(6);

			const int index = thisVal.intVal();
			
			const char c = base64_chars[index];
			quadStr += c;
		}
		
		ret += quadStr;
		
		for(int j = 0 ; j < (4 - quadSize) ; j++)
		{
			ret += "=";
		}
	}
	
	return ret;
}






int base64CharToInt(const unsigned char c)
{
	if(c == '=')
	{
		return 0;
	}
	
	for(int k = 0 ; k < base64_chars.size() ; k++)
	{
		if(base64_chars[k] == c)
		{
			return k;
		}
	}
	
	std::cout << "Incorrect char: " << c << " (" << int(c) << ")" << std::endl;
	throw std::string("Error, incorrect base64 char");
	return 0;
}






std::string base64_decode(std::string const& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i+= 4)
	{
		BigInt<24> val;
		
		const std::string s = str.substr(i, 4);
		
		const int len = s.size();
		for(int j = 0 ; j < len ; j++)
		{
			unsigned char c = s[j];			
			const int k = base64CharToInt(c);
			
			val <<= 6;
			val += k;
		}
		
		if(i >= (str.size() - 4))
		{
			//Last part, count the number of "="
			int k(0);
			for( ; k < s.size() ; k++)
			{
				if(s[k] == '=')
				break;
			}
			
			const std::string lastBin = val.getBinaryLong(); //getBinaryVal(true);
			const std::string lastConc = lastBin.substr(0, k - 1);
			ret += lastConc;
		}
		else
		{
			ret += val.getBinaryLong(); //getBinaryVal(true);
		}
	}
		
	return ret;
}




