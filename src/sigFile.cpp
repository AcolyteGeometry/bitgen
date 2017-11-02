/**
 * sigFile.cpp
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

#include "sigFile.h"


#include "signatureFileFormats.h"
#include "base64.h"
#include "version.h"
#include "bigintutil.h"
#include "RSVal.h"
#include "FileUtil.h"

#include "util.h"




RSVal decode65SignatureBytes(const std::string& decoded, bool& firstX, bool& oddVal, bool& compressed)
{	
	//std::cout << "Decoded size: " << decoded.size() << std::endl;
	if(decoded.size() != 65)
	{
		std::cout << "Number of bytes: " << decoded.size() << " expecting 65" << std::endl;
		throw std::string("Unexpected number of chars in signature");
	}

	const unsigned char signatureByte = decoded[0];
	const int signatureVal(signatureByte);
	//std::cout << "Signature: " << signatureVal << std::endl;;
	
	validateSigatureByte(signatureByte);
	

	firstX = (signatureVal == 0x1B || signatureVal == 0x1C || 
						signatureVal == 0x1F || signatureVal == 0x20);
						
	oddVal = (signatureVal == 0x1C || signatureVal == 0x1E ||
						signatureVal == 0x20 || signatureVal == 0x22);

	compressed = (signatureVal == 0x1F || signatureVal == 0x20 ||
							signatureVal == 0x21 || signatureVal == 0x22);

	
	//std::cout << "THE Y SHOULD BE: " << (oddVal ? "odd" : "even") << std::endl;
	
	//Get the rs value
	const std::string rStr = decoded.substr(1, 32);
	const std::string sStr = decoded.substr(33);
	
	const BigInt<1024> r(rStr, 'c');
	const BigInt<1024> s(sStr, 'c');

	//std::cout << "R: " << r << std::endl;
	//std::cout << "S: " << s << std::endl;
	
	const RSVal rs(r, s);
	return rs;
}


RSVal decodeBase64Signature(const std::string& signature, bool& firstX, bool& oddVal, bool& compressed)
{	
	const std::string decoded = base64_decode(signature);
	return decode65SignatureBytes(decoded, firstX, oddVal, compressed);
}






	
void validateSigatureByte(const unsigned char signatureByte)
{
	const int signatureVal(signatureByte);
	
	if(signatureVal == 0x1B)
	{
		//std::cout << "Key0 EvenY Uncomp" << std::endl;
	}
	else if(signatureVal == 0x1C)
	{
		//std::cout << "Key0 OddY Uncomp" << std::endl;
	}
	else if(signatureVal == 0x1D)
	{
		//std::cout << "Key1 EvenY Uncomp" << std::endl;
	}
	else if(signatureVal == 0x1E)
	{
		//std::cout << "Key1 OddY Uncomp" << std::endl;
	}	
	else if(signatureVal == 0x1F)
	{
		//std::cout << "Key0 EvenY Comp" << std::endl;
	}
	else if(signatureVal == 0x20)
	{
		//std::cout << "Key0 OddY Comp" << std::endl;
	}
	else if(signatureVal == 0x21)
	{
		//std::cout << "Key1 EvenY Comp" << std::endl;
	}
	else if(signatureVal == 0x22)
	{
		//std::cout << "Key1 OddY Comp" << std::endl;
	}	
	else
	{
		std::cout << "signature byte: " << signatureVal << std::endl;
		throw std::string("Unexpected signature byte val");
	}
}






int getpubKeyInfoPos(const std::string& str)
{
	const int pos1 = str.find("fo");
	if(pos1 != std::string::npos)
	{
		return pos1;		
	}
	
	const int pos2 = str.find("so");
	if(pos2 != std::string::npos)
	{
		return pos2;
	}
	
	const int pos3 = str.find("fe");
	if(pos3 != std::string::npos)
	{
		return pos3;
	}
	
	const int pos4 = str.find("se");
	if(pos4 != std::string::npos)
	{
		return pos4;
	}


	return -1;
}







std::string createSignatureContent(const BigInt<256>& hashNum, const BigInt<256>& r, const BigInt<256>& s, 
	const bool indexZero, const bool oddY, const bool compressed)
{	
	std::string sigResult;
	sigResult += "------ BEGIN BITCOIN SECP256K1 SIGNATURE ------\n";
	
	sigResult += indexZero ? "f" : "s";		//First/second
	sigResult += oddY ? "o" : "e";			//Odd/even
	sigResult += compressed ? "c" : "u";			//compressed/uncompressed
	sigResult += "\n";
		
	sigResult += hexString(r.getBinaryLong());
	sigResult += "\n";
	sigResult += hexString(s.getBinaryLong());
	sigResult += "\n";
	sigResult += "------ END BITCOIN SECP256K1 SIGNATURE ------\n";

	return sigResult;
}



std::string createFileSignatureContent(const BigInt<256>& hashNum, const BigInt<256>& r, const BigInt<256>& s, 
	const bool indexZero, const bool oddY, const bool compressed, const std::string& content, const bool rawContent)
{	
	std::string sigResult;
	sigResult += "------ BEGIN BITCOIN SECP256K1 CONTENT SIGNATURE ------\n";
	
	sigResult += indexZero ? "f" : "s";		//First/second
	sigResult += oddY ? "o" : "e";			//Odd/even
	sigResult += compressed ? "c" : "u";			//compressed/uncompressed
	sigResult += "\n";
		
	sigResult += hexString(r.getBinaryLong()); //r.strHex();	//Todo: Print as positive?
	sigResult += "\n";
	sigResult += hexString(s.getBinaryLong()); //s.strHex();	//Todo: Print as positive?	
	sigResult += "\n";
	sigResult += "=========================== Content ============================";
	sigResult += "\n";
	
	if(rawContent)
	{
		sigResult += content;
	}
	else
	{
		const std::string hexStr = hexString(content);
		for(int i = 0 ; i < hexStr.size() ; i++)
		{
			sigResult += hexStr[i];
			if((i + 1) % 64 == 0)
			{
				sigResult += "\n";
			}			
		}
		sigResult += "\n";
	}

	sigResult += "=========================== End content ========================";
	sigResult += "\n";	
	sigResult += "------ END BITCOIN SECP256K1 CONTENT SIGNATURE ------\n";

	return sigResult;
}










SigFileContent parseSignedWithIncludedFile(const std::string& signFile, std::string& fileContent, const bool rawContent)
{
	const std::string content = readFile(signFile);
	
	//std::cout << "Will parse: " << std::endl << content << std::endl;
	
	const std::string beginStr("------ BEGIN BITCOIN SECP256K1 CONTENT SIGNATURE ------");
	const std::string contentBeginStr("=========================== Content ============================\n");
	
	const std::string endStr("------ END BITCOIN SECP256K1 CONTENT SIGNATURE ------");
	const std::string contentEndStr("=========================== End content ========================");
	
	const int startPos = content.find(beginStr);
	if(startPos == std::string::npos)
	{
		throw std::string("Can not find sigFile start");
	}

	const int numberStartPos = startPos + beginStr.size();

	const int numEndPos = content.find(contentBeginStr);
	if(numEndPos == std::string::npos)
	{
		throw std::string("Can not find sigFile end");
	}


	const std::string infoPart = content.substr(numberStartPos, numEndPos - numberStartPos);
	//std::cout << "The info part: " << std::endl << infoPart << std::endl;


	const int contentStartPos = numEndPos + contentBeginStr.size();
	
	
	const int contentEndPos = content.find(contentEndStr);
	if(contentEndPos == std::string::npos)
	{
		throw std::string("Can not find content end");
	}
	
	const std::string hexNewlineContent = content.substr(contentStartPos, contentEndPos - contentStartPos);

	//std::cout << "HexNewlineContent: " << hexNewlineContent << std::endl;

	if(rawContent)
	{
		fileContent = hexNewlineContent;	//!!!!!!!!!!!!!!!!!!
	}
	else
	{
		const std::string hexContent = removeNewLine(hexNewlineContent);

		//Decode the hex content
		fileContent = hexStrToBytes(hexContent);
	}


	const int endPos = content.find(endStr);
	if(endPos == std::string::npos)
	{
		throw std::string("Can not find sigFile end");
	}
	
	const int pubkeyInfoPos = getpubKeyInfoPos(infoPart);
	if(pubkeyInfoPos == -1)
	{
		throw std::string("Parse error, can not find pub key info");
	}
	
	
	const std::string pubInfo = infoPart.substr(pubkeyInfoPos, 4);
	//std::cout << "PUBINFO: " << pubInfo << std::endl;
	const std::string xInfo = pubInfo.substr(0, 1);
	const std::string yInfo = pubInfo.substr(1, 1);
	const std::string compressedInfo = pubInfo.substr(2, 1);
	
	
	const bool firstX = (xInfo == "f");
	const bool oddY = (yInfo == "o");
	const bool compressed = (compressedInfo == "c");
	
	
	//if(compressed)
	//{
	//	throw std::string("Compressed addresses not supported");
	//}
	
	const std::string numberPart = infoPart.substr(pubkeyInfoPos + 4);
	
	
	
	std::list<std::string> numbers;
	std::string thisNum;
	bool hasNum(false);
	for(int i = 0 ; i < numberPart.size() ; i++)
	{
		const char c = numberPart[i];
		if(!isHexDigit(c))
		{
			if(hasNum)
			{
				numbers.push_back(thisNum);
				thisNum = "";
				hasNum = false;
			}			
		}
		else
		{
			hasNum = true;
			thisNum += c;
		}
	}
	
	//std::cout << "Has the following strings: " << std::endl;
	for(std::list<std::string>::const_iterator it = numbers.begin() ;
		it != numbers.end() ;
		++it)
	{
		const std::string num = (*it);
		if(num.size() != 64)
		{
			throw std::string("Error, number should be 256 bits");
		}
		//std::cout << "NUM: " << num << std::endl;	
	}
	
	if(numbers.size() != 2)
	{
		throw std::string("Error, need 3 numbers");
	}
	
	std::list<std::string>::const_iterator jt = numbers.begin();
	const std::string rNum = (*jt++);
	const std::string sNum = (*jt++);
	
	const BigInt<1024> r(BigInt<1024>::fromHex(rNum));
	const BigInt<1024> s(BigInt<1024>::fromHex(sNum));

	//std::cout << "r: " << r.strHex() << std::endl;
	//std::cout << "s: " << s.strHex() << std::endl;

	const RSVal rs(r, s);

	const SigFileContent ret(firstX, oddY, compressed, rs);
	
	return ret;
}









SigFileContent parseSignFile(const std::string& signFile)
{
	const std::string content = readFile(signFile);
	
	//std::cout << "Will parse: " << std::endl << content << std::endl;
	
	const std::string beginStr("------ BEGIN BITCOIN SECP256K1 SIGNATURE ------");
	
	const std::string endStr("------ END BITCOIN SECP256K1 SIGNATURE ------");
	
	const int startPos = content.find(beginStr);
	if(startPos == std::string::npos)
	{
		throw std::string("Can not find sigFile start");
	}

	const int numberStartPos = startPos + beginStr.size();

	const int endPos = content.find(endStr);
	if(endPos == std::string::npos)
	{
		throw std::string("Can not find sigFile end");
	}

	const std::string infoPart = content.substr(numberStartPos, endPos -numberStartPos);
	//std::cout << "The info part: " << std::endl << infoPart << std::endl;

	
	const int pubkeyInfoPos = getpubKeyInfoPos(infoPart);
	if(pubkeyInfoPos == -1)
	{
		throw std::string("Parse error, can not find pub key info");
	}
	
	
	const std::string pubInfo = infoPart.substr(pubkeyInfoPos, 4);
	//std::cout << "PUBINFO: " << pubInfo << std::endl;
	const std::string xInfo = pubInfo.substr(0, 1);
	const std::string yInfo = pubInfo.substr(1, 1);
	const std::string compressedInfo = pubInfo.substr(2, 1);

	//std::cout << "xInfo: " << xInfo << std::endl;
	//std::cout << "yInfo: " << yInfo << std::endl;
	
	//numberPart.find("fXoY");
	//numberPart.find("sXeY");
	
	const bool firstX = (xInfo == "f");
	const bool oddY = (yInfo == "o");
	const bool compressed = (compressedInfo == "c");
	
	
	//if(compressed)
	//{
	//	throw std::string("Compressed addresses not supported");
	//}
	
	const std::string numberPart = infoPart.substr(pubkeyInfoPos + 4);
	
	
	
	std::list<std::string> numbers;
	std::string thisNum;
	bool hasNum(false);
	for(int i = 0 ; i < numberPart.size() ; i++)
	{
		const char c = numberPart[i];
		if(!isHexDigit(c))
		{
			if(hasNum)
			{
				numbers.push_back(thisNum);
				thisNum = "";
				hasNum = false;
			}			
		}
		else
		{
			hasNum = true;
			thisNum += c;
		}
	}
	
	//std::cout << "Has the following strings: " << std::endl;
	for(std::list<std::string>::const_iterator it = numbers.begin() ;
		it != numbers.end() ;
		++it)
	{
		const std::string num = (*it);
		if(num.size() != 64)
		{
			throw std::string("Error, number should be 256 bits");
		}
		//std::cout << "NUM: " << num << std::endl;	
	}
	
	if(numbers.size() != 2)
	{
		throw std::string("Error, need 3 numbers");
	}
	
	std::list<std::string>::const_iterator jt = numbers.begin();
	const std::string rNum = (*jt++);
	const std::string sNum = (*jt++);
	
	const BigInt<1024> r(BigInt<257>::fromHex(rNum));
	const BigInt<1024> s(BigInt<257>::fromHex(sNum));

	std::cout << "r: " << r.strHex() << std::endl;
	std::cout << "s: " << s.strHex() << std::endl;

	const RSVal rs(r, s);

	//BitcoinEllipticCurve ec;
	//const ECPoint rs = ec.fromCoordinates(r, s);

	const SigFileContent ret(firstX, oddY, compressed, rs);
	
	return ret;
}







std::string encodeRsRaw(const RSVal& rs, const IndexAndParity& iap, const bool compressed)
{
	//std::cout << (iap.indexZero ? "indexZero" : "indexOne") << std::endl;
	//std::cout << (iap.parityOdd ? "parityOdd" : "parityEven") << std::endl;
	//std::cout << (compressed ? "compressed" : "non-compressed") << std::endl;
	
	
	
	std::string raw;
	if(iap.indexZero && !iap.parityOdd && !compressed)
	{
		raw += 0x1B;
	}
	else if(iap.indexZero && iap.parityOdd && !compressed)
	{
		raw += 0x1C;
	}
	else if(!iap.indexZero && !iap.parityOdd && !compressed)
	{
		raw += 0x1D;
	}
	else if(!iap.indexZero && iap.parityOdd && !compressed)
	{
		raw += 0x1E;
	}
	else if(iap.indexZero && !iap.parityOdd && compressed)
	{
		raw += 0x1F;
	}
	else if(iap.indexZero && iap.parityOdd && compressed)
	{
		raw += 0x20;
	}
	else if(!iap.indexZero && !iap.parityOdd && compressed)
	{
		raw += 0x21;
	}
	else if(!iap.indexZero && iap.parityOdd && compressed)
	{
		raw += 0x22;
	}
	else
	{
		throw std::string("Internal error when encoding RS");
	}
	
	
	const BigInt<256> r(rs.r);
	const BigInt<256> s(rs.s);

	
	raw += r.getBinaryLong(); //getBinaryVal(true);
	raw += s.getBinaryLong(); //getBinaryVal(true);

	//std::cout << "Size before base64: " << raw.size() << std::endl;

	//const std::string ret = base64_encode(raw);
	
	
	return raw;
}



std::string encodeRs(const RSVal& rs, const IndexAndParity& iap, const bool compressed)
{
	const std::string raw = encodeRsRaw(rs, iap, compressed);
	const std::string ret = base64_encode(raw);
	return ret;
}




std::string encodeArmory(const IndexAndParity iap, const bool compressed, const RSVal& rs, const std::string& message)
{
	std::string unencoded;
	
	unencoded += encodeRsRaw(rs, iap, compressed);
	unencoded += message;

	return unencoded;
}






std::string addNewlines(const std::string encoded)
{
	std::string ret;
	
	//Add newlines after 65 characters
	for(int i = 0 ; i < encoded.size() ; i++)
	{
		const char c = encoded[i];
		ret += c;
		if((i) % 65 == 64)
		{
			ret += "\n";
		}
	}
	
	return ret;
}



std::string encodeArmoryB64(const std::string unencoded)
{
	const std::string encoded = base64_encode(unencoded);

	return addNewlines(encoded);

/*
	std::string ret;
	
	//Add newlines after 65 characters
	for(int i = 0 ; i < encoded.size() ; i++)
	{
		const char c = encoded[i];
		ret += c;
		if((i) % 65 == 64)
		{
			ret += "\n";
		}
	}
	
	return ret;
*/
}







