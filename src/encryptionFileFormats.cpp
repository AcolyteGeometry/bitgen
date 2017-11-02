/**
 * encryptionFileFormats.cpp
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
 
/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
*/

#include "encryptionFileFormats.h"
#include "pubKey.h"

#include "util.h"


std::string createXorEncryptedStr(const PubKey& pubKey, const ECPoint& R, const BigInt<256>& encrypted,
	int encryptedSize)
{	
	std::string ret;
	ret += "------ BEGIN BITCOIN SECP256K1 XOR ENCRYPT ------\n";	
	ret += pubKey.toString();
	ret += "\n";
	ret += hexString(R.getRaw(true)); //getCompressedHex();
	ret += "\n";
	ret += hexString(encrypted.getBinaryLong()) /*strHex()*/.substr(0, 2 * encryptedSize);
	ret += "\n";	
	ret += "------ END BITCOIN SECP256K1 XOR ENCRYPT ------\n";
	
	return ret;
}




std::string createFileEncryptedStr(const PubKey& pubKey, const ECPoint& R, const std::string& encrypted,
	int encryptedSize)
{	
	
	const std::string hexEncoded = hexString(encrypted);
	
	std::string ret;
	ret += "------ BEGIN BITCOIN SECP256K1 FILE ENCRYPT ------\n";
	ret += pubKey.toString();
	ret += "\n";
	ret += hexString(R.getRaw(true)); //getCompressedHex();
	ret += "\n";
	
	for(int i = 0 ; i < hexEncoded.size() ; i++)
	{
		if((i != 0) && i % 64 == 0)
		{
			ret += "\n";
		}
		
		const unsigned char c = hexEncoded[i];
		ret += c;
		
	}
	
	ret += "\n";	
	ret += "------ END BITCOIN SECP256K1 FILE ENCRYPT ------\n";
	
	return ret;
}



//EncryptedXorData
EncryptedFileData parseXorEncryptedStr(const std::string& str)
{
	
	const std::string encryptBlockStart("------ BEGIN BITCOIN SECP256K1 XOR ENCRYPT ------\n");
	const std::string encryptBlockEnd("------ END BITCOIN SECP256K1 XOR ENCRYPT ------\n");
	
	const int blockStartPos = str.find(encryptBlockStart);
	if(blockStartPos == std::string::npos)
	{
		throw std::string("block start not found");
	}
	
	
	const int blockEndPos = blockStartPos + encryptBlockStart.size();
	
	//Find the next newline
	const int firstEndlinePos = str.find("\n", blockEndPos);
	if(firstEndlinePos == std::string::npos)
	{
		throw std::string("newline 1 not found");
	}
	
	const std::string addressStr = str.substr(blockEndPos, firstEndlinePos - blockEndPos);
	//std::cout << "X: " << xStr << std::endl;
	
	
	const int secondLineStart = firstEndlinePos + 1;
		
	const int secondEndlinePos = str.find("\n", secondLineStart);
	if(secondEndlinePos == std::string::npos)
	{
		throw std::string("newline 2 not found");
	}
	
	const std::string xyStr = str.substr(secondLineStart, secondEndlinePos - secondLineStart);
	//std::cout << "Y: " << yStr << std::endl;


	const int thirdLineStart = secondEndlinePos + 1;
		
	const int thirdEndlinePos = str.find("\n", thirdLineStart);
	if(thirdEndlinePos == std::string::npos)
	{
		throw std::string("newline 3 not found");
	}
	
	const std::string cStr = str.substr(thirdLineStart, thirdEndlinePos - thirdLineStart);
	//std::cout << "C: " << cStr << std::endl;

	const int fourtLineStart = thirdEndlinePos + 1;


/*

	const int fourtEndlinePos = str.find("\n", fourtLineStart);
	if(fourtEndlinePos == std::string::npos)
	{
		throw std::string("newline 4 not found");
	}
	
	const std::string addressStr = str.substr(fourtLineStart, fourtEndlinePos - fourtLineStart);
	//std::cout << "address: " << addressStr << std::endl;

	const int fifthLineStart = fourtEndlinePos + 1;

*/





	const int endPos = str.find(encryptBlockEnd, fourtLineStart);
	if(endPos == std::string::npos)
	{
		throw std::string("Could not find end");
	}
	
	
	const std::string parityStr = xyStr.substr(0, 2);
	const std::string xStr = xyStr.substr(2);
	
	
	if(parityStr != "02" && parityStr != "03")
	{
		std::cout << "Parity string: " << parityStr << std::endl;
		throw std::string("Incorrect parity string");
	}
	
	const bool parityOdd = (parityStr == "03");
	
	const BigInt<1024> x(BigInt<1024>::fromHex(xStr)); //xParse.c_str());
	
	BitcoinEllipticCurve ec;
	const ECPoint R = ec.recoverPointFromXAndParityOdd(x, parityOdd);


	const int cryptSize = cStr.size() / 2;

	//Add zeroes if size less than 64	
	//std::string cryptedWithZeroes(cStr);
	//while(cryptedWithZeroes.size() < 64)
	//{
	//	cryptedWithZeroes += "0";
	//}

	
	const std::string c = hexStrToBytes(cStr); //cryptedWithZeroes);
	const std::string toStore = c.substr(0, cryptSize);
	//const BigInt<256> c(cParse.c_str());
	
	const Pkif pkif(addressStr);
		
	EncryptedFileData encData(pkif, R, c);
	return encData;
}




