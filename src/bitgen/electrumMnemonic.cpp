/**
 * electruMnemonic.cpp - Bitcoin poscript generator
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



#include "mnemonic.h"


#include <list>
#include "bigintutil.h"

#include "ExtendedKey.h"

#include "hmac.h"



ExtendedPrivateKey ElectrumMnemonic::getPrivateFromElectrumMnemonic(/*const ElectrumMnemonic& mnemonicStr*/) const
{	
	const std::string seed = PbKdb2HmacSha512(/*mnemonicStr.*/toStr(), "electrum", 2048, 64);
	//std::cout << "Seed: " << hexString(seed) << std::endl;
		
	const ExtendedPrivateKey extendedPrivate = calculateMasterKey(seed);
	return extendedPrivate;
}




std::string ElectrumMnemonic::verifyElectrumMnemonic(const std::string& mnemonicRaw)
{
	
	//const std::string mnemonicStr = checkWordsInWordlist(mnemonicRaw);		//Todo: Permit all strings for electrum mnemonic?
	//std::cout << "Normalized mnemonic: \"" << mnemonicStr << "\"" << std::endl;
	
	//if(permitEmpty && mnemonicRaw.empty())	//Permit empty string
	//{
	//	return mnemonicRaw;
	//}

	
	verifyElectrumChecksum(mnemonicRaw);
		
	return mnemonicRaw;
}




bool ElectrumMnemonic::isCorrectElectrumVer(const std::string& mnemonic)
{	
	const unsigned char first = ElectrumMnemonic::getElectrumMnemonicVersion(mnemonic);
	return (first == 1);
}


std::string ElectrumMnemonic::electrumSeedVer()
{
	std::string seedVer("Seed version");
	while(seedVer.size() < 32)
	{
		seedVer += '\0';
	}

	return seedVer;
}



unsigned char ElectrumMnemonic::getElectrumMnemonicVersion(const std::string& mnemonic)
{
	const std::string seedVer = electrumSeedVer();
	
	const std::string hmacRes = calcHmacSha512(seedVer, mnemonic);
	
	//std::cout << "Full hmac: " << hexString(hmacRes) << std::endl;
		
	const std::string firstByteHmac = hmacRes.substr(0, 1);
	if(firstByteHmac.empty())
	{
		throw std::string("Internal error, hmac empty");
	}
	const unsigned char first = firstByteHmac[0];
	return first;
}



ElectrumMnemonic ElectrumMnemonic::generateElectrum(BigInt<256> randomNum, const int numWords)
{	
	for(int i = 0 ; ; i++)
	{
		const std::string mnemonic = MnemonicBase::getFromRandomWithoutChecksum(randomNum, numWords);
	
		//std::cout << std::endl;
		//std::cout << i << " total: \"" << mnemonic << "\"" << std::endl;
		
		if(ElectrumMnemonic::isCorrectElectrumVer(mnemonic))
		{
			//std::cout << i << std::endl;
			//std::cout << "Found seed: " << mnemonic << std::endl;
			//return mnemonic;
			const ElectrumMnemonic mnemonicObj(mnemonic); //, false, true);
			return mnemonicObj;
		}
		
		randomNum++;
	}

	//Should never get here
	throw std::string("Internal error in generateElectrum");
	const ElectrumMnemonic emptyMnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC);
	return emptyMnemonic;
}




void ElectrumMnemonic::verifyElectrumChecksum(const std::string& mnemonic)
{
	const unsigned char first = getElectrumMnemonicVersion(mnemonic);
	
	if(first != 1)
	{
		std::cout << "Electrum first byte: " << int(first) << std::endl;		
		throw std::string("Incorrect electrum checksum");
	}
}
