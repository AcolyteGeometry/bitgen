/**
 * signatureFileFormats.h - Bitcoin address generator
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

#include "signatureFileFormats.h"

#include "sigFile.h"



std::string removeQuotes(const std::string& str)
{
	const int quotesStart = str.find("\"");
	if(quotesStart == std::string::npos)
	{
		throw std::string("No start quote found");
	}
	
	const int quoteEnd = str.rfind("\"");
	if(quoteEnd == std::string::npos)
	{
		throw std::string("No end quote found");
	}
	const int strSize = quoteEnd - quotesStart - 1;
	
	const std::string onlyStr = str.substr(quotesStart + 1, strSize);
	
	return onlyStr;
}



std::string removeWhitespace(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c == '\n' || c == '\r' || c == '\t' || c == ' ')
		{
			continue;
		}
		
		ret += c;
	}
	
	return ret;
}
	
	
	

std::string getHexStr(const std::string& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
		{
			ret += c;
		}
	}
	
	return ret;
}


std::string splitArmoryLines(const std::string& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		ret += c;
		if((i+1) % 50 == 0)
		{
			ret += "\n            ";
		}
	}
	
	return ret;
}


std::string splitArmoryStrLines(const std::string& str)
{
	std::string ret;
	
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		ret += c;
		if((i+1) % 50 == 0)
		{
			ret += "\"\n            \"";
		}
	}
	
	return ret;
}






ArmoryFileContent ParseArmory(const std::string& content)
{
	//std::cout << "Will parse: " << content << std::endl;

	const std::string sigBlockStart("-----BEGIN-SIGNATURE-BLOCK-------------------------------------");
	const std::string sigBlockEnd("-----END-SIGNATURE-BLOCK---------------------------------------");
	const std::string addressStart("Address:");	
	const std::string messageStart("Message:");
	const std::string pubKeyStart("PublicKey:");
	const std::string signatureStart("Signature:");
	
	const int blockStartPos = content.find(sigBlockStart);
	if(blockStartPos == std::string::npos)
	{
		throw std::string("Armory block start not found");
	}


	const int addressPos = content.find(addressStart, blockStartPos + sigBlockStart.size());
	if(addressPos == std::string::npos)
	{
		throw std::string("Address not found");
	}
	const int addressStrPos = addressPos + addressStart.size();

	const int messagePos = content.find(messageStart, blockStartPos + sigBlockStart.size());
	if(messagePos == std::string::npos)
	{
		throw std::string("Message not found");
	}
	const int messageStrPos = messagePos + messageStart.size();


	const int blockEndPos = content.find(sigBlockEnd);
	if(blockEndPos == std::string::npos)
	{
		throw std::string("Armory block end not found");
	}

	const int pubkeyStartPos = content.find(pubKeyStart, blockStartPos);
	if(pubkeyStartPos == std::string::npos)
	{
		throw std::string("pubkey start not found");
	}
	
	const int pubKeyValPos = pubkeyStartPos + pubKeyStart.size();
	//std::cout << "Pubkey starts at: " << pubKeyValPos << std::endl;
	
	const int signatureStartPos = content.find(signatureStart, pubkeyStartPos);
	if(pubkeyStartPos == std::string::npos)
	{
		throw std::string("pubkey start not found");
	}

	//std::cout << "Sginature starts at: " << signatureStartPos << std::endl;

	const std::string pubVal = content.substr(pubKeyValPos, signatureStartPos- pubKeyValPos);
	//std::cout << "The val: " << pubVal << std::endl;
	
	const std::string hexDigits = getHexStr(pubVal);
	
	//std::cout << "The hex val: " << hexDigits << std::endl << std::endl;
	//Remove the leading 04
	const std::string xy = hexDigits.substr(2);

	//std::cout << "The xy val: " << xy << std::endl;
	if(xy.size() != 128)
	{
		std::cout << "Error, xy size is: " << xy.size() << std::endl;
	}

	const std::string xval = xy.substr(0, 64);
	const std::string yval = xy.substr(64);

	//std::cout << "x: " << x << std::endl;
	//std::cout << "y: " << y << std::endl;
	

	const BigInt<1024> elecX(BigInt<1024>::fromHex(xval.c_str())); //x.c_str());
	const BigInt<1024> elecY(BigInt<1024>::fromHex(yval.c_str())); //Y.c_str());
	
	BitcoinEllipticCurve ec;
	
	const ECPoint elecPubkey = ec.fromCoordinates(elecX, elecY);
		
	//const ECPoint elecPubkey(elecX, elecY);

	//std::cout << "Pub: " << elecPubkey << std::endl << std::endl;;



	//std::cout << "=======================================" << std::endl;

	const int sigValPos = signatureStartPos + signatureStart.size();

	const std::string sigstr = content.substr(sigValPos, blockEndPos - sigValPos);
	//std::cout << "The val: " << sigstr << std::endl;
	
	const std::string rs = getHexStr(sigstr);
	
	//std::cout << "The hex val: " << hexSig << std::endl << std::endl;
	//Remove the leading 04
	//const std::string rs = hexSig.substr(2);

	//std::cout << "The rs val: " << rs << std::endl;
	if(rs.size() != 128)
	{
		std::cout << "Error, rs size is: " << rs.size() << std::endl;
	}

	const std::string rval = rs.substr(0, 64);
	const std::string sval = rs.substr(64);

	//std::cout << "r: " << r << std::endl;
	//std::cout << "s: " << s << std::endl;
	

	const BigInt<1024> elecR(BigInt<1024>::fromHex(rval.c_str())); //r.c_str());
	const BigInt<1024> elecS(BigInt<1024>::fromHex(sval.c_str())); //s.c_str());		
	const RSVal elecSign(elecR, elecS);
	//std::cout << "RS: " << elecSign << std::endl;


	const int addressSize = messagePos - addressStrPos;
	const std::string parsedAddress = content.substr(addressStrPos, addressSize);
	const std::string address = removeWhitespace(parsedAddress);
	//std::cout << "THE P ADDRESS: " << address << std::endl;
	const PubAddress pubAd(address);

	const int messageSize = pubKeyValPos - messageStrPos;
	const std::string message = content.substr(messageStrPos, messageSize);

	//std::cout << "The message: " << message << std::endl;

	//Remove the quotes;
	const std::string onlyMessage = removeQuotes(message);

	//std::cout << "Signed message: " << onlyMessage << std::endl << std::endl;

	const std::string sigStart("Bitcoin Signed Message:\n");
	const std::string fullMsg(sigStart + onlyMessage);
	const std::string hashRaw = doubleSha256(fullMsg); //sha256(sha256(fullMsg));
	const BigInt<256> hash(hashRaw, 'c');

	const ArmoryFileContent sigParam(pubAd, hash, elecPubkey, elecSign);
	return sigParam;
}
