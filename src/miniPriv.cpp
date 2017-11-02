/**
 * miniPriv.cpp - Bitcoin address generator
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

#include "miniPriv.h"

#include <iostream>
#include <string>

#include "randomUtils.h"
#include "base58.h"
#include "privKey.h"


bool miniPrivChecksumOK(const std::string& miniAddr)
{
	const std::string miniAddrWithQuestionMark(miniAddr + '?');
	const std::string questionHash = sha256(miniAddrWithQuestionMark);
		
	const BigInt<256> hashNum(questionHash, 'c');
	const BigInt<256> shiftedVal = hashNum >> 248;
		
	const BigInt<256> zero;
	if(shiftedVal != zero)
	{
		return false;
	}
	
	return true;
}




bool isValidMiniPriv(const std::string miniAddr)
{
	//Does the address start with "S"??
	if(miniAddr.empty())
	{
		throw std::string("Mini private key format error");
	}

	if(miniAddr[0] != 'S')
	{
		throw std::string("Error, mini private key does not start with S");			
	}

	//There should be exactly 30 characters
	if(miniAddr.size() != 30 && miniAddr.size() != 22)
	{
		std::cout << "Size is: " << miniAddr.size() << std::endl;
		throw std::string("Error, mini private key must be 30 characters");
	}

	if(miniAddr.size() == 22)
	{
		std::cout << "Warning, the key length is only 22 characters. This is unsafe" << std::endl;
	}


	//Are there only base58 characters?
	for(int i = 0 ; i < miniAddr.size() ; i++)
	{
		const char c = miniAddr[i];
		if(!isBase58Char(c))
		{
			std::cout << "Error, the char: " << c << " is not permitted in base58" << std::endl;
			throw std::string("Error, non base58 char in mime private key");
		}
	}
		
	if(!miniPrivChecksumOK(miniAddr))
	{
		throw std::string("Incorrect checksum for mini private key");
	}
		
	return true;
}





BitcoinKeyPair convertMiniPriv(const std::string& miniAddr, const bool compressed)
{		
	const std::string hash = sha256(miniAddr);

	const BigInt<256> privKey(hash, 'c');
	//std::cout << "Private key: " << privKey << std::endl;
	
	const PrivKey myPrivKey(privKey, compressed);
	
	//std::cout << "=============Will generate keypair for mini private key" << std::endl;	
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);
	//std::cout << "=============Ready generate keypair for mini private key" << std::endl;	
	
	return keypair;
}






void miniKeyGenerate(int argc, char* argv[], const std::string& thetime)
{
	
	const bool compressed(false);
		std::cout << "Generating mini private key" << std::endl;
		std::cout << "Press random keys or move the mouse if needed" << std::endl;
		
		const std::string randomStart = getFromDevRandom(28);
		
		BigInt<256> currentNum(randomStart, 'c');
		
		//const std::string hexStr = printBin(privKey.getBinaryVal(true));//ABC
		
		//Try if this has the correct hash start
		
		std::cout << std::endl;
		for(int i = 0 ; i < 100000 ; i++)
		{
			//std::cout << "Current: " << currentNum;
			const std::string b58 = Base58Encode(currentNum);
			//std::cout << "Len: " << b58.size() << std::endl;
			//std::cout << "Trying: " << b58;
			
			const std::string substr29 = b58.substr(b58.size() - 29);
			//std::cout << "substr29: " << substr29 << std::endl;
			
			const std::string miniPrivTry = std::string("S") + substr29;
			//std::cout << "\r";
			std::cout << "Trying: " << miniPrivTry << "   " << i << "\r";
			
			if(miniPrivChecksumOK(miniPrivTry))
			{
				//Make a last check, not really needed, but better be safe
				if(!isValidMiniPriv(miniPrivTry))
				{
					throw std::string("Internal error, incorrect mini priv key");
				}
				
				std::cout << "                                                           " << std::endl;
				std::cout << "Found mini private key: " << miniPrivTry << std::endl;
				
				const BitcoinKeyPair keypair = convertMiniPriv(miniPrivTry, compressed);				
				std::cout << "The corresponding public address is: " << keypair.pubaddr.toString() /*.address*/ << std::endl;
				return;
			}
			
			currentNum++;
		}

		std::cout << "                                         " << std::endl;
		std::cout << std::endl;
		std::cout << "Failed to find mini priv" << std::endl;
}