#define CRC24_INIT 0xB704CEL
#define CRC24_POLY 0x1864CFBL

//typedef long crc24;


long crc_octets(const std::string& str)
{
	const unsigned char *octets = (const unsigned char*)str.c_str();
	int len = str.size();
      
	long crc = CRC24_INIT;
    int i;
    while (len--) 
    {
		crc ^= (*octets++) << 16;
        for (i = 0; i < 8; i++) 
        {
			crc <<= 1;
            if (crc & 0x1000000)
				crc ^= CRC24_POLY;
		}
	}
	
	return crc & 0xFFFFFFL;
}





std::string crc(const std::string& str)
{

	long crc = crc_octets(str);

	const BigInt<24> val(crc);
	//std::cout << "        Crc: " << val << std::endl;
	const BigInt<24> low = val.returnMaskHighBits(8);
	const BigInt<24> middle = (val >> 8).returnMaskHighBits(8);
	const BigInt<24> high = (val >> 16).returnMaskHighBits(8);

	const BigInt<24> changedOrder = (low << 16) + (middle << 8) + high;
	//std::cout << "Swapped Crc: " << changedOrder << std::endl;
	
	const std::string byteVal = changedOrder.getBinaryLong(); //getBinaryVal(true);
	
	//std::cout << "Byteval " << printBin(byteVal) << std::endl;
	//std::cout << "Byteval size: " << byteVal.size() << std::endl;
	std::string ret;
	ret += "=";
	ret += base64_encode(byteVal);

	return ret;
}





