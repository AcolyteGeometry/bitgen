/**
 * keychainFunc.cpp - Bitcoin address generator
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
 


#include "keychainFunc.h"
#include "pubAddress.h"
#include "pkif.h"
#include "bitsigConfig.h"
#include "commandLine.h"
#include "randomUtils.h"
#include "random.h"
#include "util.h"

#include <iostream>




void createAddressFromHex(int argc, char* argv[])
{
	const bool compressed(false);
		
	if(argc < 3)
	{
		std::cout << "Usage: bitsig hex hexdigits" << std::endl;
		std::cout << "       bitsig hex -" << std::endl;
		
		return;			
	}
		
		
	//const
	std::string privKeyStr(argv[2]);

	if(privKeyStr == "-")
	{
		std::cout << "Enter the hex private key" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		privKeyStr = readStdinLine(64, true, hexChar);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
	}
	
	if(privKeyStr.size() != 64)
	{
		std::string errMsg("Privkey must be 64 hex characters, got: ");
		errMsg += intToString(privKeyStr.size());
			
		throw errMsg;
		
		//std::cout << "Privkey must be 64 hex characters, got: " << privKeyStr.size() << std::endl;
		//return;		
	}
	
	//std::cout << "The privkey: " << privKeyStr << std::endl;
	
	
	const BigInt<256> privKey(BigInt<256>::fromHex(privKeyStr)); //privZeroX.c_str());

	std::cout << "Generating bitcoin address from hex private key" << std::endl;
	std::cout << "Private key (hex): " << hexString(privKey.getBinaryLong() /*getBinaryVal(true)*/) << std::endl;	

	const PrivKey myPrivKey(privKey, compressed);

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);
		
	BitsigConfig::storePrivateKey(keypair);
}




void generatePrivateFromHash(int argc, char* argv[])
{		
	const bool compressed(false);
		
	std::cout << "Generating address from hash" << std::endl;
		
	if(argc < 3)
	{
		std::cout << "Usage: bitsig hash \"The hashval\"" << std::endl;
		return;			
	}



	//const
	std::string hashval(argv[2]);
	if(hashval == "-")
	{
		std::cout << "Enter the hash" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		hashval = readStdinLine(50, false, everythingOk);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
	}


		
	checkHashEntropy(hashval);
				
		
	bool printAll(false);
	if(argc >= 4)
	{
		const std::string extrastring(argv[3]);
		if(extrastring == "all")
		{
			printAll = true;
		}
	}	
		
	const std::string sha = sha256(hashval);
	const BigInt<256> privKey(sha, 'c');	

	const PrivKey myPrivKey(privKey, compressed);

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);

	BitsigConfig::storePrivateKey(keypair);
}




void generatePrivateFromBrainwallet(int argc, char* argv[])
{		
	std::cout << "Brainwallet generation currently disabled in bitsig" << std::endl;
	return;
	
	
	const bool compressed(false);
		
	std::cout << "Generating brainwallet" << std::endl;
		
	if(argc < 3)
	{
		std::cout << "Usage: bitsig brainwallet \"The passphrase\"" << std::endl;
		return;			
	}


	const std::string passphrase(argv[2]);
		
	checkPassphraseEntropy(passphrase);
		
	std::cout << "Passphrase       :\"" << passphrase << "\"" << std::endl;
		
	bool printAll(false);
	if(argc >= 5)
	{
		const std::string extrastring(argv[4]);
		//std::cout << "Extrastring: " << extrastring << std::endl;
		if(extrastring == "all")
		{
			printAll = true;
		}
		//return;
	}
		
		
	const std::string sha = sha256(passphrase);
	const BigInt<256> privKey(sha, 'c');
		
		
	std::list<std::string> generationExtra;
	generationExtra.push_back("Passphrase       : ");		
	generationExtra.push_back(passphrase);


	const PrivKey myPrivKey(privKey, compressed);

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);
		
	BitsigConfig::storePrivateKey(keypair);
}



