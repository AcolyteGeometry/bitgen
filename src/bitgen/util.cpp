/**
 * util.cpp - Bitcoin poscript generator
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
 
 
#include "util.h"
#include "bigint.h"

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <fstream>
#include <vector>
#include <list>


int min(const int a, const int b)
{
	return (a < b) ? a : b;
}


std::list<std::string> splitLines(const std::string& str)
{
	std::list<std::string> ret;
	
	const int len = str.size();
	std::string thisStr;
	for(int i = 0 ; i < len ; i++)
	{
		if(str[i] == '\n')
		{
			ret.push_back(thisStr);
			thisStr = "";
		}
		else
		{
			thisStr += str[i];
		}		
	}
	
	return ret;
}



std::string repeat(const char c, const int num)
{
	std::string ret;
	for(int i = 0 ; i < num ; i++)
	{
		ret += c;
	}
	
	return ret;
}


std::string convertFromHex(const std::string hexString)
{
	std::string ret;
	unsigned char byte(0);
	for(int i = 0 ; i < hexString.size() ; i++)
	{
		const unsigned char c = hexString[i];
		if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
		{
			std::string errMsg("Error, digit out of range: ");
			errMsg += c;
			errMsg += " in ";
			errMsg += hexString;
			
			throw errMsg; 
		}
		unsigned char d = (c >= '0' && c <= '9') ? (c - '0') : ((c >= 'a' && c <= 'f') ? (10 + c - 'a') : (10 + c - 'A'));

		if((i % 2) == 0)
		{
			byte |= (d << 4);
		}
		else
		{
			byte |= d;
			ret += byte;
			byte = 0;
		}
		
	}
	
	return ret;
}




//Todo: Move to util
std::string getHexString(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const unsigned char c = str[i];
		if(c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f')
		{
			ret += c;
		}
	}
	
	return ret;
}



std::string minSize(std::string str, const int minSize, const bool right)
{
	if(right)
	{
		while(str.size() < minSize)
		{
			str = std::string(" ") + str;
		}
		
	}
	else
	{
		while(str.size() < minSize)
		{
			str += " ";
		}
	}
	
	return str;
}






//Todo: Rename to plainHexToBigInt()
//Converts a hex string to a big integer
BigInt<256> plainToBigInt(std::string str)
{
	if(str.size() > 32)
	{
		std::cout << "Only 32 chacters allowed" << std::endl;
		throw std::string("Message too long");
	}
	
	while(str.size() < 32)
	{
		str += '\0';
	}
	
	const BigInt<256> num(str, 'c');	
	return num;
}





std::string removeNewLine(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const unsigned char c = str[i];
		if(c == '\n' || c == '\r')
		{
			continue;
		}
		
		ret += c;
	}
	
	
	return ret;
}





std::string hexStrToBytes(const std::string& cStr)
{	
	std::string raw;
	for(int i = 0 ; i < cStr.size() ; i += 2)
	{	
		const std::string subStr = cStr.substr(i, 2);
		BigInt<32> num(BigInt<32>::fromHex(subStr.c_str()));
		//std::cout << "The num: " << num << std::endl;
		const unsigned char c = num.intVal();
		raw += c;
	}
	
	return raw;
}



std::string doubleToString(const double& val)
{
	char str[128];
	sprintf(str, "%f", val);
	
	std::string ret(str);
	return ret;
}

std::string intToString(const int num)
{
	char s[128];
	sprintf(s, "%d", num);
	return s;	
}


std::string toLower(const std::string& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c >= 'A' && c <= 'Z')
		{
			ret += c - 'A' + 'a';
		}
		else
		{
			ret += c;
		}
	}
	
	return ret;	
}


std::string toUpper(const std::string& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c >= 'a' && c <= 'z')
		{
			ret += c - 'a' + 'A';
		}
		else
		{
			ret += c;
		}
	}
	
	return ret;	
}



std::string removePrefix(const std::string& str, const std::string& start)
{
	if(!startsWith(str, start))
	{
		throw std::string("Does not have correct prefix");
	}
	
	return str.substr(start.size());
}



bool endsWith(const std::string& str, const std::string ending)
{
	if(ending.size() > str.size())
	{
		return false;
	}
	
	const std::string myEnding = str.substr(str.size() - ending.size());
	
	return (myEnding == ending);
}


bool startsWith(const std::string& str, const std::string& start)
{
	const std::string myStart = str.substr(0, start.size());
	
	return (myStart == start);
}




std::string removeNewline(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c != '\r' && c != '\n')
		{
			ret += c;
		}
	}
	
	return ret;
}











bool isHexDigit(const char c)
{
	if(c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
	{
		return true;
	}
	
	return false;
}