std::string generateArmoryClearFileFormat(const std::string& message, const RSVal& rs, 
			const IndexAndParity& iap, const bool compressed)
{
	const std::string sigBlockStart("-----BEGIN BITCOIN SIGNED MESSAGE-----");
	const std::string sigBlockMiddle("-----BEGIN BITCOIN SIGNATURE-----");	
	const std::string sigBlockEnd("-----END BITCOIN SIGNATURE-----");
	
	const std::string rawRs = encodeRsRaw(rs, iap, compressed);

	std::string ret;
	
	ret += sigBlockStart;
	ret += "\n";
	ret += "Comment: Signed with bitsig version ";
	ret += versionNumber;
	ret += "\n";	
	ret += "\n";
	ret += message; 
	ret += "\n";	
	ret += sigBlockMiddle;	
	ret += "\n";
	ret += "\n";
	ret += "\n";	
	ret += addNewlines(base64_encode(rawRs));
	ret += "\n";
	ret += crc(rawRs);
	ret += "\n";	
	ret += sigBlockEnd;
	ret += "\n";
	
	return ret;
}




std::string generateArmoryA46FileFormat(const std::string& message, const RSVal& rs, 
			const IndexAndParity iap, const bool compressed)
{
	std::string ret;
	const std::string sigBlockStart("-----BEGIN BITCOIN MESSAGE-----");
	const std::string sigBlockEnd("-----END BITCOIN MESSAGE-----");
		
	const std::string armoryEncoded = encodeArmory(iap, compressed, rs, message);
	
	ret += sigBlockStart;
	ret += "\n";
	ret += "Comment: Signed with bitsig version ";
	ret += versionNumber;
	ret += "\n";	
	ret += "\n";
	ret += encodeArmoryB64(armoryEncoded);
	ret += "\n";	
	ret += crc(armoryEncoded); //"=ABCD";		//Todo!!! CRC
	ret += "\n";	
	ret += sigBlockEnd;
	ret += "\n";
	
	return ret;
}