void generatePrivateFromDice(int argc, char* argv[])
{
	const bool compressed(false);
	if(argc < 3)
	{
		std::cout << "Usage: bitsig dice digits (1-6)" << std::endl;
		return;			
	}
		
	//const
	std::string dice(argv[2]);
	
	if(dice == "-")
	{
		std::cout << "Enter the wif" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		dice = readStdinLine(100, true, oneToSix);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
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

		
	std::cout << "Generating bitcoin address from dice, got " << dice.size() << " throws" << std::endl;
	std::cout << "Dice result (1-6): " << dice << std::endl;
		
	const BigInt<256> privkey = getRandomFrom6SideDice(dice);		
		
		
	const PrivKey myPrivKey(privkey, compressed);
		
	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privkey, compressed);

	std::cout << "PUB: " << keypair.pubaddr << std::endl;
	//std::cout << "PRIV: " << keypair.privaddr << std::endl;

	BitsigConfig::storePrivateKey(keypair);
}







void generatePrivateFromRandom(const std::vector<std::string>& args)
{
	const bool compressed(false);

	std::cout << "Warning: Random number generated outside of bitsig with /dev/random." << std::endl;
	std::cout << "         Ensure that your random number generator gives good random numbers." << std::endl;
	std::cout << "         Flaws in the random number generation " << std::endl;
	std::cout << "         might make the private key possible to guess." << std::endl;
	std::cout << "         Consider other random number generators, such as dice." << std::endl;				
	
	std::cout << "Generating random bitcoin address" << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	
	const BigInt<256> privKey = getRandomNumber();	
		
	std::list<std::string> generationExtra;
	const BitcoinKeyPair keypair = generateRandomAddress(privKey, generationExtra, compressed);
		
		
	std::cout << "PUB: " << keypair.pubaddr << std::endl;
	//std::cout << "PRIV: " << keypair.privaddr << std::endl;
		
	BitsigConfig::storePrivateKey(keypair);
		
	const int numPrivate = BitsigConfig::numOwnKeypairs();
		
	std::cout << "There are now " << numPrivate << " private keys stored" << std::endl;
}



void deleteKey(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig delete publicKey [checksum]" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
		
	const std::string pubAddr = argv[2];
		
	const PubAddress myPubAddr(pubAddr);
	//Verify that the address is valid
	const BigInt<32> checksum = myPubAddr.getChecksum();		
		
		
	const bool privExist = BitsigConfig::hasPrivateForPublic(myPubAddr);
	const bool pubExist = BitsigConfig::hasPublic(myPubAddr);
	if(!privExist && !pubExist)
	{
		throw std::string("Does not have any key for this public address");
	}



	if(argc < 4)
	{
		std::cout << "Add the checksum in order to delete the address: " << checksum.strHex() << std::endl;
			
		//std::cout << "You must give hash" << std::endl;
		return;
	}

	const std::string submittedChecksum(argv[3]);
			
	if(submittedChecksum != checksum.strHex())
	{
		throw std::string("Incorrect given checksum");
	}
		
	std::cout << "OK, deleting address: " << pubAddr << std::endl;
		
	if(privExist)
	{
		BitsigConfig::deletePrivate(myPubAddr);
	}
	else
	{
		std::cout << "Deleting pub key" << std::endl;
		BitsigConfig::deletePublic(myPubAddr);		
	}
}





void setDefaultPrivatekey(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig setdef publicKeyForPrivateKey" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
		
	const std::string pubAddr = argv[2];
		
	const PubAddress myPubAddr(pubAddr);
	//Verify that the address is valid
	const BigInt<32> checksum = myPubAddr.getChecksum();		
		
		
	const bool privExist = BitsigConfig::hasPrivateForPublic(myPubAddr);
	if(!privExist)
	{
		throw std::string("Does not have private key for this public address");
	}
				
	std::cout << "Setting default private key for " << pubAddr << std::endl;
	BitsigConfig::setDefaultKey(myPubAddr);
}



//Todo: Use in bitsig as well
void listKeys(int argc, char* argv[])
{
	std::cout << "Public addresses for own private keys used for signing or decryption: " << std::endl;
		
	std::cout << std::endl;
	BitsigConfig::listOwnPublicAddresses();

	std::cout << std::endl;
	std::cout << std::endl;
		
	std::cout << "Public keys from others (without any private key)" << std::endl;
	std::cout << " used for encryption or signature verification: " << std::endl;
	std::cout << std::endl;	
	BitsigConfig::listPublicAddresses();
	
	std::cout << std::endl;
}		




