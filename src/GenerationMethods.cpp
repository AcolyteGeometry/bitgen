/**
 * GenerationMethods.cpp - Bitcoin address generator
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
 

#include "GenerationMethods.h"
#include "GenerationParameters.h"
#include "commandLine.h"
#include "util.h"
#include "random.h"
#include "privKey.h"
#include "bitcoinKeyPair.h"
#include "mnemonic.h"
#include "SaveFolderHandler.h"
#include "txtGeneration.h"
#include "qrenc.h"
#include "randomUtils.h"
#include "FileUtil.h"
#include "currencySpecific.h"
#include "keyDerivationFunction.h"
#include "version.h"
#include "printPS.h"
#include "ExtendedKey.h"
#include "hmac.h"

#include <iostream>
#include <list>
#include <stdlib.h>
#include <algorithm> 
#include <iterator>


void GenerationMethods::splitPassphrase(const std::string& passphrase, std::list<std::string>& generationExtra)
{	
	int lastNewlinePos(0);
	for(int i = 0 ; i < passphrase.size() ; i++)
	{
		const char c = passphrase[i];
		
		const int rowLength = i - lastNewlinePos;
		
		//std::cout << "RowLength: " << rowLength << std::endl;
		
		if(rowLength > 40 && c == ' ')
		{
			const std::string row = passphrase.substr(lastNewlinePos, i - lastNewlinePos);
			generationExtra.push_back(row);
			
			lastNewlinePos = i + 1;
		}
		else if(rowLength > 50)
		{
			const std::string row = passphrase.substr(lastNewlinePos, i - lastNewlinePos);
			generationExtra.push_back(row);
			
			lastNewlinePos = i;
		}
	}
	
	const std::string row = passphrase.substr(lastNewlinePos);
	generationExtra.push_back(row);
	
}




//Todo: Move to separate file
bool ExtraOptionsRandom::useArg(const std::string &arg)
{
	if(arg == "urandom")
	{
		if(longbuf)
		{
			throw std::string("Can not combine urandom and long");
		}
		useURandom = true;
		return true;		
	}
	else if(arg == "long")
	{
		if(useURandom)
		{
			throw std::string("Can not combine urandom and long");
		}

		std::cout << "Using 128 bytes from /dev/random" << std::endl;
		longbuf = true;
		return true;				
	}
	else if(arg == "encrypt")
	{
		if(onlyIndicatedUsage)
		{
			throw std::string("Error, only one usage indication permitted");
		}
			
		onlyIndicatedUsage = true;
		signVerify = false;
		return true;				
	}
	else if(arg == "verify")
	{
		if(onlyIndicatedUsage)
		{
			throw std::string("Error, only one usage indication permitted");
		}
			
		onlyIndicatedUsage = true;
		signVerify = true;
		return true;				
	}		
	
	return false;
}




void GenerationMethods::printRandomSourceWarning(const std::string generator) // = "/dev/random")
{
	std::cout << "Generating root key information" << std::endl << std::endl;
		
	std::cout << "Warning: Random number generated outside of bitgen with ";
	
#ifdef __linux	
	std::cout << generator << ".";
#else
	std::cout << "CryptGenRandom().";
#endif
	
	std::cout << std::endl;
	std::cout << "         Ensure that your random number generator gives good random numbers." << std::endl;
	std::cout << "         Flaws in the random number generation " << std::endl;
	std::cout << "         might make the private key possible to guess." << std::endl;
	std::cout << "         Consider other random number generators, such as dice." << std::endl;				
}



void GenerationMethods::saveAllFiles(const std::string& pubaddr, const std::string& privaddr, const std::string& genMethod,
	const std::list<ExtraInfo>& generationExtra, const std::string& thetime, const GenerationParameters& params,
	const Mnemonic& mnemonic, 
	const int newNum// = 0
	)
{
	//std::cout << "Saving : " << (useFolder ? "folder" : "file") << std::endl;
	
	std::string privPath(".");
	std::string pubPath(".");
	
	if(params.getUseFolder())
	{
		//const int newNum = getNextNumberAndreateBaseFolders();
		
		const std::pair<std::string, std::string> folders = SaveFolderHandler::createNewFolders(newNum);
		
		privPath = folders.second;
		pubPath = folders.first;
	}

	
	if(params.getGenerateTXT())
	{
		saveTxtFiles(pubaddr, privaddr, genMethod, generationExtra, thetime, 
			params.getAccountName(), 
			mnemonic.toStr(), 
			params.getCompressed(), 
			pubPath, privPath, versionNumber, applicationName);
	}
	
	if(params.getGeneratePS())
	{		
		const Language lang = getLanguage();		
		const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang);
			//PostscriptGenerator::ADDRTYPE_NORMAL, );
		
		const PublicPostscriptInfo pubInfo(pubaddr, thetime, params.getAccountName());
		const PrivatePostscriptInfo privInfo(privaddr, mnemonic.toStr(), genMethod, params.getCompressed(), generationExtra);
		
		const PsInfoFile psinfo = postscriptGenerator.generate(privInfo, pubInfo);
			
		const std::string privFilename = pubaddr + "_private.ps";
		const std::string fullPrivPath = (privPath == ".") ? privFilename : (privPath + "/" + privFilename);
	
		std::cout << "Saving privfile: " << fullPrivPath << std::endl;
		fileWrite(fullPrivPath, psinfo.privatePs);
			
		const std::string pubFilename = pubaddr + "_share.ps";
		const std::string fullPubPath = (pubPath == ".") ? pubFilename : (pubPath + "/" + pubFilename);	
		std::cout << "Saving publfile: " << fullPubPath << std::endl;
		fileWrite(fullPubPath, psinfo.publicPs);
		
	}
}





void GenerationMethods::generateWithDice(const std::vector<std::string>& args, 
					const std::string& thetime)
{
	GenerationParameters params;
			
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen dice digits(1-6) [AccountName]" << std::endl;
		std::cout << " or" << std::endl;
		std::cout << "       bitgen dice - [AccountName]" << std::endl;
			
		return;
	}

	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);
	//std::cout << "Extra args: " << allExtraArgs.size() << std::endl;
	
	params.parseArgs(allExtraArgs);	
	params.finally();
	
		
	std::string dice(args[1]);
	if(dice == "-")
	{
		std::cout << "Enter the 100 dice values at one row as digits 1-6" << std::endl;
		std::cout << "Press enter when all digits are entered" << std::endl;
		
		dice = readStdinLine(100, true, oneToSix);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;
	}
		
	if(dice.size() != 100)
	{
		std::string errMsg("Need exactly 100 dice throws, got: ");
		errMsg += intToString(dice.size());
		errMsg += " throws";
			
		throw errMsg;
		
		//std::cerr << "Need exactly 100 dice throws, got: " << dice.size() << " throws" << std::endl;
		//return;
	}

	const int newNum = params.setAccountNameforFolder();

	std::cout << "Generating bitcoin address from dice, got " << dice.size() << " throws" << std::endl;
	std::cout << "Dice result (1-6): " << dice << std::endl;
		
	const BigInt<256> privkey = getRandomFrom6SideDice(dice);		
		
	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Dice val (1-6)", dice);
	generationExtra.push_back(extraInfo);

	//generationExtra.push_back("Dice val (1-6)   : ");
	//generationExtra.push_back(dice.substr(0, 50));
	//generationExtra.push_back(dice.substr(50));
		
		
	const PrivKey myPrivKey(privkey, params.getCompressed());
		
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

	std::cout << "PUB: " << keypair.pubaddr << std::endl;
	std::cout << "PRIV: " << keypair.privaddr << std::endl;

	const Mnemonic mnemonicObj(privkey); // = Mnemonic::generate(privkey);
	//const std::string mnemonic = mnemonicObj.toString(); //generateMnemonic(privkey);
	//std::cout << "Mnemonic: " << mnemonic << std::endl;

	saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "dice", 
					generationExtra, thetime, params, mnemonicObj, newNum);
}




void GenerationMethods::generateRandom(const std::vector<std::string>& args, const std::string& thetime)
{
	GenerationParameters params;	
#ifndef __linux
	std::cerr << "Random is not supported on Windows. Use \"hash\" instead" << std::endl;
	return;
#endif
	

	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 0);

	ExtraOptionsRandom extraOptions;
	
		
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		
		if(params.useArg(arg))
		{
			//Nothing
		}
		else if(extraOptions.useArg(arg))
		{
			//Nothing			
		}
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();
	
	
	if(extraOptions.useURandom)
	{
		std::cout << "Using /dev/urandom with hash of buffer" << std::endl;		
	}


	const int newNum = params.setAccountNameforFolder();

	const std::string generator = extraOptions.useURandom ? "/dev/urandom" : "/dev/random";

	printRandomSourceWarning(generator);
		
	std::cout << "Generating random address" << std::endl;
	if(!extraOptions.useURandom)
	{
		std::cout << "Press random keys or move the mouse if needed" << std::endl;
	}
	
	const BigInt<256> privKey = extraOptions.useURandom ? getBlockCalcRandomNumber() : getRandomNumber(extraOptions.longbuf);
		
	const PrivKey myPrivKey(privKey, params.getCompressed());

		
	const std::string hexStr = hexString(privKey.getBinaryLong());

	std::list<ExtraInfo> generationExtra;
	
	const ExtraInfo extraInfo("Raw hex", hexStr);
	generationExtra.push_back(extraInfo);
	
	//generationExtra.push_back("Raw hex: ");
	//generationExtra.push_back(hexStr.substr(0, 32));
	//generationExtra.push_back(hexStr.substr(32));

	
	BitcoinEllipticCurve ec;

	const Wif wif = myPrivKey.getWif();
	const PubKey pubKey = myPrivKey.getPubkey(ec, extraOptions.onlyIndicatedUsage, extraOptions.signVerify);
	const Pkif pkif = pubKey.getPkif();
	const PubAddress pubAddr = pubKey.getAddress();
	
	std::cout << "PUB : " << pubAddr << std::endl;
	std::cout << "PKIF: " << pkif.toString() << std::endl;	
	std::cout << "PRIV: " << wif.toString() << std::endl;
		
	const Mnemonic mnemonicObj(privKey); // = Mnemonic::generate(privKey);
	//const std::string mnemonic = mnemonicObj.toString(); //generateMnemonic(privKey); 
		
	saveAllFiles(pubAddr.toString() , wif.toString(), generator, generationExtra, thetime, params, mnemonicObj, newNum);
}






void GenerationMethods::generateFromWif(const std::vector<std::string>& args, const std::string& thetime)
{
	//std::cout << "Generating from wif" << std::endl;
	GenerationParameters params;	
	
	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);

	ExtraOptionsRandom extraOptions;
	
		
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		
		if(params.useArg(arg))
		{
			//Nothing
		}
		else if(extraOptions.useArg(arg))
		{
			//Nothing			
		}
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();
	
	const int newNum = params.setAccountNameforFolder();
		
	std::string wifStr(args[1]);

	const Wif wif(wifStr);
	
	const PrivKey myPrivKey = wif.getPriv();

	std::list<ExtraInfo> generationExtra;
	
	const ExtraInfo extraInfo("Wif", "");
	generationExtra.push_back(extraInfo);

	
	BitcoinEllipticCurve ec;

	const PubKey pubKey = myPrivKey.getPubkey(ec, extraOptions.onlyIndicatedUsage, extraOptions.signVerify);
	const Pkif pkif = pubKey.getPkif();
	const PubAddress pubAddr = pubKey.getAddress();
	
	std::cout << "PUB : " << pubAddr << std::endl;
	std::cout << "PKIF: " << pkif.toString() << std::endl;	
	std::cout << "PRIV: " << wif.toString() << std::endl;
				
	//const std::string mnemonic = generateMnemonic(myPrivKey.key); 
	const Mnemonic mnemonicObj(myPrivKey.key); // = Mnemonic::generate(myPrivKey.key);
	//const std::string mnemonic = mnemonicObj.toString(); //generateMnemonic(privKey); 
		
		
	saveAllFiles(pubAddr.toString() , wif.toString(), "Wif",
		generationExtra, thetime, params, mnemonicObj, newNum);
}




std::string GenerationMethods::generateOneFileContents(const BitcoinKeyPair& keypair, const bool addPubkey, std::string& customString)
{
	std::string contents;
	
	if(!customString.empty())
	{
		for(int i = 0 ; i < customString.size() ; i++)
		{
			const char c = customString[i];
			if(c == '#')		//Pub
			{
				contents += keypair.pubaddr.toString();				
			}
			else if(c == '@')	//Priv
			{
				contents += keypair.privaddr.toString();
			}
			else
			{
				contents += c;
			}
		}

		contents += "\n";

		return contents;
	}
	
	contents += keypair.pubaddr.toString();
	contents += ", ";
	if(addPubkey)
	{
		contents += keypair.pubaddr.toString();
		contents += ", ";							
	}
	contents += keypair.privaddr.toString();
	contents += "\n";
	
	return contents;
}


void GenerationMethods::generateBulk(const std::vector<std::string>& args, const std::string& thetime)
{		
	GenerationParameters params;
		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen bulk NumAddresses [extraOptions]" << std::endl;
		return;			
	}

	const std::string numAddresses(args[1]);
	//std::cout << "NUM ADDR: " << numAddresses << std::endl;
	const int num = atoi(numAddresses.c_str());


	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);
		
	bool useURandom(false);
	bool longbuf(false);	
	bool oneFile(false);
	bool addPubkey(false);
	bool readCustom(false);
	
	std::string customString;
	
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		
		if(readCustom)
		{
			readCustom = false;
			customString = arg;
			std::cout << "Custom string: " << customString << std::endl;
			continue;
		}
		
		if(params.useArg(arg))
		{
			//Nothing
		}						
		else if(arg == "urandom")
		{
			if(longbuf)
			{
				throw std::string("Can not combine urandom and long");
			}
			useURandom = true;
		}
		else if(arg == "long")
		{
			if(useURandom)
			{
				throw std::string("Can not combine urandom and long");
			}
			
			std::cout << "Using 128 bytes from /dev/random" << std::endl;
			longbuf = true;
		}
		else if(arg == "onefile")
		{
			oneFile = true;
		}
		else if(arg == "onefilecustom")
		{
			oneFile = true;
			readCustom = true;
		}		
		else if(arg == "pubkey")
		{
			addPubkey = true;
		}		
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();
	
	const std::string generator = useURandom ? "/dev/urandom" : "/dev/random";


	std::cout << "Generating " << num << " random addresses" << std::endl;
	
	if(!useURandom)
	{
		std::cout << "Press random keys or move the mouse if needed" << std::endl;
	}

	std::string contents;
	std::string filename;

	for(int i = 0 ; i < num ; i++)
	{
		std::cout << "Address " << (i + 1) << " " << std::flush;

		const int newNum = params.setAccountNameforFolder();
		const BigInt<256> privKey = useURandom ? getBlockCalcRandomNumber() : getRandomNumber(longbuf);
				
		const PrivKey myPrivKey(privKey, params.getCompressed());
		const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

		std::cout << keypair.pubaddr.toString() << std::endl;
		
		if(oneFile)
		{
			filename = std::string("bulk_") + keypair.pubaddr.toString() + ".txt";
			
			contents += generateOneFileContents(keypair, addPubkey, customString);			
		}
		else
		{	
			
			const std::string hexStr = hexString(privKey.getBinaryLong());

			std::list<ExtraInfo> generationExtra;
			const ExtraInfo extraInfo("Raw hex", hexStr);
			generationExtra.push_back(extraInfo);
						
			const Mnemonic mnemonicObj(privKey);

			
			saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), generator,
				generationExtra, thetime, params, mnemonicObj, newNum);				
		}
	}

	if(oneFile)
	{
		std::cout << std::endl;
		std::cout << "Writing file: " << filename << std::endl;
		fileWrite(filename, contents);

	}
		
	std::cout << std::endl;
	std::cout << "All addresses generated. Ready" << std::endl;
}







void GenerationMethods::generateHash(const std::vector<std::string>& args,
	const std::string& thetime)
{
	GenerationParameters params;
	const int minChars(200);
	
	std::cout << "Generating address from hash" << std::endl;
		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen hash \"The hashval\" [extraOptions]" << std::endl;
		std::cout << " or" << std::endl;
		std::cout << "       bitgen hash - [extraOptions]" << std::endl;
		
		return;			
	}


	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);
	//std::cout << "Num extra args: " << allExtraArgs.size() << std::endl;

		
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		if(params.useArg(arg))
		{
			//Nothing
		}										
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();

	std::string hashval(args[1]);
	
	if(hashval == "-")
	{
		std::cout << "Enter the hash string at one row" << std::endl;
		std::cout << "Press enter when everything is entered" << std::endl;
		
		hashval = readStdinLine(minChars, false, everythingOk);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;		
	}


	const int newNum = params.setAccountNameforFolder();
		
	checkHashEntropy(hashval, minChars);
								
		
	const std::string sha = sha256(hashval);
	const BigInt<256> privKey(sha, 'c');
		
		
	std::list<ExtraInfo> generationExtra;
	const std::string privHex = hexString(privKey.getBinaryLong());

	const ExtraInfo extraInfo("Hex from hash", privHex);
	generationExtra.push_back(extraInfo);
	
	
	//generationExtra.push_back("Hex from hash    : ");
	//generationExtra.push_back(privHex.substr(0, 32));
	//generationExtra.push_back(privHex.substr(32));
		
	const PrivKey myPrivKey(privKey, params.getCompressed());

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

	//const std::string mnemonic = generateMnemonic(privKey);
	const Mnemonic mnemonicObj(privKey); // = Mnemonic::generate(privKey);
	//const std::string mnemonic = mnemonicObj.toString();


	saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "hash", 
		generationExtra, thetime, params, mnemonicObj, newNum);
}






void GenerationMethods::generateHex(const std::vector<std::string>& args, const std::string& thetime)
{
	GenerationParameters params;
		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen-" << getCurrencyShortName() << " hex hexdigits [extraOptions]" << std::endl;
		std::cout << " or" << std::endl;
		std::cout << "       bitgen-" << getCurrencyShortName() << " hex - [extraOptions]" << std::endl;
		
		return;			
	}
		
	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);
	//std::cout << "Num extra args: " << allExtraArgs.size() << std::endl;
	
	bool printInfo(false);
		
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		if(params.useArg(arg))
		{
			//Nothing
		}										
		else if(arg == "info")	//Todo: Move to GenerationParameters ???
		{
			printInfo = true;
		}									
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();
		
	//const
	std::string privKeyStrMix(args[1]);

	if(privKeyStrMix == "-")
	{
		std::cout << "Enter the 64 hex values at one row" << std::endl;
		std::cout << "Press enter when all digits are entered" << std::endl;
		
		privKeyStrMix = readStdinLine(64, true, hexChar);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;
		std::cout << std::endl;
	}
	
	if(privKeyStrMix.size() != 64)
	{
		std::string errMsg("Privkey must be 64 hex characters, got: ");
		errMsg += intToString(privKeyStrMix.size());
			
		throw errMsg;		
	}
		
	
	//std::cout << "The privkey: " << privKeyStr << std::endl;
	const std::string privKeyStrUpper = toUpper(privKeyStrMix);
		
	const BigInt<256> privKey = BigInt<256>::fromHex(privKeyStrUpper);

	const int newNum = params.setAccountNameforFolder();
/*	
	std::string accountName = params.getAccountName();
	int newNum(0);
	if(params.getUseFolder() && accountName.empty())
	{
		newNum = SaveFolderHandler::getNextNumberAndreateBaseFolders();
		const std::string currency = getCurrencyShortName();
		accountName = currency + " address " + intToString(newNum);
	}
*/


	std::cout << "Generating bitcoin address from hex private key" << std::endl;

	std::cout << "Private key (hex): " << hexString(privKey.getBinaryLong() /*getBinaryVal(true)*/) << std::endl;	

	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Hex privkey", privKeyStrUpper);
	generationExtra.push_back(extraInfo);				
	
	//generationExtra.push_back("Hex privkey      : ");				
	//generationExtra.push_back(privKeyStrUpper.substr(0, 32));
	//generationExtra.push_back(privKeyStrUpper.substr(32));

	BitcoinEllipticCurve ec;

	const PrivKey myPrivKey(privKey, params.getCompressed());
	const PubKey pubKey = myPrivKey.getPubkey(ec);
	const PubAddress address = pubKey.getAddress();

	if(printInfo)
	{
		std::cout << "Wif    : " << myPrivKey.toString() << std::endl;
		std::cout << "Pkif   : " << pubKey.toString() << std::endl;		
		std::cout << "Address: " << address.toString() << std::endl;
		
	}

	//const std::string mnemonic = generateMnemonic(privKey);
	const Mnemonic mnemonicObj(privKey); // = Mnemonic::generate(privKey);
	//const std::string mnemonic = mnemonicObj.toString();


		
	//params.setAccountName(accountName);
	saveAllFiles(address.toString(), myPrivKey.toString(), "hex input", 
		generationExtra, thetime, params, mnemonicObj, newNum);
}