std::string generateArmoryFilFormat(const ECPoint& pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message)
{
	const BigInt<256> r(rs.r);
	const BigInt<256> s(rs.s);


	const BigInt<256> pubx(pubkey.x);
	const BigInt<256> puby(pubkey.y);


	std::string pubstr;
	pubstr += "04";
	pubstr += hexString(pubx.getBinaryLong()); //strHex();
	pubstr += hexString(puby.getBinaryLong()); //puby.strHex();

	std::string rsstr;
	rsstr += hexString(r.getBinaryLong()); //r.strHex();
	rsstr += hexString(s.getBinaryLong()); //s.strHex();



	std::string sigResult;
	sigResult += "-----BEGIN-SIGNATURE-BLOCK-------------------------------------\n";
	sigResult += "Address:    ";
	sigResult += bitcoinAddress.toString(); //.address;
	sigResult += "\n";
	
	sigResult += "Message:    \"";
	sigResult += splitArmoryStrLines(message);
	sigResult += "\"\n";

	sigResult += "PublicKey:  ";	
	sigResult += splitArmoryLines(pubstr);
	sigResult += "\n";
	
	sigResult += "Signature:  ";	
	sigResult += splitArmoryLines(rsstr);
	sigResult += "\n";
	
	sigResult += "-----END-SIGNATURE-BLOCK---------------------------------------\n";

	return sigResult;
}

















