#ifndef MNEMONIC_H
#define MNEMONIC_H

/**
 * mnemonic.h - Bitcoin poscript generator
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

#include <string>
#include <list>
#include "bigint.h"
#include "ExtendedKey.h"


class MnemonicBase
{
public:
	enum DummyEmptyMnemonic { DUMMY_EMPTY_MNEMONIC };

	MnemonicBase(const MnemonicBase::DummyEmptyMnemonic dummy) : mnemonic("") {}

	MnemonicBase(const std::string& inMnemonic) : mnemonic(inMnemonic) {}
	
	std::string toStr() const { return mnemonic; }

	int numberWords() const;


protected:
	static std::string getFromRandomWithoutChecksum(const BigInt<256>& randomNum, const int numWords);
	static std::string mnemonicAt(const int num);


	const std::string mnemonic;	
};




class Mnemonic : public MnemonicBase
{
public:

	explicit Mnemonic(const MnemonicBase::DummyEmptyMnemonic dummy) : MnemonicBase("") 
	{
		//Todo: throw		
	};
		
	explicit Mnemonic(const std::string& inMnemonic);
	explicit Mnemonic(const BigInt<256>& myPrivkey, const int numWords = 24);

	int checkWords() const;


	BigInt<256> toPrivKey() const;
	std::string getAsNumber() const;		

	ExtendedPrivateKey getExtendedPrivFromMnemonic(const std::string& passphrase) const;
	
	//Todo: Move to MnemonicBase?
	static std::string bigIntToMnemonic(BigInt<264> total, const int numWords);
	static BigInt<264> mnemonicToBigInt(const std::string& mnemonic);
	
private:
	static Mnemonic generateVariableSize(const BigInt<256>& privkey, const int numWords);

	static Mnemonic generate(const BigInt<256>& privkey);	//Todo: Not needed



	static int getNumberTextWords(const std::string& mnemonicStr);
	static int getNumberChecksumBits(const std::string& mnemonicStr);


	static bool checkWordsInWordlist(std::string str, std::string& ret, std::list<std::string>& words, const int expectedNumWords = 0);
	static std::string checkWordsInWordlist(const std::string& mnemonic);



	static BigInt<264> getChecksum(const std::string& mnemonicStr);
	static BigInt<264> computeChecksum(const std::string& mnemonicStr);
	static void verifyChecksum(const std::string& mnemonicStr);

	//Todo: Rename to verify()
	static std::string verifyMnemonic(const std::string& mnemonicRaw);

};



class ElectrumMnemonic : public MnemonicBase
{
public:
	
	ElectrumMnemonic(const MnemonicBase::DummyEmptyMnemonic dummy) : MnemonicBase("") 
	{
		//Todo: throw
	}
	
	ElectrumMnemonic(const std::string& inMnemonic) :
		MnemonicBase(inMnemonic) 
		{
			verifyElectrumMnemonic(inMnemonic);
		};
	
	static ElectrumMnemonic generateElectrum(BigInt<256> randomNum, const int numWords);

//Todo: REMOVE static and param!!!!!!!!
	//static 
	ExtendedPrivateKey getPrivateFromElectrumMnemonic(/*const ElectrumMnemonic& mnemonicStr*/) const;
private:

	//Todo: Rename to verify()
	static std::string verifyElectrumMnemonic(const std::string& mnemonicRaw);

	static std::string electrumSeedVer();

	static bool isCorrectElectrumVer(const std::string& mnemonic);
	static void verifyElectrumChecksum (const std::string& mnemonic);
	static unsigned char getElectrumMnemonicVersion(const std::string& mnemonic);
	
};




#endif