void GenerationMethods::generateFromMnemonic(const std::vector<std::string>& args, const std::string& thetime)
{
	GenerationParameters params;	
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen mnemonic Mnemonic [extraOptions]" << std::endl;
		std::cout << "       bitgen mnemonic - [extraOptions]" << std::endl;
		
		return;			
	}
					
	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 1);
	//std::cout << "Num extra args: " << allExtraArgs.size() << std::endl;

	
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		if(params.useArg(arg))
		{
			//Nothing
		}
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
		
	params.finally();
	
	std::string mnemonicStr(args[1]);
	
	if(mnemonicStr == "-")
	{
		std::cout << "Enter the mnemonic string at one row" << std::endl;
		std::cout << "Press enter when everything is entered" << std::endl;

		mnemonicStr = readStdinLine(0, false, everythingOk);		
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;
		std::cout << std::endl;
	}
			
	const Mnemonic mnemonicObj(mnemonicStr/*, false*/);
	const BigInt<256> privKey = mnemonicObj.toPrivKey();
	


	std::cout << "Generating bitcoin address from mnemonic private key" << std::endl;
	std::cout << "Private key (hex): " << hexString(privKey.getBinaryLong() /*getBinaryVal(true)*/) << std::endl;	

	std::string accountName = params.getAccountName();
	int newNum(0);
	if(params.getUseFolder() && accountName.empty())
	{
		newNum = SaveFolderHandler::getNextNumberAndCreateBaseFolders();
		const std::string currency = getCurrencyShortName();
		accountName = currency + " address " + intToString(newNum);
	}
	params.setAccountName(accountName);

	std::list<ExtraInfo> generationExtra;

	const PrivKey myPrivKey(privKey, params.getCompressed());

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

	const Mnemonic myMnemonicObj(privKey); // = Mnemonic::generate(privKey);

		
	saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "mnemonic input", 
		generationExtra, thetime, params, myMnemonicObj, newNum);
}