std::string generateInputsIoFileFormat(const ECPoint& not_used_pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message, const IndexAndParity& iap, const bool compressed)
{
	std::string sigResult;
	sigResult += "-----BEGIN BITCOIN SIGNED MESSAGE-----\n";
	sigResult += message;
	sigResult += "\n";	
	sigResult += "-----BEGIN SIGNATURE-----\n";
	sigResult += bitcoinAddress.toString(); //.address;	
	sigResult += "\n";
	
	sigResult += encodeRs(rs, iap, compressed);
	sigResult += "\n";
	
	sigResult += "-----END BITCOIN SIGNED MESSAGE-----\n";

	return sigResult;
}




std::string generateMultibitFileFormat(const ECPoint& not_used_pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message, const IndexAndParity& iap, const bool compressed)
{
	std::string sigResult;
	sigResult += "-----BEGIN BITCOIN SIGNED MESSAGE-----\n";
	sigResult += message;
	sigResult += "\n";	
	sigResult += "-----BEGIN BITCOIN SIGNATURE-----\n";
	sigResult += "Version: Bitgen ";
	sigResult += versionNumber;	
	//sigResult += "Version: Bitcoin-qt (1.0)";
	sigResult += "\n";
	
	sigResult += "Address: ";
	sigResult += bitcoinAddress.toString(); //.address;
	sigResult += "\n";
	sigResult += "\n";
		
	//sigResult += "Signature:  ";	
	sigResult += encodeRs(rs, iap, compressed);
	sigResult += "\n";
	
	sigResult += "-----END BITCOIN SIGNATURE-----\n";

	return sigResult;
}






