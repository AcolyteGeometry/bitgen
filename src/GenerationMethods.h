#ifndef GENERATION_METHODS_H
#define GENERATION_METHODS_H

/**
 * GenerationMethods.h - Bitcoin poscript generator
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
 
 
#include <vector>
#include <string>
#include <list>

#include "GenerationParameters.h"
#include "bitcoinKeyPair.h"
#include "extraInfo.h"

#include "mnemonic.h"


//Todo: Move to separate file
class ExtraOptionsRandom
{
public:
	ExtraOptionsRandom() :
		useURandom(false),
		longbuf(false),
		onlyIndicatedUsage(false),
		signVerify(false)
		{}

	bool useArg(const std::string &arg);

//private:
	bool useURandom;
	bool longbuf;
	bool onlyIndicatedUsage;
	bool signVerify;	
	
};



class GenerationMethods
{
public:	

	static void generateBrainWallet(const std::vector<std::string>& args, const std::string& thetime);
	static void generateHash(const std::vector<std::string>& args, const std::string& thetime);
	static void generateHex(const std::vector<std::string>& args, const std::string& thetime);
	static void generateBulk(const std::vector<std::string>& args, const std::string& thetime);
	static void generateWithDice(const std::vector<std::string>& args, const std::string& thetime);
	static void generateFromMnemonic(const std::vector<std::string>& args, const std::string& thetime);
	static void generateRandom(const std::vector<std::string>& args, const std::string& thetime);
	static void generateFromWif(const std::vector<std::string>& args, const std::string& thetime);
	
	static void printRandomSourceWarning(const std::string generator = "/dev/random");
	
	static std::string generateOneFileContents(const BitcoinKeyPair& keypair, const bool addPubkey, std::string& customString);
	
	
	static void saveAllFiles(const std::string& pubaddr, const std::string& privaddr, const std::string& genMethod,
		const std::list<ExtraInfo>& generationExtra, const std::string& thetime, const GenerationParameters& params,
		const Mnemonic& mnemonic, 
		const int newNum = 0
	);
	
	static void saveExtendedFiles(const std::string& extPrivate, const std::string& extPublic, const std::string& generator,
		const std::string& thetime, const GenerationParameters& params, const std::list<ExtraInfo>& generationExtra, 
		const std::string& derivationPath, const MnemonicBase& mnemonic, const bool isElectrum);
	
	
		
		
	static void createElectrumMnemonic(const std::string& thetime, const std::string& mnemonicRaw, const GenerationParameters& params, const std::list<ExtraInfo>& extra);
	static void createElectrumRandom(const std::string& thetime, const int numWords, const GenerationParameters& params);
	static void createElectrumHash(const std::string& thetime, const int numWords, const std::string& hash, const GenerationParameters& params);
	static void createElectrumHex(const std::string& thetime, const int numWords, const std::string& hex, const GenerationParameters& params);
	static void createElectrumDice(const std::string& thetime, const int numWords, const std::string& diceStr, const GenerationParameters& params);
	static void createElectrumBrain(const std::string& thetime, const int numWords, const std::string& salt, const std::string& brainVal, const GenerationParameters& params);
	
	
	static void createExtendedSeed(const std::string& thetime, const std::string& hashval, const GenerationParameters& params);
	static void createExtendedMnemonic(const std::string& thetime, const std::string& seedval, const GenerationParameters& params);	
	static void createExtendedRandom(const std::string& thetime, const GenerationParameters& params);
	static void createExtendedHash(const std::string& thetime, const std::string& hashval, const GenerationParameters& params);
	static void createExtendedHex(const std::string& thetime, const std::string& hexhval, const GenerationParameters& params);
	static void createExtendedDice(const std::string& thetime, const std::string& diceval, const GenerationParameters& params);
	static void createExtendedExtBrain(const std::string& thetime, const std::string& salt, const std::string& brainVal, 
		const GenerationParameters& params);
	static void createExtendedExtPriv(const std::string& thetime, const std::string& exprivVal, const GenerationParameters& params);
		


	
	
private:
	static void generateElectrumMnemonicInfo(const std::string& thetime, const BigInt<256>& randomNum, 
		const int numWords, const std::string& genMethod, const GenerationParameters& params, 
		const std::list<ExtraInfo>& extra);
	
	static void saveElectrumMnemonic(const std::string& thetime,const ElectrumMnemonic& mnemonic, 
		const std::string& genMethod, const GenerationParameters& params,
		const std::list<ExtraInfo>& extra);


	static void splitPassphrase(const std::string& passphrase, std::list<std::string>& generationExtra);
};


#endif