#ifndef NO_BRAINWALLET_GENERATION

void GenerationMethods::generateBrainWallet(const std::vector<std::string>& args,
	const std::string& thetime)
{	
	GenerationParameters params;	
		
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen brainwallet email/salt \"The passphrase\" [extraOptions]" << std::endl;
		return;
	}

	const std::vector<std::string> allExtraArgs = getExtraArgs(args, 2);
	std::cout << "Num extra args: " << allExtraArgs.size() << std::endl;

	
	for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
		it != allExtraArgs.end() ;
		++it)
	{
		const std::string& arg = (*it);
		if(params.useArg(arg))
		{
			//Nothing
		}								
		else
		{
			std::cout << "Incorrect argument: " << arg << std::endl;
			throw std::string("Argument error");			
		}
	}
	
	params.finally();
	
	const std::string salt(args[1]);

	std::string passphrase(args[2]);
	if(passphrase == "-")
	{
		std::cout << "Enter the brainwallet phrase" << std::endl;
		std::cout << "Press enter when all characters are entered" << std::endl;
		
		passphrase = readStdinLine(50, false, everythingOk);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;		
	}

		
	checkPassphraseEntropy(passphrase);

	std::cout << "Generating brainwallet" << std::endl;

	std::cout << "Salt: \"" << salt << "\"" << std::endl;		
	std::cout << "Passphrase: \"" << passphrase << "\"" << std::endl;
		
				
	std::cout << "Calculating address, this may take a while..." << std::endl;	
	const std::string kdfStr = bitgenArgon2Kdf(passphrase, salt);
	if(kdfStr.size() != 32)
	{
		throw std::string("KDF error");
	}
	const std::string sha = sha256(kdfStr);
	const BigInt<256> privKey(sha, 'c');
		
		
	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Passphrase", passphrase);
	generationExtra.push_back(extraInfo);		

	//generationExtra.push_back("Passphrase: ");		
	//splitPassphrase(passphrase, generationExtra);
		
		
	std::cout << "Calculating keys..." << std::endl;	
		
	const PrivKey myPrivKey(privKey, params.getCompressed());
		
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey);

	//const std::string mnemonic = generateMnemonic(privKey);
	const Mnemonic myMnemonicObj(privKey); // = Mnemonic::generate(privKey);
	//const std::string mnemonic = myMnemonicObj.toString();


	saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "brainwallet", 
		generationExtra, thetime, params, myMnemonicObj);
}