void listOwnPublicKeys()
{		
	std::string defPrivContent;
	const std::set<BitcoinKeyPair> ownPairs = BitsigConfig::getOwnKeypairs(defPrivContent);
	if(ownPairs.empty())
	{
		std::cout << "No own public keys exist" << std::endl;
		return;
	}
		
	std::cout << "Stored own public keys: " << std::endl;
		
	std::cout << "                       address                               PKIF" << std::endl;

	BitsigConfig::listOwnPublicKeys();
}




void listPrivateKeys()
{		
	std::string defPrivContent;
	const std::set<BitcoinKeyPair> ownPairs = BitsigConfig::getOwnKeypairs(defPrivContent);
	if(ownPairs.empty())
	{
		std::cout << "No private keys exist" << std::endl;
		return;
	}
		
	std::cout << "Stored private addresses: " << std::endl;
		
	std::cout << "WARNING -- PRIVATE KEYS -- DO NOT SHARE" << std::endl;
	std::cout << "                       public                               private" << std::endl;

	BitsigConfig::listPrivateAddresses();
	std::cout << "WARNING -- PRIVATE KEYS -- DO NOT SHARE" << std::endl;
}



void listOwnKeys(const std::vector<std::string>& args)
{		
		//std::cout << "Args size: " << args.size() << std::endl;
		if(args.size() >= 2)
		{
			const std::string extraArg(args[1]);
			if(extraArg == "private")
			{
				listPrivateKeys();
			}
			if(extraArg == "public")
			{
				listOwnPublicKeys();
			}			
			else
			{
				std::cout << "Unknown: " << extraArg << std::endl;
			}
		}
		else
		{
			std::cout << "Own public addresses: " << std::endl;
		
			BitsigConfig::listOwnPublicAddresses();
		
			std::cout << std::endl;
		}
}



void listThePublicKeys(const std::vector<std::string>& args)
{
	std::cout << "Stored public keys: " << std::endl;
	std::cout << "Num       PKIF                                         Owner" << std::endl;
	std::cout << "===============================================================" << std::endl;
		
	BitsigConfig::listPublicKeys /*sddresses*/();
}





void listPublicAddresses(const std::vector<std::string>& args)
{		
		bool listOnlyNames(false);
		
		const std::vector<std::string> allExtraArgs = getExtraArgs(args, 0);
		for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
			it != allExtraArgs.end() ;
			++it)
		{
			const std::string& arg = (*it);
			if(arg == "name")
			{
				listOnlyNames = true;
			}
		}
		
		if(listOnlyNames)
		{
			std::cout << "Listing only names" << std::endl;
			BitsigConfig::listPublicAddresses(true);
		}
		else
		{
			std::cout << "Stored public addresses: " << std::endl;
			std::cout << "Num       address                      Owner" << std::endl;
			std::cout << "====================================================" << std::endl;
		
			BitsigConfig::listPublicAddresses();
		}
}



/*
void listPublicKeys(const std::vector<std::string>& args)
{		
		bool listOnlyNames(false);
		
		const std::vector<std::string> allExtraArgs = getExtraArgs(args, 0);
		for(std::vector<std::string>::const_iterator it = allExtraArgs.begin() ;
			it != allExtraArgs.end() ;
			++it)
		{
			const std::string& arg = (*it);
			if(arg == "name")
			{
				listOnlyNames = true;
			}
		}
		
		if(listOnlyNames)
		{
			std::cout << "Listing only names" << std::endl;
			BitsigConfig::listPublicAddresses(true);
		}
		else
		{
			std::cout << "Stored public keys: " << std::endl;
			std::cout << "Num       address                    Owner" << std::endl;
			std::cout << "====================================================" << std::endl;
		
			BitsigConfig::listPublicKeys();
		}
}
*/




void importPrivateKey(int argc, char* argv[])
{
	std::cout << "Importing private key" << std::endl;
		
	if(argc < 3)
	{
		std::cout << "Usage: bitsig importprivate wif" << std::endl;
		std::cout << "             or" << std::endl;
		std::cout << "       bitsig importprivate -" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
		
	//const 
	std::string wif(argv[2]);	

	if(wif == "-")
	{
		std::cout << "Enter the wif" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		wif = readStdinLine(30, false, isBase58Char);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;
	}
	
	
	const Wif myWif(wif);
		
	std::cout << "Private key: " << myWif << std::endl;
		
	const PrivKey myPrivKey = myWif.getPriv();
	
	//bool compressed(false);
	//const BigInt<256> privKey = myWif.wifToPrivKey(compressed);
	//const PrivKey myPrivKey(privKey, compressed);

	const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);

	std::cout << "PUB: " << keypair.pubaddr << std::endl;
	std::cout << "PRIV: " << keypair.privaddr << std::endl;

	BitsigConfig::storePrivateKey(keypair);		
}







