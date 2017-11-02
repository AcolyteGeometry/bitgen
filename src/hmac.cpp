/**
 * hmac.cpp - Bitcoin poscript generator
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


#include "hmac.h"

#include "hash.h"

#include "bigint.h"



std::string calcHmacSha512(const std::string& Kin, const std::string& m)
{	
	std::string K(Kin);
	
	if(K.size() > 128)
	{
		K = sha512(K);
		//std::cout << "K SIZE IS NOW: " << K.size() << std::endl;
	}	

	while(K.size() < 128)
	{
		K += '\0';
	}
	
	//std::cout << "Size of K: " << K.size() << std::endl;
	
	BigInt<1024> Kprim(K, 'c');
	
	BigInt<1024> opad("5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c");
	BigInt<1024> ipad("3636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636");

	BigInt<1024> KprimXorOpad(Kprim ^ opad);
	BigInt<1024> KprimXorIpad(Kprim ^ ipad);

	const std::string XorO = KprimXorOpad.getBinaryLong();
	const std::string XorI = KprimXorIpad.getBinaryLong();
	
	const std::string XorI_M = XorI + m;
	const std::string HashXorI_M = sha512(XorI_M);
	const std::string XorO_HashXorI_M = XorO + HashXorI_M;
	const std::string result = sha512(XorO_HashXorI_M);


	//std::cout << "KPrim     : " << hexString(Kprim.getBinaryLong()) << std::endl;	
	//std::cout << "Opad      : " << opad << std::endl;
	//std::cout << "Ipad      : " << ipad << std::endl;

	//std::cout << std::endl;
	//std::cout << "XorO      : " << hexString(XorO) << std::endl;
	//std::cout << "XorI      : " << hexString(XorI) << std::endl;
	//std::cout << std::endl;

	//std::cout << "Size KxIraw : " << XorI.size() << std::endl;
	//std::cout << "Size KxOraw : " << XorO.size() << std::endl;
	
	//std::cout << "XorI_M    : " << hexString(XorI_M) << std::endl;
	//std::cout << "Siz XorI_M: " << XorI_M.size() << std::endl;
	//std::cout << "HashKxICM : " << hexString(HashXorI_M) << std::endl;
	//std::cout << "TotConcat : " << hexString(XorO_HashXorI_M) << std::endl;
	//std::cout << "Result    : " << hexString(result) << std::endl;
	
	return result;
}






std::string xorStrings(const std::string& a, const std::string& b)
{
	if(a.size() != b.size())
	{
		throw std::string("Error, strings for xor not equal length");
	}
	
	std::string ret;
	for(int i = 0 ; i < a.size(); i++)
	{
		const unsigned char achar = a[i];
		const unsigned char bchar = b[i];
		const unsigned char res = achar ^ bchar;
		ret += res;
	}
	
	return ret;
}



int ceilDivide(const int x, const int y)
{	
	 return (x + y - 1) / y;	 
}


std::string PbKdb2HmacSha512(const std::string& data, const std::string& salt, const int iterations, const int keylen)
{
	//std::cout << "Data: " << data << std::endl;
	//std::cout << "Salt: " << salt << std::endl;
	//std::cout << "Iter: " << iterations << std::endl;
	//std::cout << "KeyL: " << keylen << std::endl;


	const int numIter = ceilDivide(keylen, 64); //keylen / 64;
	//std::cout << "Num iterations: " << numIter << std::endl;

	std::string buf;
	//Go through all blocks
	for(int i = 0 ; i < numIter ; i++)	
	{
		//std::cout << "Iterating" << std::endl;
		const int block = i + 1;
		BigInt<32> bigInt(block);
		const std::string intString = bigInt.getBinaryLong();
		//std::cout << "The intString: " << hexString(intString) << std::endl;
		const std::string saltAndInt = salt + intString;

		//std::cout << "data: " << hexString(data) << std::endl;		
		//std::cout << "data size: " << data.size() << std::endl;		
		
		//std::cout << "SaltAndInt: " << hexString(saltAndInt) << std::endl;
		//std::cout << "SaltAndInt size: " << saltAndInt.size() << std::endl;		
		
        std::string rv = calcHmacSha512(data, saltAndInt);
        //std::cout << "!!!!RV before inner loop: " << hexString(rv) << std::endl;
        //std::cout << std::endl;
        //std::cout << std::endl;
        
		std::string u(rv);
        
        //std::cout << "RV: " << hexString(rv) << std::endl;
		const int loopIterations = iterations - 1;
		for(int j = 0 ; j < loopIterations ; j++)
		{
			//std::cout << "INNER LOOP" << std::endl;
			//const std::string thePass(u);
			
			//std::cout << "========thePass: " << hexString(u) << std::endl;
            //const std::string 
            u = calcHmacSha512(data, u);
            //std::cout << "u: " << hexString(u) << std::endl;
            rv = xorStrings(rv, u);
            //std::cout << "Updated rv: " << hexString(rv) << std::endl;

		}

		buf += rv;
		//std::cout << "Buf: " << hexString(buf) << std::endl;
	}

    const std::string finalResult = buf.substr(0, keylen);


	return finalResult;
}







std::string hmacSha256(const std::string& key, const std::string& message)
{
	if(key.size() != 32)
	{
		throw std::string("Key length error, must be 32");
	}

	const unsigned char op(0x5c);
	const unsigned char ip(0x36);

	
	std::string oPad;
	std::string iPad;
	for(int i = 0 ; i < 32 ; i++)
	{		
		const unsigned char c = key[i];
		oPad += (op ^ c);
		iPad += (ip ^ c);		
	}
	
	for(int i = 0 ; i < 32 ; i++)
	{		
		oPad += op;
		iPad += ip;		
	}
	
	const std::string inner = sha256(iPad + message);
	const std::string hmac = sha256(oPad + inner);
	return hmac;
}