std::string removeExtra(const std::string& str)
{
	std::string ret;
	for(int i = 0 ; i < str.size() ; i++)
	{
		const char c = str[i];
		if(c == ' ' || c == '\r' || c == '\n')
		{
			continue;
		}
		
		ret += c;
	}	
	
	return ret;
	
}





InputsIoData parseMultibit(const std::string& str)
{
	const std::string sigBlockStart("-----BEGIN BITCOIN SIGNED MESSAGE-----");
	const std::string signatureStart("-----BEGIN BITCOIN SIGNATURE-----");	
	const std::string sigBlockEnd("-----END BITCOIN SIGNATURE-----");
	
	const int blockStartPos = str.find(sigBlockStart);
	if(blockStartPos == std::string::npos)
	{
		throw std::string("InputsIo block start not found");
	}
	

	const int sigStartPos = str.find(signatureStart);
	if(sigStartPos == std::string::npos)
	{
		throw std::string("InputsIo signature start not found");
	}


	const int sigEndPos = str.find(sigBlockEnd);
	if(sigEndPos == std::string::npos)
	{
		throw std::string("Multibit signature end not found");
	}

	const int messageStartPos = blockStartPos + sigBlockStart.size();
	const int messageLen = sigStartPos - messageStartPos;

	const std::string messageWithPadding = str.substr(messageStartPos, messageLen);
	//std::cout << std::endl;
	//std::cout << "The Message WithPadding: " << messageWithPadding << std::endl << std::endl;
	const int firstMsgNewlinePos = messageWithPadding.find("\n");
	if(firstMsgNewlinePos == std::string::npos)
	{
		throw std::string("InputsIo first message newline not found");
	}

	const int lastMsgNewlinePos = messageWithPadding.rfind("\n");
	if(lastMsgNewlinePos == std::string::npos)
	{
		throw std::string("InputsIo last message newline not found");
	}
	
	const int messStartPos = firstMsgNewlinePos + 1;
	const int messSize = lastMsgNewlinePos - messStartPos;
	
	const std::string message = messageWithPadding.substr(messStartPos, messSize);
	//std::cout << "Only message: " << message << std::endl;
	
	const int sigBlockStartPos = sigStartPos + signatureStart.size();
	const int sigBlockLen = sigEndPos - sigBlockStartPos;
	const std::string sigBlock = str.substr(sigBlockStartPos, sigBlockLen);
	//std::cout << "The sig block: " << sigBlock << std::endl;
	
	
	
	
	
	
	
	
	//Get the bitcoin address in the first row
	const std::string theAddressStr("Address: ");
	const int addressPos = sigBlock.find(theAddressStr);
	if(addressPos == std::string::npos)
	{
		throw std::string("Multibit address not found");
	}

	
	const int addressEndPos = sigBlock.find("\n", addressPos + 1);
	if(addressEndPos == std::string::npos)
	{
		throw std::string("InputsIo address end not found");
	}
	
	
	std::cout << std::endl;
	
	const int addressStartPos = addressPos + theAddressStr.size();
	const int addressLen = addressEndPos - addressStartPos;
	
	const std::string addressWithPadding = sigBlock.substr(addressStartPos, addressLen);
	//std::cout << "Address with padding: " << addressWithPadding << std::endl;
	
	const std::string address = removeExtra(addressWithPadding);
	//std::cout << "Address: " << address << std::endl;
	

	const std::string signatureWithPadding = sigBlock.substr(addressEndPos);
	//std::cout << "Signature with padding: " << signatureWithPadding << std::endl;
	const std::string signature = removeExtra(signatureWithPadding);
	//std::cout << "Signature: " << signature << std::endl;

	
	
	InputsIoData ioData(message, address, signature);
	return ioData;
}