#endif







void GenerationMethods::saveExtendedFiles(const std::string& extPrivate, const std::string& extPublic, const std::string& generator,
	const std::string& thetime, const GenerationParameters& params, const std::list<ExtraInfo>& generationExtra, 
	const std::string& derivationPath,
	const MnemonicBase& mnemonic,
	const bool isElectrum = false
	)
{	
	const std::string accountName = params.getAccountName();
	
	const std::string fileId = extPublic.substr(50);
	
	if(params.getGenerateTXT())
	{
		FilePair files = generateExtendedTxtFiles(extPublic, extPrivate, 
						generator, thetime, accountName, versionNumber, applicationName, generationExtra, derivationPath,
						params.getCurrencySpecific());
		

		saveExtendedTxtFiles(files, fileId, params.getCurrencySpecific());
	}	
	
	
	if(params.getGeneratePS())
	{
		//std::cout << "Will save ps files" << std::endl;
		
		const Language lang = getLanguage();		
		const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang, 
			(isElectrum ? PostscriptGenerator::ADDRTYPE_ELECTRUM : PostscriptGenerator::ADDRTYPE_EXTENDED), 
			params.getCurrencySpecific());
		
		//std::cout << "1" << std::endl;
		const PublicPostscriptInfo pubInfo(extPublic, thetime, accountName);
		
		//const std::list<std::string> generationExtra;
		const PrivatePostscriptInfo privInfo(extPrivate, mnemonic.toStr(), generator, true, generationExtra);
		//std::cout << "2" << std::endl;
		
		const PsInfoFile psinfo = postscriptGenerator.generate(privInfo, pubInfo);

		//std::cout << "3" << std::endl;

		std::string currencySpecStr;
		if(params.getCurrencySpecific())
		{
			currencySpecStr += getCurrencyShortName();
			currencySpecStr += "_";		
		}

		const std::string privFilename = "extended_" + currencySpecStr + fileId + "_private.ps";
		//const std::string fullPrivPath = (privPath == ".") ? privFilename : (privPath + "/" + privFilename);
	
		std::cout << "Saving privfile: " << privFilename << std::endl;
		fileWrite(privFilename, psinfo.privatePs);
			
		const std::string pubFilename = "extended_" + currencySpecStr + fileId + "_share.ps";
		//const std::string fullPubPath = (pubPath == ".") ? pubFilename : (pubPath + "/" + pubFilename);	
		std::cout << "Saving publfile: " << pubFilename << std::endl;
		fileWrite(pubFilename, psinfo.publicPs);
		
	}

}