void addPublicKey(const PubKey& pubKey, 
	const std::string& submittedChecksum, const std::string& name)
{
	//Verify that the address is valid
	const std::string checksum = pubKey.getChecksum();
	
	const std::string checksumHex = hexString(checksum);
	if(submittedChecksum.empty())
	{
		std::cout << "Add the checksum in order to store the public key: " << checksumHex << std::endl;
		return;			
	}
					
	if(submittedChecksum == checksumHex)
	{
		std::cout << "OK, storing" << std::endl;
		std::cout << "public key: " << pubKey.toString() << std::endl;
		std::cout << "for name: " << name << std::endl;
			
		const Pkif pkif = pubKey.getPkif();
		BitsigConfig::addPublicKey(pkif, name);
	}
	else
	{
		std::cout << "Incorrect checksum given, use: " << checksumHex << std::endl;
	}	
}



void addPublicBitcoinAddress(const PubAddress& myPubAddr, 
	const std::string& submittedChecksum, const std::string& name)
{
	//Verify that the address is valid
	const BigInt<32> checksum = myPubAddr.getChecksum();
	const std::string hexSumAsString = hexString(checksum.getBinaryLong());
	
	if(submittedChecksum.empty())
	{
		std::cout << "Add the checksum in order to store the public key: " << hexSumAsString
		/*checksum.strHex()*/ << std::endl;
		return;			
	}
	
		
	//const std::string submittedChecksum(argv[4]);
			
	if(submittedChecksum == hexSumAsString) //checksum.strHex())
	{
		std::cout << "OK, storing" << std::endl;
		std::cout << "Address: " << myPubAddr.toString() /*.address*/  << std::endl;
		std::cout << "for name: " << name << std::endl;
		
		BitsigConfig::addPublicAddress(myPubAddr, name);
	}
	else
	{
		std::cout << "Incorrect checksum given, use: " << checksum.strHex() << std::endl;
	}
}




void importPublicKey(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cout << "Usage: bitsig import pubaddr name [checksum]" << std::endl;
		//std::cout << "Example: bitsig import 1M1y6rTCc6Q55rfug1X9aFc2g3eSkqvi6V Bob C9162FEA" << std::endl;
		std::cout << "Omit the checksum in order to get the checksum" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
		
	const std::string inPubAddr(argv[2]);	
	const std::string name(argv[3]);

	std::string submittedChecksum; //(argv[4]);

	if(argc >= 5)
	{
		submittedChecksum = argv[4];
	}

	std::string bitcoinAddress;
	//Is this an address or a PKIF?
	if(PubAddress::isAddress(inPubAddr))
	{
		std::cout << "This is a bitcoin address" << std::endl;
		bitcoinAddress = inPubAddr;

		const PubAddress myPubAddr(bitcoinAddress);

		addPublicBitcoinAddress(myPubAddr, submittedChecksum, name);		
		return;
	}
	else if(Pkif::isPkif(inPubAddr))
	{
		std::cout << "This is a public key" << std::endl;
		
		//bool compressed(false);
		//const Pkif pkif(inPubAddr); // = Pkif::parsePkif(inPubAddr);
		const PubKey pubKey = PubKey::fromPkif(inPubAddr);
		
		//const ECPoint p = Pkif::parsePkif(inPubAddr, compressed);
		//const Pkif pkif(p, compressed);
		
		addPublicKey(pubKey, submittedChecksum, name);

		//const PubAddress a = PubAddress::bitcoinAddresFromPubkey(p, compressed);
		//std::cout << "Corresponding bitcoin address: " << a.address << std::endl;
		//bitcoinAddress = a.address;
	}
	else
	{
		std::cout << "Incorrect format: " << inPubAddr << std::endl;
		throw std::string("Error, incorrect format");
	}


	//const PubAddress myPubAddr(bitcoinAddress);

	//addPublicBitcoinAddress(myPubAddr, submittedChecksum, name);		
}		
		



