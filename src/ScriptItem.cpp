
#include "ScriptItem.h"
#include "scriptdecoder.h"

#include "util.h"
#include "bigintutil.h"

#include <stdio.h>




std::string ScriptItem::toString() const
{
	if(object == OP_HASH160)
	{
		return "OP_HASH160";
	}
	else if(object == OP_EQUALVERIFY)
	{
		return "OP_EQUALVERIFY";
	}
	else if(object == OP_CHECKSIG)
	{
		return "OP_CHECKSIG";
	}		
	else if(object == OP_DUP)
	{
		return "OP_DUP";
	}		
	else if(object == OP_HASH256)
	{
		return "OP_HASH256";
	}
	else if(object == OP_EQUAL)
	{
		return "OP_EQUAL";
	}
	else if(object == OP_CHECKMULTISIG)
	{
		return "OP_CHECKMULTISIG";
	}
	else if(object == OP_NOP)
	{
		return "OP_NOP";
	}
	else if(object == OP_PUSHDATA1)
	{
		return "OP_PUSHDATA1";
	}
	else if(object == OP_0)
	{
		return "OP_0";
	}
	//else if(object == OP_TRUE)	//Todo: Needed? Use NUMBER(1) instead??
	//{
	//	return "OP_TRUE";
	//}
	else if(object == NUMBER)
	{
		//Todo: Add num
		std::string ret("NUMBER(");		
		const unsigned char num = data[0];
		char str[128];
		sprintf(str, "%d", num);
		ret += str; //hexString(data);
		ret += ")";
		
		return ret;
	}
	else if(object == BOOL)
	{
		std::string ret("BOOL(");
		if(data == "1")
		{
			ret += "TRUE";
		}
		else
		{
			ret += "FALSE";
		}
		ret += ")";

		return ret;
	}	
	else if(object == DATA)
	{
		std::string ret("DATA(");
		ret += hexString(data);
		ret += ")";

		return ret;
	}
	else if(object == OP_RETURN)
	{
		std::string ret("OP_RETURN(");
		
		if(startsWith(data, "DOCPROOF"))
		{
			ret += "DOCPROOF ";
			ret += hexString(data.substr(8));
		}
		else
		{
			ret += ScriptDecoder::printable(data);
		}
		
		ret += ")";

		return ret;
	}
	else if(object == OP_CHECKLOCKTIMEVERIFY)
	{
		return "OP_CHECKLOCKTIMEVERIFY";
	}
	else if(object == OP_DROP)
	{
		return "OP_DROP";
	}
	else
	{
		std::string ret("UNK(");
		ret += intToString(int(object));
		ret += ")";
		
		return ret;
	}

}


std::string ScriptItem::scriptInteger(const int blockHeight)
{
	std::string script;
	
	BigInt<256> num(blockHeight);
	const std::string intBytes = num.getBinaryShort();

	const unsigned char byteValSize(intBytes.size());
	script += byteValSize;

	//Take bytes in reverse
	for(int i = (intBytes.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char mybyte = intBytes[i];
		script += mybyte;
	}
	
	
	return script;
}



std::string ScriptItem::serialize() const
{		
	std::string ret;
			
	if(object == OP_HASH160)
	{
		const unsigned char c = '\xA9';
		ret += c;
		return ret;
	}
	else if(object == OP_EQUALVERIFY)
	{
		const unsigned char c = '\x88';
		ret += c;
		return ret;
	}
	else if(object == OP_CHECKSIG)
	{
		const unsigned char c = '\xAC';
		ret += c;
		return ret;
	}
	else if(object == OP_DUP)
	{
		const unsigned char c = '\x76';
		ret += c;
		return ret;		
	}	
	else if(object == OP_HASH256)
	{
		const unsigned char c = '\xAA';
		ret += c;
		return ret;				
	}
	else if(object == OP_EQUAL)
	{
		const unsigned char c = '\x87';
		ret += c;
		return ret;		
	}
	else if(object == OP_CHECKMULTISIG)
	{
		const unsigned char c = '\xAE';
		ret += c;
		return ret;		
	}
	/*
	else if(object == OP_NOP)
	{
		return "OP_NOP";
	}
	else if(object == OP_PUSHDATA1)
	{
		return "OP_PUSHDATA1";
	}
	*/
	else if(object == OP_0)
	{
		const unsigned char c = '\x00';
		ret += char(c);
		return ret;				
	}
	//else if(object == OP_TRUE)	//Todo: Needed? Use NUMBER(1) instead??
	//{
	//	const unsigned char c = '\x51';
	//	ret += char(c);
	//	return ret;				
	//}	
	else if(object == NUMBER)
	{
		//if(data.size() == 1)
		//const unsigned char num = data[0];
		
		if(dataNum >= 1 && dataNum <= 16)
		{
			const int scriptNum = 0x50 + dataNum;
			const unsigned char c = scriptNum;
			ret += c;
		}
		else
		{
			ret += scriptInteger(dataNum);
			//std::cout << "NUMBER not implemented: " << num << std::endl;
			//throw std::string("Number not implemented");
		}
		
		return ret;
	}
	/*
	else if(object == BOOL)
	{
		std::string ret("BOOL(");
		if(data == "1")
		{
			ret += "TRUE";
		}
		else
		{
			ret += "FALSE";
		}
		ret += ")";

		return ret;
	}
	*/
	else if(object == DATA)
	{
		const int len = data.size();
		if(len >= 1 && len <= 75)
		{
			const unsigned char lenChar = len;		

			ret += lenChar;			
			ret += data;			
		}
		else if(len < 256)
		{
			const unsigned char c = 0x4C;
			const unsigned char lenChar = len;		

			ret += c;
			ret += lenChar;			
			ret += data;
		}
		else if(len < 65536)
		{
			const unsigned char c = 0x4D;
			const unsigned char lenCharLow = len % 256;
			const unsigned char lenCharHigh = len / 256;

			ret += c;
			ret += lenCharLow;			
			ret += lenCharHigh;			
			ret += data;
		}
		
		 	 
		else
		{
			std::cout << "DATA with len: " << len << " not implemented" << std::endl;
			throw std::string("Data with this length not implemented");
		}
		
		return ret;
	}
	
	else if(object == OP_RETURN)
	{
		const unsigned char c = '\x6A';
		ret += c;
		return ret;		
		
		/*
		std::string ret("OP_RETURN");
		return ret;
		
		if(startsWith(data, "DOCPROOF"))
		{
			ret += "DOCPROOF ";
			ret += hexString(data.substr(8));
		}
		else
		{
			ret += ScriptDecoder::printable(data);
		}
		
		ret += ")";

		return ret;
		*/
	}
	else if(object == OP_CHECKLOCKTIMEVERIFY)
	{
		const unsigned char c = '\xB1';
		ret += c;
		return ret;		
	}	
	else if(object == OP_DROP)
	{
		const unsigned char c = '\x75';
		ret += c;
		return ret;		
	}
	else
	{
		std::cout << "Serialization not implemented, enum position: " << object << std::endl;
		
		throw std::string("Serialization not implemented");
	}
}