void GenerationMethods::createExtendedRandom(const std::string& thetime, const GenerationParameters& params)
{
	const bool useCurrencySpecific = params.getCurrencySpecific();
	//;
	//std::cout << "Creating extended key" << std::endl;
	if(useCurrencySpecific)
	{
		std::cout << "Will use currencyspecific addresses" << std::endl;
	}
	
	std::cout << "Generating random data for private key..." << std::endl;
	const BigInt<256> privKeyInt = getRandomNumber();	
	const PrivKey privKey(privKeyInt, true);

	std::cout << "Generating random data for chain code..." << std::endl;	
	const BigInt<256> chainCode = getRandomNumber();	
	
	const BigInt<32> zero;	
	const ExtendedPrivateKey extendedPrivate(0, zero, zero, chainCode, privKey);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();


	//std::cout << "Created extended" << std::endl << extendedPrivate.toString() << std::endl;
	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	std::list<ExtraInfo> generationExtra;

	saveExtendedFiles(extendedPrivate.toString(params.getCurrencySpecific()), extendedPublicKey.toString(params.getCurrencySpecific()), "/dev/random", thetime, 
		params, generationExtra, 
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));
}



void GenerationMethods::createExtendedSeed(const std::string& thetime, const std::string& seedval, const GenerationParameters& params)
{
	std::cout << "Creating extended key from hex seed" << std::endl;
	
	const std::string seedUpper = toUpper(seedval);

	const std::string seed(convertFromHex(seedUpper));
	const Mnemonic mnemonic(seed/*, false*/);

	const ExtendedPrivateKey extendedPrivate = calculateMasterKey(seed);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Seed", hexString(seed));
	generationExtra.push_back(extraInfo);


	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "seed", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), mnemonic /*seed*/);	
}






















