/**
 * encrypt.cpp
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

#include "encrypt.h"


#include "aes.h"
#include <string.h>
#include <iostream>


std::string encryptAes128(const std::string& key, const std::string& ivStr, const std::string& origPlain)
{
	if(ivStr.size() != 16)
	{
		throw std::string("Incorrect iv length");
	}

	if(key.size() != 16)
	{
		throw std::string("Incorrect key length");
	}
	
	const int plainSize = origPlain.size();
	const int lastSize = plainSize % 16;
	
	const int extraSize = (lastSize == 16) ? 16 : 16 - lastSize;
	
	
	
	std::string plain(origPlain);
	
	const unsigned char extraChar(extraSize);	
	for(int i = 0 ; i < extraSize ; i++)
	{
		plain += extraChar;
	}
	
	//std::cout << "Padded plain size: " << plain.size() << std::endl;
	
	
	aes_context ctx;
	unsigned char iv[16];
	memcpy(iv, ivStr.c_str(), 16);
	
	
	unsigned char two_blocks[32];
	unsigned char* magic;
	
	aes_setkey_enc(&ctx, (unsigned char*)key.c_str(), 128);
	
	std::string encrypted;
	
	for(int i = 0 ; i < plain.size() ; i += 16)
	{		
		unsigned char decbuf[16];
		
		aes_crypt_cbc(&ctx, AES_ENCRYPT, 16, iv, (unsigned char*)&plain[i], decbuf);
		const std::string d((const char*)decbuf, 16);
		
		encrypted += d;
		
	}
	
	return encrypted;
}




std::string encryptAes256(const std::string& key, const std::string& ivStr, const std::string& origPlain)
{
	if(ivStr.size() != 32)
	{
		throw std::string("Incorrect iv length");
	}

	if(key.size() != 32)
	{
		throw std::string("Incorrect key length");
	}
	
	const int plainSize = origPlain.size();
	const int lastSize = plainSize % 32;
	
	const int extraSize = (lastSize == 32) ? 32 : 32 - lastSize;
	
	
	
	std::string plain(origPlain);
	
	const unsigned char extraChar(extraSize);	
	for(int i = 0 ; i < extraSize ; i++)
	{
		plain += extraChar;
	}
	
	//std::cout << "Padded plain size: " << plain.size() << std::endl;
	
	
	aes_context ctx;
	unsigned char iv[32];
	memcpy(iv, ivStr.c_str(), 32);
	
	
	unsigned char two_blocks[64];
	unsigned char* magic;
	
	aes_setkey_enc(&ctx, (unsigned char*)key.c_str(), 256);
	
	std::string encrypted;
	
	for(int i = 0 ; i < plain.size() ; i += 32)
	{		
		unsigned char decbuf[32];
		
		aes_crypt_cbc(&ctx, AES_ENCRYPT, 32, iv, (unsigned char*)&plain[i], decbuf);
		const std::string d((const char*)decbuf, 32);
		
		encrypted += d;
		
	}
	
	return encrypted;
}








std::string decryptAes128(const std::string& key, const std::string& ivStr, const std::string& enc)
{
	//std::cout << "Decrypting aes" << std::endl;
	if(ivStr.size() != 16)
	{
		throw std::string("Incorrect iv length");
	}

	if(key.size() != 16)
	{
		throw std::string("Incorrect key length");
	}
	
	if(enc.size() % 16 != 0)
	{
		throw std::string("Incorrect encrypt size");
	}
	
	//std::cout << "KEY: " << printBin(key) << std::endl;
	//std::cout << "IV : " << printBin(ivStr) << std::endl;
	
//	unsigned char decrypted[1024];
	
	
	aes_context ctx;
	unsigned char iv[16];
	memcpy(iv, ivStr.c_str(), 16);
	
	
	unsigned char two_blocks[32];
	unsigned char* magic;
	
	aes_setkey_dec(&ctx, (unsigned char*)key.c_str(), 128);
	
	std::string decrypted;
	
	for(int i = 0 ; i < enc.size() ; i += 16)
	{
		//std::cout << "Iterating aes" << std::endl;
		//aes_crypt_cbc(&ctx, AES_DECRYPT, 16, iv, (unsigned char*)&enc[i], (unsigned char*)(&decrypted[i]));
		
		unsigned char decbuf[16];
		
		aes_crypt_cbc(&ctx, AES_DECRYPT, 16, iv, (unsigned char*)&enc[i], decbuf);
		const std::string d((const char*)decbuf, 16);
		
		decrypted += d;
		
	}
	
	//Remove the padding at the end
	const unsigned char c = decrypted[enc.size() - 1];
	const int numPadding = c;
	for(int i = 0 ; i < numPadding ; i++)
	{
		const unsigned char thisPadding = decrypted[enc.size() - i - 1];
		if(thisPadding != c)
		{
			throw std::string("Padding error");
		}
	}
	
	std::string ret = decrypted.substr(0, enc.size() - numPadding);
	//std::string ret((const char*)decrypted, enc.size() - numPadding);
	return ret;
}







std::string decryptAes256(const std::string& key, const std::string& ivStr, const std::string& enc)
{
	//std::cout << "Decrypting aes" << std::endl;
	if(ivStr.size() != 32)
	{
		throw std::string("Incorrect iv length");
	}

	if(key.size() != 32)
	{
		throw std::string("Incorrect key length");
	}
	
	if(enc.size() % 32 != 0)
	{
		throw std::string("Incorrect encrypt size");
	}
	
	//std::cout << "KEY: " << printBin(key) << std::endl;
	//std::cout << "IV : " << printBin(ivStr) << std::endl;
	
//	unsigned char decrypted[1024];
	
	
	aes_context ctx;
	unsigned char iv[32];
	memcpy(iv, ivStr.c_str(), 32);
	
	
	unsigned char two_blocks[64];
	unsigned char* magic;
	
	aes_setkey_dec(&ctx, (unsigned char*)key.c_str(), 256);
	
	std::string decrypted;
	
	for(int i = 0 ; i < enc.size() ; i += 32)
	{
		//std::cout << "Iterating aes" << std::endl;
		//aes_crypt_cbc(&ctx, AES_DECRYPT, 16, iv, (unsigned char*)&enc[i], (unsigned char*)(&decrypted[i]));
		
		unsigned char decbuf[32];
		
		aes_crypt_cbc(&ctx, AES_DECRYPT, 32, iv, (unsigned char*)&enc[i], decbuf);
		const std::string d((const char*)decbuf, 32);
		
		decrypted += d;
		
	}
	
	//Remove the padding at the end
	const unsigned char c = decrypted[enc.size() - 1];
	const int numPadding = c;
	//std::cout << "Num padding: " << numPadding << std::endl;
	
	for(int i = 0 ; i < numPadding ; i++)
	{
		const unsigned char thisPadding = decrypted[enc.size() - i - 1];
		if(thisPadding != c)
		{
			throw std::string("Padding error");
		}
	}
	
	std::string ret = decrypted.substr(0, enc.size() - numPadding);
	//std::string ret((const char*)decrypted, enc.size() - numPadding);
	return ret;
}