std::string parseArmoryClear(const std::string& str, std::string& message)
{
	const std::string sigBlockStart("-----BEGIN BITCOIN SIGNED MESSAGE-----");
	const std::string sigBlockMiddle("\n-----BEGIN BITCOIN SIGNATURE-----");
	const std::string sigBlockEnd("-----END BITCOIN SIGNATURE-----");
	
	const int startPos = str.find(sigBlockStart);
	if(startPos == std::string::npos)
	{
		throw std::string("Can not find message start");
	}

	const int middlePos = str.rfind(sigBlockMiddle);
	if(middlePos == std::string::npos)
	{
		throw std::string("Can not find message middle");
	}


	const int endPos = str.rfind(sigBlockEnd);
	if(startPos == std::string::npos)
	{
		throw std::string("Can not find message end");
	}


	const int firstPos = startPos + sigBlockStart.size();
	const std::string first = str.substr(firstPos, middlePos - firstPos);
	//std::cout << "FIRST: " << first << std::endl;
	
	//Find the first empty line
	const std::string emptyLine("\n\n");
	const int emptyLinePos = first.find(emptyLine);
	if(emptyLinePos == std::string::npos)
	{
		throw std::string("Can not find empty line");
	}
	
	const int messageStartPos = emptyLinePos + emptyLine.size();
	
	//const std::string 
	message = first.substr(messageStartPos);
	//std::cout << "The message: \"" << message << "\"" << std::endl;

	const int lastStart = middlePos + sigBlockMiddle.size();

	const std::string last = str.substr(lastStart, endPos - lastStart);
	
	//std::cout << "Last: " << last << std::endl;
	
	std::string totalB64sig;
	
	int signatureStartPos = 0;
	
	for(;;)
	{
		const int newlinePos = last.find("\n", signatureStartPos);
		//std::cout << "newlinePos: " << newlinePos << std::endl;
		if(newlinePos == std::string::npos)
		{
			throw std::string("Can not find newline");
		}
		
		const std::string line = last.substr(signatureStartPos, newlinePos - signatureStartPos);
		
		//Is there a "=" followed by pther characters?
		//In that case, it is the CRC which shoul dnot be included
		if(line.size() > 2)
		{
			if(line[0] == '=' && line[1] != '\n')
			{
				break;
			}
		}
		
		totalB64sig += line;
		//std::cout << "THE LINE: " << line << std::endl;
		signatureStartPos = newlinePos + 1;
	}
	
	//std::cout << "Total b64 sig: " << totalB64sig << std::endl;
	
	return totalB64sig;
}