void GenerationMethods::saveElectrumMnemonic(const std::string& thetime,
	const ElectrumMnemonic& mnemonic, 
	const std::string& genMethod, 
	const GenerationParameters& params,
	const std::list<ExtraInfo>& extra)
{	
	//if(!mnemonic.electrum())
	//{
	//	throw std::string("Internal error, mnemonic not electrum type");
	//}
	
	
	
	std::cout << "Found seed: " << mnemonic.toStr() << std::endl;	
	std::cout << std::endl;

	const ExtendedPrivateKey extendedPrivate = /*ElectrumMnemonic::*/mnemonic.getPrivateFromElectrumMnemonic();	
	
	std::cout << "Extended private: " << extendedPrivate.toString() << std::endl;
	
	const ExtendedPublicKey extendedPub = extendedPrivate.getPub();	

	std::cout << "Extended pub: " << extendedPub.toString() << std::endl;
	std::cout << std::endl;
	

	std::list<ExtraInfo> generationExtra;
		
	const ExtraInfo extraInfo("Mnemonic", mnemonic.toStr(), true);
	generationExtra.push_back(extraInfo);

	generationExtra.insert(generationExtra.end(), extra.begin(), extra.end());

	saveExtendedFiles(extendedPrivate.toString(), extendedPub.toString(), genMethod, thetime, params, generationExtra,
		extendedPub.extendedData.chainString(), mnemonic, true);
}



void GenerationMethods::generateElectrumMnemonicInfo(const std::string& thetime,
	const BigInt<256>& randomNum, 
	const int numWords, 
	const std::string& genMethod, 
	const GenerationParameters& params,
	const std::list<ExtraInfo>& extra)
{
	const ElectrumMnemonic mnemonic = ElectrumMnemonic::generateElectrum(randomNum, numWords);
		
	saveElectrumMnemonic(thetime, mnemonic, genMethod, params, extra);
	
}




void GenerationMethods::createElectrumDice(const std::string& thetime,
	const int numWords,
	const std::string& diceStr, 
	const GenerationParameters& params)
{
	std::cout << "Creating electrum seed with " << numWords << " words from dice" << std::endl;
	
	const double numWordsDouble(numWords);
	
	const int numDiceNeeded = ((numWordsDouble * 11.0) / 2.63);
	
	if(diceStr.size() != numDiceNeeded)
	{
		std::cout << "Number dice needed: " << numDiceNeeded << ", got: " << diceStr.size() << std::endl;
		throw std::string("Not correct number dice digits");
	}
	
	const BigInt<256> randomNum = getRandomFrom6SideDice(diceStr, numDiceNeeded);		
	//std::cout << "Random: " << hexString(randomNum.getBinaryLong())	<< std::endl;
	//std::cout << "Non-zero digits: " << randomNum.numberBitsNeeded() << std::endl;
	
	
	std::list<ExtraInfo> extra;
	const ExtraInfo extraInfo("Dice", diceStr);	
	extra.push_back(extraInfo);
	generateElectrumMnemonicInfo(thetime, randomNum, numWords, "dice", params, extra);	
}





void GenerationMethods::createElectrumHex(const std::string& thetime,
	const int numWords,
	const std::string& hex, 
	const GenerationParameters& params)
{
	std::cout << "Creating electrum seed from hex" << std::endl;
	
	const int numWordsBound = (numWords * 11) * 2 / 8;
	const int numHexCharsNeeded = min(numWordsBound, 64);
	
	if(hex.size() < numHexCharsNeeded)
	{
		std::cout << "Num hex digits needed: " << numHexCharsNeeded << ", got: " << hex.size() << std::endl;
		throw std::string("Not enough hex digits");
	}
	else if(hex.size() > numHexCharsNeeded)
	{
		std::cout << "Num hex digits needed: " << numHexCharsNeeded << ", got: " << hex.size() << std::endl;
		throw std::string("Too many hex digits");
	}
	
	const std::string hexUpper = toUpper(hex);
		
	const BigInt<256> randomNum = BigInt<256>::fromHex(hexUpper);
	
	std::list<ExtraInfo> extra;
	const ExtraInfo extraInfo("Hex", hexUpper);	
	extra.push_back(extraInfo);
	
	generateElectrumMnemonicInfo(thetime, randomNum, numWords, "hex", params, extra);	
}





void GenerationMethods::createElectrumBrain(const std::string& thetime,
	const int numWords,
	const std::string& salt, const std::string& brainVal,	
	const GenerationParameters& params)
{
	const int minBrainStrSize(100);
	
	if(brainVal.size() < minBrainStrSize)
	{
		std::cout << "Brain string size: " << brainVal.size() << ", need: " << minBrainStrSize << std::endl;
		throw std::string("Brain string too short");
	}
	
	std::cout << "Creating electrum seed from brain string" << std::endl;
	
	std::cout << "Calculating privkey, this may take a while..." << std::endl;	
	const std::string kdfStr = bitgenArgon2Kdf(brainVal, salt);
	if(kdfStr.size() != 32)
	{
		throw std::string("KDF error");
	}
	const std::string sha = sha256(kdfStr);
	const BigInt<256> randomNum(sha, 'c');
		
			
	std::list<ExtraInfo> extra;
	const ExtraInfo extraInfo("Brain", brainVal, true);	
	extra.push_back(extraInfo);
	
	generateElectrumMnemonicInfo(thetime, randomNum, numWords, "brain", params, extra);	
}





void GenerationMethods::createElectrumHash(const std::string& thetime,
	const int numWords,
	const std::string& hash, 
	const GenerationParameters& params)
{
	std::cout << "Creating electrum seed from hash" << std::endl;
	
	checkHashEntropy(hash, 200);
								
		
	const std::string sha = sha256(hash);
	const BigInt<256> randomNum(sha, 'c');
	
	const std::list<ExtraInfo> extra;
	generateElectrumMnemonicInfo(thetime, randomNum, numWords, "hash", params, extra);	
}




