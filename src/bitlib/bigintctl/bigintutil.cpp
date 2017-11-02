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

#include "bigintutil.h"





char nibbleToHex(const unsigned char c)
{
	if(c > 15)
	{
		throw std::string("Incorrect argument for nibbleToHex");
	}
	
	std::string ret;
	const char chars[] = "0123456789ABCDEF";

	return chars[c];
}


std::string charToHex(const unsigned char c)
{
	std::string ret;
	const char chars[] = "0123456789ABCDEF";
	const unsigned char low = c & 0x0f;

	const unsigned char high = (c >> 4) & 0x0f;

	ret += chars[high];
	ret += chars[low];

	return ret;
}



std::string charToBin(const unsigned char c)
{
	std::string ret;

	char val = c;

	for(int i = 0 ; i < 8 ; i++)
	{
		const bool last = val & 0x01;
		ret += (last ? "1" : "0");
		val >>= 1;
	}

	return ret;
}


std::string hexString(const std::string& binStr)
{
	std::string ret;
	for(unsigned int i = 0 ; i < binStr.size() ; i++)
	{
		const unsigned char c = binStr[i];
		ret += charToHex(c);
	}
	
	return ret;
}

std::string hexStringFull(const std::string& binStr)
{
	return hexString(binStr);
}

std::string addNibble(bool& writeStarted, unsigned char nibble)
{	
	
	if(nibble != 0)
	{				
		writeStarted = true;
	}

	
	std::string ret;
	if(writeStarted)
	{		
		ret += nibbleToHex(nibble);
	}
	
	return ret;
}



std::string hexStringShort(const std::string& binStr)
{
	bool writeStarted(false);
	std::string ret;
	for(unsigned int i = 0 ; i < binStr.size() ; i++)
	{
		const unsigned char c = binStr[i];
		
		const unsigned char low = c & 0x0f;
		const unsigned char high = (c >> 4) & 0x0f;
	
		ret += addNibble(writeStarted, high);
		ret += addNibble(writeStarted, low);		
	}
	
	if(ret.empty())
	{
		ret += "0";
	}
	
	return ret;
}

std::string hexStr(const std::string& binStr)
{
	return hexStringShort(binStr);
}


std::string hexStrFull(const std::string& binStr)
{
	return hexStringFull(binStr);
}


std::string binString(const std::string& binStr)
{
	std::string ret;
	for(unsigned int i = 0 ; i < binStr.size() ; i++)
	{
		const unsigned char c = binStr[i];
		ret += charToBin(c);
	}
	
	return ret;
}




//std::string decString(const std::string& binStr)
//{	
//	return "";
//}




