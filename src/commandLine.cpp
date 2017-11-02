



#include "commandLine.h"


#include <stdio.h>
#include <iostream>


std::vector<std::string> getExtraArgs(const std::vector<std::string>& args, const int start)
{
	std::vector<std::string> ret;
	int num(0);
	
	for(std::vector<std::string>::const_iterator it = args.begin() ;
		it != args.end() ;
		++it)
	{
		const std::string& str = (*it);
		if(num > start)
		{
			ret.push_back(str);
		}
		
		num++;
	}
	return ret;
}



bool hexChar(const char c)
{
	return (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F');
}




bool oneToSix(const char c)
{
	return (c >= '1' && c <= '6');
}



bool everythingOk(const char c)
{
	return true;
}



std::string readStdinLine(const int minChars, const bool exactNum, bool (*charOk)(const char c))
{
	std::string ret;
	for(;;)
	{
		const char c = getchar();
		//std::cout << "Got: " << int(c) << std::endl;
		
		if(c == '\n')
		{
			if(ret.size() < minChars)
			{
				std::cout << "You have " << ret.size() << " chars. ";
				std::cout << "Still need " << (exactNum ? "" : "at least ") << (minChars - ret.size()) << "." << std::endl;
				std::cout << ret;
				
				continue;
			}
			else
			{
				break;
			}
		}
		else if(c == EOF)
		{
			//std::cout << "Got EOF" << std::endl;
			return ret;
		}
		
		if(charOk(c)) //c >= '1' && c <= '6')
		{
			ret += c;
		}
	}
	
	if(exactNum && (ret.size() > minChars))
	{
		std::cout << "Got too many values, removing extras" << std::endl;
		ret = ret.substr(0, minChars);
	}
	
	std::cout << std::endl;
	return ret;
}