void GenerationMethods::createElectrumRandom(const std::string& thetime,
	const int numWords, 
	const GenerationParameters& params)
{
	std::cout << "Creating electrum seed from /dev/random" << std::endl;
	
	BigInt<256> randomNum = getRandomNumber();		//Todo: Only get as much entropy that is needed with numWords
	//std::cout << "Random num: " << hexString(randomNum.getBinaryLong()) << std::endl;
	
	const std::list<ExtraInfo> extra;
	generateElectrumMnemonicInfo(thetime, randomNum, numWords, "random", params, extra);
}




void GenerationMethods::createElectrumMnemonic(const std::string& thetime, const std::string& mnemonicRaw, 
	const GenerationParameters& params, const std::list<ExtraInfo>& extra)
{
	std::cout << "Creating extended key from mnemonic" << std::endl;
	
	//Make sure we use lower case letters with single spaces.	
	//const std::string mnemonicStr = normalizeMnemonicStr(mnemonicRaw);
	const ElectrumMnemonic mnemonic(mnemonicRaw);
	const int numWords = mnemonic.numberWords();
	
	if(numWords > 24)
	{
		std::cout << "Number words incorrent, got: " << numWords << " words" << std::endl;
		throw std::string("Incorrect num words");
	}
	
	std::cout << "Normalized mnemonic: \"" << mnemonic.toStr() << "\"" << std::endl;
/*
	if(!mnemonic.verifyElectrumVersion())
	{
		std::cout << "Incorrect checksum for electrum seed" << std::endl;
		throw std::string("Incorrect checksum for electrum seed");
	}
*/

	saveElectrumMnemonic(thetime, mnemonic, /*numWords,*/ "mnemonic", params, extra);	
}




void GenerationMethods::createExtendedMnemonic(const std::string& thetime, const std::string& mnemonicRaw, 
	const GenerationParameters& params)
{
	std::cout << "Creating extended key from mnemonic" << std::endl;
	
	const Mnemonic mnemonic(mnemonicRaw/*, false*/);		//Verify the mnemonic
	
	//Make sure we use lower case letters with single spaces.	
	//const std::string mnemonicStr = normalizeMnemonicStr(mnemonicRaw);
	std::cout << "Normalized mnemonic: \"" << mnemonic.toStr() << "\"" << std::endl;
	
	const std::string not_used_privData = mnemonic.getAsNumber();	//Todo???


	const std::string passphrase = params.getPassword();


	const ExtendedPrivateKey extendedPrivate = /*Mnemonic::*/mnemonic.getExtendedPrivFromMnemonic(/*mnemonic.toStr(),*/ passphrase);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	if(params.getPrintExtendedLegacy())
	{
		std::cout << "Private legacy: " << extendedPrivate.toString(true) << std::endl;
		std::cout << "Public legacy : " << extendedPublicKey.toString(true) << std::endl;
		std::cout << std::endl;		
	}

	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Mnemonic", mnemonic.toStr(), true);
	generationExtra.push_back(extraInfo);

	if(!passphrase.empty())
	{
		const ExtraInfo extraInfo2("Password", passphrase);
		generationExtra.push_back(extraInfo2);
	}

	const ExtraInfo extraInfo3("EntBits", intToString(not_used_privData.size() * 8));
	generationExtra.push_back(extraInfo3);



	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "BIP 39 seed", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), mnemonic);	
}








void GenerationMethods::createExtendedHash(const std::string& thetime, const std::string& hashval, const GenerationParameters& params)
{
	std::cout << "Creating extended hash key" << std::endl;
	
	const int minHashSize(50);
	if(hashval.size() < minHashSize)
	{
		std::cerr << "Hash must be at least " << minHashSize << " characters" << std::endl;
		std::cerr << "Got hashval: \"" << hashval << "\"" << std::endl;
		throw std::string("Hash not long enough");
	}

	const int hashSizeHalf = hashval.size() / 2;
	const std::string hashLeft = hashval.substr(0, hashSizeHalf);
	const std::string hashRight = hashval.substr(hashSizeHalf);

	//std::cout << "L: " << hashLeft << std::endl;
	//std::cout << "R: " << hashRight << std::endl;
	
	const std::string shaleft = sha256(hashLeft);
	const BigInt<256> privKeyInt(shaleft, 'c');
	const PrivKey privKey(privKeyInt, true);
		
		
	const std::string sharight = sha256(hashRight);
	const BigInt<256> chainCode(sharight, 'c');
	
	const BigInt<32> zero;		//Todo: Make zero the default argument
	const ExtendedPrivateKey extendedPrivate(0, zero, zero, chainCode, privKey);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	std::list<ExtraInfo> generationExtra;

	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "hash", thetime, 
		params, generationExtra,
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));	
}







void GenerationMethods::createExtendedHex(const std::string& thetime, const std::string& hexhval, const GenerationParameters& params)
{
	std::cout << "Creating extended hex key" << std::endl;
	
	const int hexSize(128);
	if(hexhval.size() != hexSize)
	{
		std::cerr << "Hex value must be exactly " << hexSize << " characters" << std::endl;
		std::cerr << "Got hexval: \"" << hexhval << "\" with " << hexhval.size() << " characters" << std::endl;
		throw std::string("Incorrect hex length");
	}


	const int hexSizeHalf = 64;
	const std::string hexLeft = hexhval.substr(0, hexSizeHalf);
	const std::string hexRight = hexhval.substr(hexSizeHalf);

	//std::cout << "L: " << hexLeft << " size: " << hexLeft.size() << std::endl;
	//std::cout << "R: " << hexRight << " size: " << hexRight.size() << std::endl;


	const std::string privKeyStrUpper = toUpper(hexLeft);
	const BigInt<256> privKeyInt = BigInt<256>::fromHex(privKeyStrUpper);	
	const PrivKey privKey(privKeyInt, true);

	const std::string chainCodeStrUpper = toUpper(hexRight);
	const BigInt<256> chainCode = BigInt<256>::fromHex(chainCodeStrUpper);

	//std::cout << "PrivkeyInt: " << privKeyInt << std::endl;
	//std::cout << "ChainCoInt: " << chainCode << std::endl;
	
	
	const BigInt<32> zero;		//Todo: Make zero the default argument
	const ExtendedPrivateKey extendedPrivate(0, zero, zero, chainCode, privKey);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;


	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Hexval", hexhval);
	generationExtra.push_back(extraInfo);


	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "hex", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));	
}