std::string parseArmoryBase64(const std::string& str)
{
	const std::string sigBlockStart("-----BEGIN BITCOIN MESSAGE-----");

	const std::string sigBlockEnd("-----END BITCOIN MESSAGE-----");


	const int startBlockPos = str.find(sigBlockStart);
	if(startBlockPos == std::string::npos)
	{
		throw std::string("Can not find message start");
	}
	
	const int contentStart = startBlockPos + sigBlockStart.size();
	
	
	const int endBlockPos = str.rfind(sigBlockEnd);
	if(endBlockPos == std::string::npos)
	{
		throw std::string("Can not find message end");
	}
	
	const std::string messPart = str.substr(contentStart, endBlockPos - contentStart);
	//std::cout << "Message part: " << messPart << std::endl;
	
	const std::string emptyLine("\n\n");
	
	//Find the empty line
	const int emptyLinePos = messPart.find(emptyLine);
	if(emptyLinePos == std::string::npos)
	{
		throw std::string("Can not find the empty line");
	}

	int signatureStartPos = emptyLinePos + emptyLine.size();
	
	std::string totalB64sig;
	
	for(;;)
	{
		const int newlinePos = messPart.find("\n", signatureStartPos + 2);
		//std::cout << "newlinePos: " << newlinePos << std::endl;
		if(newlinePos == std::string::npos)
		{
			throw std::string("Can not find newline");
		}
		
		const std::string line = messPart.substr(signatureStartPos, newlinePos - signatureStartPos);
		
		//Is there a "=" followed by pther characters?
		//In that case, it is the CRC which shoul dnot be included
		if(line.size() > 2)
		{
			if(line[0] == '=' && line[1] != '\n')
			{
				break;
			}
		}
		
		totalB64sig += line;
		//std::cout << "THE LINE: " << line << std::endl;
		signatureStartPos = newlinePos + 1;
	}

	
	//const InputsIoData data("mess", "addr", totalB64sig);
	
	return totalB64sig;
}




InputsIoData parseInputsIo(const std::string& str)
{
	const std::string sigBlockStart("-----BEGIN BITCOIN SIGNED MESSAGE-----");
	const std::string signatureStart("-----BEGIN SIGNATURE-----");	
	const std::string sigBlockEnd("-----END BITCOIN SIGNED MESSAGE-----");
	
	const int blockStartPos = str.find(sigBlockStart);
	if(blockStartPos == std::string::npos)
	{
		throw std::string("InputsIo block start not found");
	}
	

	const int sigStartPos = str.find(signatureStart);
	if(sigStartPos == std::string::npos)
	{
		throw std::string("InputsIo signature start not found");
	}


	const int sigEndPos = str.find(sigBlockEnd);
	if(sigEndPos == std::string::npos)
	{
		throw std::string("InputsIo signature end not found");
	}

	const int messageStartPos = blockStartPos + sigBlockStart.size();
	const int messageLen = sigStartPos - messageStartPos;

	const std::string messageWithPadding = str.substr(messageStartPos, messageLen);
	//std::cout << "The Message WithPadding: " << messageWithPadding << std::endl;
	const int firstMsgNewlinePos = messageWithPadding.find("\n");
	if(firstMsgNewlinePos == std::string::npos)
	{
		throw std::string("InputsIo first message newline not found");
	}

	const int lastMsgNewlinePos = messageWithPadding.rfind("\n");
	if(lastMsgNewlinePos == std::string::npos)
	{
		throw std::string("InputsIo last message newline not found");
	}
	
	const int messStartPos = firstMsgNewlinePos + 1;
	const int messSize = lastMsgNewlinePos - messStartPos;
	
	const std::string message = messageWithPadding.substr(messStartPos, messSize);
	//std::cout << "Only message: " << message << std::endl;
	
	const int sigBlockStartPos = sigStartPos + signatureStart.size();
	const int sigBlockLen = sigEndPos - sigBlockStartPos;
	const std::string sigBlock = str.substr(sigBlockStartPos, sigBlockLen);
	//std::cout << "The sig block: " << sigBlock << std::endl;
	
	//Get the bitcoin address in the first row
	const int firstNewlinePos = sigBlock.find("\n");
	if(firstNewlinePos == std::string::npos)
	{
		throw std::string("InputsIo first newline not found");
	}

	
	const int addressEndPos = sigBlock.find("\n", firstNewlinePos + 1);
	if(addressEndPos == std::string::npos)
	{
		throw std::string("InputsIo address end not found");
	}
	
	const std::string addressWithPadding = sigBlock.substr(0, addressEndPos);
	//std::cout << "Address with padding: " << addressWithPadding << std::endl;
	
	const std::string address = removeExtra(addressWithPadding);
	//std::cout << "Address: " << address << std::endl;
	

	const std::string signatureWithPadding = sigBlock.substr(addressEndPos);
	//std::cout << "Signature with padding: " << signatureWithPadding << std::endl;
	const std::string signature = removeExtra(signatureWithPadding);
	//std::cout << "Signature: " << signature << std::endl;

	
	
	InputsIoData ioData(message, address, signature);
	return ioData;
}