EncryptedFileData parseFileEncryptedStr(const std::string& str)
{
	
	const std::string encryptBlockStart("------ BEGIN BITCOIN SECP256K1 FILE ENCRYPT ------\n");
	const std::string encryptBlockEnd("------ END BITCOIN SECP256K1 FILE ENCRYPT ------\n");
	
	const int blockStartPos = str.find(encryptBlockStart);
	if(blockStartPos == std::string::npos)
	{
		throw std::string("block start not found");
	}
	
	
	const int blockEndPos = blockStartPos + encryptBlockStart.size();
	
	//Find the next newline
	const int firstEndlinePos = str.find("\n", blockEndPos);
	if(firstEndlinePos == std::string::npos)
	{
		throw std::string("newline 1 not found");
	}
	
	const std::string pkifStr = str.substr(blockEndPos, firstEndlinePos - blockEndPos);
	//std::cout << "X: " << xStr << std::endl;
	
	
	const int secondLineStart = firstEndlinePos + 1;
		
	const int secondEndlinePos = str.find("\n", secondLineStart);
	if(secondEndlinePos == std::string::npos)
	{
		throw std::string("newline 2 not found");
	}
	
	const std::string xyStr = str.substr(secondLineStart, secondEndlinePos - secondLineStart);
	//std::cout << "Y: " << yStr << std::endl;


	const int thirdLineStart = secondEndlinePos + 1;






		
	//const int thirdEndlinePos = str.find("\n", thirdLineStart);
	//if(thirdEndlinePos == std::string::npos)
	//{
	//	throw std::string("newline 3 not found");
	//}
	
	//const std::string cStr = str.substr(thirdLineStart, thirdEndlinePos - thirdLineStart);
	//std::cout << "C: " << cStr << std::endl;

	//const int fourtLineStart = thirdEndlinePos + 1;

	const int endPos = str.find(encryptBlockEnd); //, fourtLineStart);
	if(endPos == std::string::npos)
	{
		throw std::string("Could not find end");
	}
	
	const std::string withNewline = str.substr(thirdLineStart, endPos - thirdLineStart);
	
	//std::cout << "CSTR: " << std::endl << withNewline << std::endl;
	
	const std::string parityStr = xyStr.substr(0, 2);
	const std::string xStr = xyStr.substr(2);
	
	
	if(parityStr != "02" && parityStr != "03")
	{
		std::cout << "Parity string: " << parityStr << std::endl;
		throw std::string("Incorrect parity string");
	}
	
	const bool parityOdd = (parityStr == "03");
	
	const BigInt<1024> x(BigInt<1024>::fromHex(xStr)); //xParse.c_str());
	
	BitcoinEllipticCurve ec;
	const ECPoint R = ec.recoverPointFromXAndParityOdd(x, parityOdd);


	std::string cStr;
	for(int i = 0 ; i < withNewline.size() ; i++)
	{
		const unsigned char c = withNewline[i];
		if(isHexDigit(c))
		{
			cStr += c;
		}
	}
	


	if(cStr.size() % 2 != 0)
	{
		throw std::string("Must have even number of hex digits");
	}
	
	const int cryptSize = cStr.size() / 2;

	//Convery the hex coded string to raw bytes
	
	std::string raw = hexStrToBytes(cStr);
	

	
	//const PubAddress bitcoinAddress(addressStr);
	const Pkif pkif(pkifStr);
	
	EncryptedFileData encData(pkif, R, raw);
	return encData;
}