void GenerationMethods::createExtendedDice(const std::string& thetime, const std::string& diceval, const GenerationParameters& params)
{
	std::cout << "Creating extended dice key" << std::endl;
	
	const int diceSize(200);
	if(diceval.size() != diceSize)
	{
		std::cerr << "Dice value must be exactly " << diceSize << " characters" << std::endl;
		std::cerr << "Got diceval: \"" << diceval << "\" with " << diceval.size() << " characters" << std::endl;
		throw std::string("Incorrect dice length");
	}


	const int diceSizeHalf = 100;
	const std::string diceLeft = diceval.substr(0, diceSizeHalf);
	const std::string diceRight = diceval.substr(diceSizeHalf);

	std::cout << "L: " << diceLeft << " size: " << diceLeft.size() << std::endl;
	std::cout << "R: " << diceRight << " size: " << diceRight.size() << std::endl;

	const BigInt<256> privKeyInt = getRandomFrom6SideDice(diceLeft);		
	const PrivKey privKey(privKeyInt, true);

	const BigInt<256> chainCode = getRandomFrom6SideDice(diceRight);		


	//std::cout << "PrivkeyInt: " << privKeyInt << std::endl;
	//std::cout << "ChainCoInt: " << chainCode << std::endl;
	
	
	const BigInt<32> zero;		//Todo: Make zero the default argument
	const ExtendedPrivateKey extendedPrivate(0, zero, zero, chainCode, privKey);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Diceval", diceval);
	generationExtra.push_back(extraInfo);

	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "dice", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));
}











#ifndef NO_BRAINWALLET_GENERATION



void GenerationMethods::createExtendedExtBrain(const std::string& thetime, const std::string& salt, const std::string& brainVal, 
	const GenerationParameters& params)
{
	std::cout << "Creating extended brain wallet" << std::endl;
	
	const int brainSize(200);
	if(brainVal.size() < brainSize)
	{
		std::cerr << "Brain size must be at least " << brainSize << " characters" << std::endl;
		std::cerr << "Got " << brainVal.size() << " characters" << std::endl;
		throw std::string("Incorrect brain wallet length");
	}


	const int brainSizeHalf = brainVal.size() / 2;
	const std::string brainLeft = brainVal.substr(0, brainSizeHalf);
	const std::string brainRight = brainVal.substr(brainSizeHalf);

	std::cout << "L: " << brainLeft << " size: " << brainLeft.size() << std::endl;
	std::cout << "R: " << brainRight << " size: " << brainRight.size() << std::endl;

	std::cout << "Generating brainwallet" << std::endl;

	std::cout << "Salt: \"" << salt << "\"" << std::endl;		
	std::cout << "Passphrase: \"" << brainRight << "\"" << std::endl;
		
				
	std::cout << "Calculating privkey, this may take a while..." << std::endl;	
	const std::string kdfStr = bitgenArgon2Kdf(brainRight, salt);
	if(kdfStr.size() != 32)
	{
		throw std::string("KDF error");
	}
	const std::string sha = sha256(kdfStr);
	const BigInt<256> privKeyInt(sha, 'c');
	const PrivKey privKey(privKeyInt, true);


	std::cout << "Calculating chain code, this may take a while..." << std::endl;	
	const std::string kdfStrChainCode = bitgenArgon2Kdf(brainLeft, salt);
	if(kdfStrChainCode.size() != 32)
	{
		throw std::string("KDF error");
	}
	const std::string shaChainCode = sha256(kdfStrChainCode);
	const BigInt<256> chainCode(shaChainCode, 'c');


	//std::cout << "PrivkeyInt: " << privKeyInt << std::endl;
	//std::cout << "ChainCoInt: " << chainCode << std::endl;
	
	
	const BigInt<32> zero;		//Todo: Make zero the default argument
	const ExtendedPrivateKey extendedPrivate(0, zero, zero, chainCode, privKey);

	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();

	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	//std::cout << "Will create gen extra list" << std::endl;
	std::list<ExtraInfo> generationExtra;

	//std::cout << "Will create gen extra 1" << std::endl;

	const ExtraInfo extraInfoSalt("Salt", salt);
	generationExtra.push_back(extraInfoSalt);

	//std::cout << "Will create gen extra 2" << std::endl;
	
	const ExtraInfo extraInfoPassphrase("Passphrase", brainVal);
	generationExtra.push_back(extraInfoPassphrase);
	
	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "brain", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));
	
}
#endif





void GenerationMethods::createExtendedExtPriv(const std::string& thetime, const std::string& exprivVal, const GenerationParameters& params)
{
	std::cout << "Using extended key from xpriv" << std::endl;
		
	const ExtendedKey extendedKey = decodeHD(exprivVal);
	if(!extendedKey.isPrivate)
	{
		std::cout << "This is not a private extended key" << std::endl;
		throw std::string("Incorrent argument");
	}

	const ExtendedPrivateKey extendedPrivate = decodePrivateHD(extendedKey);	
	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();
	
	std::cout << "Private: " << extendedPrivate.toString() << std::endl;
	std::cout << "Public : " << extendedPublicKey.toString() << std::endl;
	std::cout << std::endl;

	std::list<ExtraInfo> generationExtra;

	saveExtendedFiles(extendedPrivate.toString(), extendedPublicKey.toString(), "xprv", thetime, params, generationExtra,
		extendedPublicKey.extendedData.chainString(), Mnemonic(MnemonicBase::DUMMY_EMPTY_MNEMONIC));
}



