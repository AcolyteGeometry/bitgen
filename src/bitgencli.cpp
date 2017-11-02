/**
 * bitgencli.cpp - Bitcoin address generator
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
 

#include <iostream>
#include <stdlib.h>

#include "bigint.h"
#include "sha2.h"
#include "ripemd160.h"
#include "bitcoinEC.h"
#include "qrenc.h"
#include "util.h"
#include "FileUtil.h"

#include <unistd.h>

//Todo: Remove dependencies
#include <ctime>
#include <dirent.h>

#include <set>
#include <map>
#include <stdio.h>


#include "hash.h"
#include "base58.h"
#include "addressConversion.h"
#include "signature.h"
#include "sigFile.h"
#include "random.h"
#include "bitcoinKeyPair.h"
#include "randomUtils.h"
#include "bitgenConfig.h"
#include "language.h"
#include "printPS.h"
#include "mnemonic.h"
#include "miniPriv.h"
#include "unittest.h"
#include "privKey.h"
#include "pubKey.h"
#include "version.h"

#include "commandLine.h"
#include "keyDerivationFunction.h"
#include "RangeSearcher.h"
#include "HierarchialRandom.h"
#include "txtGeneration.h"
#include "currencySpecific.h"
#include "Directory.h"
#include "license.h"
#include "SaveFolderHandler.h"
#include "GenerationParameters.h"
#include "predicates.h"
#include "usagePrintout.h"
#include "GenerationMethods.h"
#include "VanitySearcher.h"
#include "bitlibVersion.h"
#include "multiSig.h"
#include "timelock.h"
#include "transaction.h"
#include "transactiondecoder.h"
#include "transactionSignatureChecker.h"
#include "binaryTransaction.h"
#include "ExtendedKey.h"
#include "hmac.h"


Language getLanguage()
{
	const std::string constPath = getConfigPath();
	
	const std::string confLang = getConfiguredLang();
	
	//std::cout << "Using lang: " << confLang << std::endl;
	
	if(confLang == "en")
	{
		return LANG_EN;		
	}
	else if(confLang == "de")
	{
		return LANG_DE;
	}
	
	
	return LANG_EN;
}











void generateRootKey(const std::string& theTime, int argc, char* argv[])
{
	std::string label;
	
	if(argc < 3)
	{
		std::cout << "Generation method not given. For example random, hash, etc" << std::endl;
		throw std::string("Generation method must be given");
	}
	
	const std::string genMethod = argv[2];
	std::cout << "Generation method: " << genMethod << std::endl;
	

	
	if(genMethod == "random")
	{
		const bool compressed(true);	//C!
	if(argc >= 4)
	{
		label = argv[3];
		std::cout << "Label: " << label << std::endl;
	}
	
	GenerationMethods::printRandomSourceWarning();
		
	std::cout << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
		

	//Todo: Use other random sources as well
	const std::string random = getFromDevRandom(32);
		
	const BigInt<256> privKeyR(random, 'c');
	generateHierarchialRootKey(privKeyR, theTime, label, compressed);
	}
	else if(genMethod == "dice")
	{
		const bool compressed(true);	//C!
		if(argc < 4)
		{
			std::cout << "Usage: bitgen genRoot dice digits(1-6)" << std::endl;
			return;			
		}
		
		const std::string dice(argv[3]);
		if(dice.size() != 100)
		{
			std::string errMsg("Need exactly 100 dice throws, got: ");
			errMsg += intToString(dice.size());
			errMsg += " throws";
			
			throw errMsg;
			
			//std::cerr << "Need exactly 100 dice throws, got: " << dice.size() << " throws" << std::endl;
			//return;
		}

		//std::string label;
		if(argc > 4)
		{
			label = argv[4];
		}

		std::cout << "Generating bitcoin address from dice, got " << dice.size() << " throws" << std::endl;
		std::cout << "Dice result (1-6): " << dice << std::endl;
		
		const BigInt<256> privKeyR = getRandomFrom6SideDice(dice);		
		generateHierarchialRootKey(privKeyR, theTime, label, compressed);
	}
	else
	{
		std::cout << "Unknown generation method: " << genMethod << std::endl;
	}	
}






void generatePublicFromRoot(int argc, char* argv[], const std::string& thetime)
{		
	const bool compressed(true);	//C!
	
	bool printAll(false);

	if(argc < 3)
	{
		throw std::string("pubroot filename must be given");
	}
		
	std::string pubRootFile = argv[2];

	if(!endsWith(pubRootFile, ".pubroot"))
	{
		throw std::string("Error, file name must end with .pubroot");
	}


	std::string accountName;
	if(argc > 3)
	{
		accountName = argv[3];
		std::cout << "Account name: " << accountName << std::endl;
	}



	std::cout << "Getting random number" << std::endl;
	const std::string random = getFromDevRandom(32);
		
	const BigInt<256> keyInd(random, 'c');
	const BigInt<257> keyIndex(BigInt<257>::asPositive(keyInd));
		
	//PrivKey priv(privKeyR);

	//std::cout << "Index: " << keyIndex << std::endl;

	//std::string privFileContent(hexString(privKeyR.getBinaryVal()) + "\n" + keypair.privaddr.address);

		
	//std::cout << "Reading: " << pubRootFile << std::endl;
	const std::string pubStr = readFile(pubRootFile);
		 
	 //Parse the content
	const int newlinepos = pubStr.find("\n");
	if(newlinepos == std::string::npos)
	{
		throw std::string("Could not parse file, no newline found");
	}
	const std::string pubkeyX = pubStr.substr(0, newlinepos);
		
		
	//std::cout << "The pub key X string: " << pubkeyXHex << std::endl;
		
		
	const BigInt<1024> pubKeyXRoot = BigInt<1024>::fromHex(pubkeyX);
	//std::cout << "PubX val: " << pubKeyXRoot << std::endl;


	const int newlinepos2 = pubStr.find("\n", newlinepos + 1);
	if(newlinepos2 == std::string::npos)
	{
		throw std::string("Could not parse file, newline #2 not found");
	}
	const int yStart = newlinepos + 1;
	
	
	//Todo: Use compressed format, store only X
	
	const std::string pubkeyY = pubStr.substr(yStart, newlinepos2 - yStart);
	//std::cout << "Ykoord: " << pubkeyY << std::endl;		
	//std::cout << "The pub key Y string: " << pubkeyYHex << std::endl;
		
		
	const BigInt<1024> pubKeyYRoot = BigInt<1024>::fromHex(pubkeyY);
	//std::cout << "PubY val: " << pubKeyYRoot << std::endl;


	BitcoinEllipticCurve myec;

	ECPoint PublicKeyRoot = myec.fromCoordinates(pubKeyXRoot, pubKeyYRoot);

	//ECPoint PublicKeyRoot(pubKeyXRoot, pubKeyYRoot);

	//std::cout << "The root public key: " << std::endl << PublicKeyRoot << std::endl;


	//const PubAddress rootPub = pubPointTob58(PublicKeyRoot);
	const PubKey mypub(PublicKeyRoot, compressed);
	const PubAddress rootPub = mypub.getAddress();
	
	//const PubAddress rootPub = PubAddress::bitcoinAddresFromPubkey(PublicKeyRoot, compressed);
		
	const std::string digest = sha256(rootPub.toString()).substr(0,4);
		
	const std::string currencyShortName = getCurrencyShortName();
		
	const std::string filePrefix = std::string("root") + currencyShortName + "_" + hexString(digest);
	//std::cout << "filePrefix: " << filePrefix << std::endl;


	const BigInt<256> privAdd(keyIndex);
	const PrivKey myPrivKey(privAdd, compressed);
	const ECPoint PrivKeyAdd = myPrivKey.point(myec);
			
	const ECPoint myPublic = myec.add(PublicKeyRoot, PrivKeyAdd);
		
	const PubKey thepub(myPublic, compressed);
	const PubAddress pub = thepub.getAddress();
		
	//const PubAddress pub = PubAddress::bitcoinAddresFromPubkey(myPublic, compressed);
		

	std::cout << "PUB: " << pub << std::endl;

	const std::string fileName(pub.toString() + ".privindex");
	std::cout << "Will save file: " << fileName << std::endl;
	fileWrite(fileName, keyIndex.strHex());


	//const std::list<std::string> not_used_printExtra;

	const Language lang = getLanguage();
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang);
		
	const PublicPostscriptInfo pubInfo(pub.toString(), thetime, accountName);
		

	const HierarcialInfo hierarcialInfo(hexString(digest), pubkeyX, pubkeyY);	

	//postscriptGenerator.savePrivateHierarcialFile(hexString(digest), 
	//	hexString(privAdd.getBinaryLong()), pub.toString(), pubInfo, hierarcialInfo);
			
	const std::string psPublic = postscriptGenerator.privateHierarcialPrivate(
				/*hexString(digest),*/ hexString(privAdd.getBinaryLong()), pubInfo, hierarcialInfo);
	
	const std::string pubFilename =  pubInfo.pubkey + "_priv.ps";
	std::cout << "Saving privfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);
			
}







void generatePrivateFromRoot(int argc, char* argv[], const std::string& thetime)
{		
	const bool compressed(true);	//C!
		bool printAll(false);

		if(argc < 3)
		{
			std::cout << "Usage: bitgen hipriv privrootfile privindexfile" << std::endl;
			throw std::string("Priv root filename must be given");
		}
		
		std::string privateRootFile = argv[2];

		if(!endsWith(privateRootFile, ".privroot"))
		{
			throw std::string("Error, file name must end with .privroot");
		}


		if(argc < 4)
		{
			std::cout << "Usage: bitgen hipriv privrootfile privindexfile" << std::endl;			
			throw std::string("privindex filename must be given");
		}


		const std::string privIndexFile = argv[3];
		if(!endsWith(privIndexFile, ".privindex"))
		{
			throw std::string("Error, index file must have extension .privindex");
		}

		//const int keyIndex = atoi(argv[3]);
		//std::cout << "Using index: " << keyIndex << std::endl;
		//const int keyIndex(0);

		//std::string privFileContent(hexString(privKeyR.getBinaryVal()) + "\n" + keypair.privaddr.address);

		
		std::cout << "Reading: " << privateRootFile << std::endl;
		const std::string privStr = readFile(privateRootFile);
		 
		 //Parse the content
		const int newlinepos = privStr.find("\n");
		if(newlinepos == std::string::npos)
		{
			throw std::string("Could not parse file, no newline found");
		}
		const std::string privkey = privStr.substr(0, newlinepos);
		
		//std::cout << "The priv key string: " << privkeyHex << std::endl;
		
		
		const BigInt<256> privKeyRoot = BigInt<256>::fromHex(privkey);
		//const BigInt<256> privKeyR(privkeyHex);
		std::cout << "Rootpriv: " << hexString(privKeyRoot.getBinaryLong()) << std::endl;


		//Read the index file
		const std::string indexStr = readFile(privIndexFile);
		std::cout << "Index file content: " << indexStr << std::endl;
		//std::cout << "Will convert: " << indexHex << std::endl;
		
		const BigInt<257> keyIndex = BigInt<257>::fromHex(indexStr);



		BigInt<257> N(BitcoinEllipticCurve::Nval());


		//todo
		const BigInt<257> privKeyRootLarger(BigInt<257>::asPositive(privKeyRoot));
		const BigInt<257> privKeyTotal = privKeyRootLarger + keyIndex;
		const BigInt<257> privKeyMod = privKeyTotal % N; 	//Modulus
		const BigInt<256> privKey(privKeyMod);
		
		const PrivKey myPrivKey(privKey, compressed);
		
		
		//const BigInt<256> privKey = privKeyRoot + keyIndex; //Todo: Modulus!!!!!!!
		const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(myPrivKey); //privKey, compressed);

		std::cout << "PRIV: " << keypair.privaddr << std::endl;			
		std::cout << "PUB: " << keypair.pubaddr << std::endl;

		std::list<ExtraInfo> generationExtra;

		std::string generationMethod("hierarchial random ");
		//generationMethod += privateRootFile;

		//const std::string mnemonic = generateMnemonic(privKey);
		const Mnemonic myMnemonicObj(privKey); // = Mnemonic::generate(privKey);
		//const std::string mnemonic = myMnemonicObj.toString();


		GenerationParameters params(compressed, true, true);

		GenerationMethods::saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), generationMethod, 
						generationExtra, "", params, myMnemonicObj);
}


































void giveInfoAboutWif(const std::string& wif)
{
	const Wif myWif(wif);
	
	const PrivKey myPrivKey = myWif.getPriv();

	std::cout << "Format is     : " << (myPrivKey.compressed ? "compressed" : "non-compressed") << std::endl;
	std::cout << "WIF           : " << myWif << std::endl;
	
	std::cout << "Private key   : " << hexString(myPrivKey.key.getBinaryLong()) << std::endl;

	BitcoinEllipticCurve ec;

	const PubKey pubKey = myPrivKey.getPubkey(ec);
	const ECPoint publicPoint = pubKey.point;
	std::cout << "Public point  :(" << publicPoint.x << std::endl;
	std::cout << "              : " << publicPoint.y << ")" << std::endl;
	std::cout << "Public key    : " << hexString(publicPoint.getRaw(myPrivKey.compressed)) << std::endl;
	std::cout << "PKIF          : " << pubKey.toString() << std::endl;

	
	PubAddress address = pubKey.getAddress();
	std::cout << "Public address: " << address << std::endl;


	std::cout << std::endl;
	std::cout << "Validate OK" << std::endl;
}







void giveInfoAboutPubKey(const PubKey& pubkey)
{
	std::cout << "Pkif          : " << pubkey.toString() << std::endl;	
	std::cout << "Format is     : " << (pubkey.compressed ? "compressed" : "non-compressed") << std::endl;

	std::cout << "Use           : ";
	if(!pubkey.onlyIndicatedUsage)
	{
		std::cout << "Any" << std::endl;
	}
	else
	{
		std::cout << (pubkey.signVerify ? "Signature verification" : "Encrypt") << std::endl;		
	}
	
	
	
	std::cout << "Public point  :(" << pubkey.point.x << std::endl;
	std::cout << "              : " << pubkey.point.y << ")" << std::endl;
	std::cout << "Public key    : " << hexString(pubkey.point.getRaw(pubkey.compressed)) << std::endl;
	std::cout << "Public address: " << pubkey.getAddress() << std::endl;
}


void giveInfoAboutPkif(const std::string& pkifStr)
{
	const PubKey pubkey = PubKey::fromPkif(pkifStr);
	 giveInfoAboutPubKey(pubkey);
}


void giveInfoAboutPublicKey(const std::string& entity)
{
	const PubKey pubkey = PubKey::fromHex(entity);
	giveInfoAboutPubKey(pubkey);
}



void giveInfoAboutAddress(const std::string& entity)
{
	const PubAddress address(entity);	
	//address.validate();
	std::cout << "Validate OK" << std::endl;
	const BigInt<200> decoded(address.getNumeric());
	std::cout << "Digits    : 1         2         3         4         5         6" << std::endl;
//todo
	std::cout << "Numeric   : " << hexStrFull(decoded.getBinaryLong()) << std::endl;
	
	const BigInt<168> fourBytesDropped(decoded >> 32);
	std::cout << "NoChecksum: " << hexStrFull(fourBytesDropped.getBinaryLong()) << std::endl;

	const BigInt<168> hashVal(fourBytesDropped & 160);
	const BigInt<160> hashVal160(hashVal);
	
	std::cout << "Hash160   :   " << hexStrFull(hashVal160.getBinaryLong()) << std::endl;


	const BigInt<168> netVersion = fourBytesDropped >> 160;		//160 bit hash
	std::cout << "NetVersion: " << netVersion << std::endl;
	
}




PubKey stringToPubkey(const std::string pkifStr)
{
	if(Pkif::isPkif(pkifStr))
	{
		//std::cout << "OK, pkif: " << pkifStr << std::endl;
		const Pkif pkif(pkifStr);
		const PubKey pubKey = pkif.getPubKey();
		return pubKey;
	}
	else if(PubKey::isPubkey(pkifStr))
	{
		//std::cout << "OK, pubkey: " << pkifStr << std::endl;			
		const PubKey pubKey = PubKey::fromHex(pkifStr);
		return pubKey;		
	}
	else
	{
		std::cout << "Not a pkif or pubkey: " << pkifStr << std::endl;
		throw std::string("PKIF error");
	}
}



std::list<PubKey> getSortedPkifs(std::list<std::string> pkifStrings)
{
	std::set<PubKey> mypkifSet;
	
	for(std::list<std::string>::const_iterator it = pkifStrings.begin() ;
		it != pkifStrings.end() ;
		++it)
	{
		const std::string pkifStr = (*it);		

		const PubKey pubKey = stringToPubkey(pkifStr);	
		mypkifSet.insert(pubKey);		
	}
	
	std::list<PubKey> pkifList;
	
	int num(1);
	for(std::set<PubKey>::const_iterator it = mypkifSet.begin() ;
		it !=  mypkifSet.end() ;
		++it)
	{
		const PubKey& pubKey = (*it);
		std::cout << "Pubkey #" << num++ << ": " << pubKey.toString() << std::endl;;
		std::cout << "" << pubKey.toStringHex() << std::endl;

		pkifList.push_back(pubKey);
	}
	
	
	if(pkifStrings.size() != pkifList.size())	//Check if there are duplicates
	{
		throw std::string("Incorrect number public keys");
	}
	
	return pkifList;
}









bool transactionSupportedForCurrency()
{
	const std::string currency = getCurrencyShortName();
	if(currency == "ltc" || currency == "btc" || currency == "btc" || currency == "doge" || 
		currency == "vtc" || currency == "nmc"  || currency == "ftc")
	{
		return true;
	}
	
	return false;
}




void createTimelock(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		const std::string currency = getCurrencyShortName();
		std::cout << "Timelock not supported for currency: " << currency << std::endl;
		throw std::string("Not supported");
	}
	
	//std::cout << "createTimelock" << std::endl;

	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen timelock blockheight pkif" << std::endl;
		std::cout << "Creates timelocked script" << std::endl;
		return;
	}
		
	std::string blockHeightStr(args[1]);
	const int blockHeight = atoi(blockHeightStr.c_str());
	std::cout << "Blockheight: " << blockHeight << std::endl;

	const std::string pkifStr = args[2];
	
	const PubKey pubKey = stringToPubkey(pkifStr);

	const BinaryScript script = createTimelockScript(blockHeight, pubKey);
	std::cout << "Redeem script: " << script.hex() << std::endl;

	
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script);

	std::cout << std::endl;
	std::cout << "The timelock address: " << pubAddress.toString() << std::endl;			
	
	const std::string scriptFilename = "redeem_timelock_" + getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
	Script::saveScriptWithFilename(script, scriptFilename);	
}










std::string createMultisigTxtFile(const int numReq, const std::list<PubKey>& mypkifSet, 
	const PubAddress& pubAddress, const std::string& script)
{
	std::string fileContent;
	fileContent += "Type    : Multisig ";
	//fileContent += "Type   : ";
	fileContent += intToString(numReq);
	fileContent += "/";
	fileContent += intToString(mypkifSet.size());
	fileContent += "\n";

	fileContent += "Address : ";
	fileContent += pubAddress.toString();
	fileContent += "\n";

	fileContent += "\n";
	
	int num(1);
	for(std::list<PubKey>::const_iterator it = mypkifSet.begin() ;
		it != mypkifSet.end() ;
		++it)
	{
		const PubKey& pk = (*it);
		const PubAddress& pubAddress = pk.getAddress();
		fileContent += "Pubkey ";
		fileContent += intToString(num++);		
		fileContent += ": ";
		fileContent += pk.toString();
		fileContent += " ";
		fileContent += pubAddress.toString();
		fileContent += "\n";
	}
	
	fileContent += "\n";
	
	fileContent += "Redeem script: ";
	fileContent += hexString(script);
	fileContent += "\n";

	return fileContent;
}




void createMultisig(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Create multisig not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	//std::cout << "createMultisig" << std::endl;

	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen multisig numReq NumTotal pkif1 ... pkifn" << std::endl;
		return;
	}
		
	std::string numReqStr(args[1]);
	const int numReq = atoi(numReqStr.c_str());
	//std::cout << "Num required: " << numReq << std::endl;

	std::string numTotalStr(args[2]);
	const int numTotal = atoi(numTotalStr.c_str());
	
	//std::cout << "Num total: " << numTotal << std::endl;
	if(numReq > numTotal)
	{
		std::cout << "Required more than total" << std::endl;
		throw std::string("Required can not be more than total");		
	}
	
	
	if(args.size() <= 2 + numTotal)
	{
		std::cout << "Pkif missing" << std::endl;
		std::cout << "Usage: bitgen multisig numReq NumTotal pkif1 ... pkifn" << std::endl;
		return;
	}
	
	std::list<std::string> pkifList;
	for(int i = 0 ; i < numTotal ; i++)
	{
		const std::string pkif = args[3 + i];
		//std::cout << "Found pkif/pubkey: " << pkif << std::endl;
		
		pkifList.push_back(pkif);
	}
	
	std::cout << std::endl;
	std::cout << "CREATING MULTISIG ADDRESS " << numReq << "/" << pkifList.size() << std::endl;
	

	
	const std::list<PubKey> mypkifSet = getSortedPkifs(pkifList);	

	
	const BinaryScript script = ScriptCreator::createMultiSigScript(numReq, mypkifSet);	



	std::cout << std::endl;
	std::cout << "Redeem script: " << script.hex() << std::endl;

	std::cout << std::endl;


	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(script);

	std::cout << std::endl;
	std::cout << "The multisig address: " << pubAddress.toString() << std::endl;
	
	std::string fileContent = createMultisigTxtFile(numReq, mypkifSet, pubAddress, script.raw());
	
	const std::string filename = "multi_" + pubAddress.toString() + "_pub.txt";
	
	std::cout << "Writing file: " << filename << std::endl;
	fileWrite(filename, fileContent);
	
	//Save script
	const std::string scriptFilename = "multi_redeem_" +  getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
	Script::saveScriptWithFilename(script, scriptFilename);
	
}





void informationAboutEntity(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen info wif/pkif/address" << std::endl;
		return;
	}
		
	std::string entity(args[1]);
	
	bool unsafeInput(false);
	if(entity == "-")
	{
		std::cout << "Enter the entity value at one row" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		entity = readStdinLine(10, false, isBase58Char);
	}
	else
	{
		unsafeInput = true;
	}
		
	

	//Check which kind of entity
	if(BitgenPredicate::isWif(entity))
	{
		//It is a wif
		if(unsafeInput)
		{
			std::cout << "WARNING: The information entered at the command line" << std::endl;
			std::cout << "WARNING: might be stored in a shell history file." << std::endl;
			std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;	
			std::cout << std::endl;		
		}
		
		std::cout << "Entity type: WIF" << std::endl;
		giveInfoAboutWif(entity);
	}
	else if(BitgenPredicate::isPkif(entity))
	{		
		std::cout << "Entity type   : PKIF" << std::endl;
		giveInfoAboutPkif(entity);
	}
	else if(BitgenPredicate::isPublicKey(entity))
	{		
		std::cout << "Entity type   : Public key" << std::endl;
		giveInfoAboutPublicKey(entity);
	}	
	else if(BitgenPredicate::isAddress(entity))
	{		
		std::cout << "Entity type   : Address" << std::endl;
		giveInfoAboutAddress(entity);
	}
	else
	{
		//std::cout << "Unknown entity: " << entity << std::endl;
		std::string errMsg("Unknown entity: ");
		errMsg += entity;
		throw errMsg;
	}



	
}



void validatePrivateWif(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen infoPrivate wif" << std::endl;
		return;
	}
		
	//const
	std::string wif(argv[2]);
	if(wif == "-")
	{
		std::cout << "Enter the wif value at one row" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		wif = readStdinLine(10, false, isBase58Char);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;	
		std::cout << std::endl;		
	}
		
	

		
	const Wif myWif(wif);
		

	const PrivKey thePrivKey = myWif.getPriv();	
	const BigInt<257> privKeyLong(thePrivKey.key);

	BitcoinEllipticCurve ec;

	const PubKey pubKey = thePrivKey.getPubkey(ec);
	const Pkif pkif = pubKey.getPkif();

	std::cout << "Format is     : " << (thePrivKey.compressed ? "compressed" : "non-compressed") << std::endl;
	std::cout << "WIF           : " << myWif << std::endl;
	
	std::cout << "Private key   : " << hexString(privKeyLong.getBinaryLong())  << std::endl;

	const ECPoint publicKey = pubKey.point;
	std::cout << "Public point  :(" << publicKey.x << std::endl;
	std::cout << "              : " << publicKey.y << ")" << std::endl;
	std::cout << "Public key    : " << pubKey.toStringHex() << std::endl;
	std::cout << "PKIF          : " << pkif.toString() << std::endl;

	const PubAddress address = pubKey.getAddress();
	std::cout << "Public address: " << address << std::endl;


	std::cout << std::endl;
	std::cout << "Validate OK" << std::endl;
}




void verifyAddress(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen infoAddress bitcoinAddress" << std::endl;
		return;			
	}
	
	const std::string adr(argv[2]);
	const PubAddress myPub(adr);
	//myPub.validate();
	std::cout << "Validate OK" << std::endl;
}








void generateWifFromMiniPrivKey(int argc, char* argv[], const std::string& thetime)
{		
	const bool compressed(false);	
	
	if(argc < 3)
	{
		std::cout << "Usage: bitgen miniPrivKey MiniPrivateKey" << std::endl;
		throw std::string("Missing argument");
	}
		
	std::cout << "Generating bitcoin address from mini private key" << std::endl;

	
	//const 
	std::string miniAddr(argv[2]);
	if(miniAddr == "-")
	{
		std::cout << "Enter the wif value at one row" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		miniAddr = readStdinLine(30, false, isBase58Char);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;		
	}
	
	
	std::cout << "Will convert mini address: " << miniAddr << std::endl;

	if(!isValidMiniPriv(miniAddr))
	{
		throw std::string("Incorrect mini addr");
	}

	//We now know the format is OK. Perform the conversion to standard key
	const BitcoinKeyPair keypair = convertMiniPriv(miniAddr, compressed);
		
		
	std::list<ExtraInfo> generationExtra;
	const ExtraInfo extraInfo("Mini private key", miniAddr);
	generationExtra.push_back(extraInfo);	
	//generationExtra.push_back("Mini private key: " + miniAddr);

	const PrivKey pk = keypair.privaddr.getPriv();
	//const std::string mnemonic = generateMnemonic(pk.key);
	const Mnemonic myMnemonicObj(pk.key); // = Mnemonic::generate(pk.key);
	//const std::string mnemonic = myMnemonicObj.toString();


	std::string accountName;
	
	GenerationParameters params(compressed, true, true);
	GenerationMethods::saveAllFiles(keypair.pubaddr.toString(), keypair.privaddr.toString(), "Mini private key", 
		generationExtra, thetime, params, myMnemonicObj);
}
		


/*
void handleVanityFound(const long int startTime, const int i)
{
			const long int endTime = time(NULL);
			const long int totalTime = (endTime - startTime);
			const int iterationsPerSec = (totalTime == 0) ? 0 : (i / totalTime);
			
			std::cout << std::endl;
			std::cout << "Found solution: " << pub << std::endl;
			std::cout << " while searching for: " << searchPattern << std::endl;
			if(iterationsPerSec != 0)
			{
				std::cout << "Iterations per second: " << iterationsPerSec << std::endl;
			}
			std::cout << std::endl;

			const BigInt<257> N(BitcoinEllipticCurve::N());
				
			const BigInt<257> privKeyRLarger(privKeyR);
			const BigInt<257> privKeyTotal = privKeyRLarger + i;
			const BigInt<257> privKeyMod = privKeyTotal % N; 	//Modulus
			const BigInt<256> privKey(privKeyMod);
				
			const BitcoinKeyPair keypair = BitcoinKeyPair::calcKeys(privKey, compressed);

			std::cout << "PRIV: " << keypair.privaddr << std::endl;			
			std::cout << "PUB : " << keypair.pubaddr << std::endl;
				
			std::list<std::string> generationExtra;
				
			const std::string mnemonic = generateMnemonic(privKey);
				
				
			saveAllFiles(keypair.pubaddr.address, keypair.privaddr.address, "vanity", 
				generationExtra, thetime, accountName, mnemonic, compressed);			
}
*/









void generateVanityPoolResult(int argc, char* argv[], const std::string& thetime, const bool usePool)
{
	if(argc < 4)
	{
		std::cout << "Usage: bitgen vanityPoolResult wif1 wif2 [Name]" << std::endl;
		throw std::string("Incorrect usage");
	}

	const std::string wifStr1 = argv[2];
	const std::string wifStr2 = argv[3];


	std::string accountName;
	if(argc > 4)
	{
		accountName = argv[4];
	}
	
	const Wif wif1(wifStr1);
	const Wif wif2(wifStr2);

	const PrivKey privKey1 = wif1.getPriv();

	const BigInt<1024> N(BitcoinEllipticCurve::Nval());

	const PrivKey pk1 = wif1.getPriv();
	const PrivKey pk2 = wif2.getPriv();
	
	const BigInt<1024> priv1(BigInt<1024>::asPositive(pk1.key));
	const BigInt<1024> priv2(BigInt<1024>::asPositive(pk2.key));

	
	const BigInt<1024> sum = priv1 + priv2;
	const BigInt<1024> modsum = sum % N;
	
	const BigInt<256> privKey(modsum);
				
	const PrivKey myPrivKey(privKey, privKey1.compressed);
	const Wif wif = myPrivKey.getWif();
	
	BitcoinEllipticCurve ec;
	
	const PubAddress address = myPrivKey.getAddress(ec);
	
	std::cout << std::endl;
	std::cout << "Wif: " << wif << std::endl;	
	std::cout << "Address: " << address << std::endl;
	
	std::list<ExtraInfo> generationExtra;
				
	const Mnemonic myMnemonicObj(privKey);

				
	GenerationParameters params(myPrivKey.compressed, true, true);
	GenerationMethods::saveAllFiles(address.toString(), wif.toString(), "vanity pool", 
		generationExtra, thetime, 
		params,
		myMnemonicObj
		);				
}















//Todo: Move to separate file for vanity generation
void generateVanity(int argc, char* argv[], const std::string& thetime, const bool usePool)
{
#ifndef __linux
	std::cerr << "Vanity is not supported on Windows." << std::endl;
	return;
#endif
	
	bool printAll(false);
		
	std::string searchPatternRaw;
	if(argc < 3)
	{
		std::cout << "Usage: bitgen vanity pattern [Name]" << std::endl;
		throw std::string("Pattern must be given (eg 1PR)");
	}

	searchPatternRaw = argv[2];


	std::string accountName;
	if(argc > 3)
	{
		accountName = argv[3];
		std::cout << "Account name: " << accountName << std::endl;
	}

	std::string pkifStr;
	if(argc > 3)
	{
		pkifStr = argv[3];
		std::cout << "Pkif: " << pkifStr << std::endl;
	}

	if(usePool)
	{
		if(pkifStr.empty())
		{
			std::cout << "Usage: bitgen vanityPool pattern pkif [Name]" << std::endl;
			
			throw std::string("Pkif must be given");
		}
	}


	VanitySearcher::searchForVanityAddress(searchPatternRaw, usePool, pkifStr, thetime, accountName);
}









void generatePaymentRequest(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen reqpayment coinAddress amount [label] [message] [LastDate]" << std::endl;
		return;			
	}
	
	const std::string adr(argv[2]);
	const PubAddress myPub(adr);
	//myPub.validate();

	std::string amount;
	if(argc > 3)
	{
		const std::string amountGiven(argv[3]); 
		//Make sure the value is a non-zero number
		const double value = atof(amountGiven.c_str());
		if(value == 0.0)
		{
			throw std::string("Incorrect or zero sum given");
		}
		
		std::cout << "Using amount: " << value << std::endl;
		char str[128];
		sprintf(str, "%0f", value);
		amount = str;
	}


	std::string label;
	if(argc > 4)
	{
		label = argv[4];
	}

	std::string message;
	if(argc > 5)
	{
		message = argv[5];
	}

	std::string lastDate;
	if(argc > 6)
	{
		lastDate = argv[6];
	}
		
	const std::list<std::string> not_used_printExtra;

	const Language lang = getLanguage();
	//const int margin(100);
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang/*, margin*/);		
	const PublicPostscriptInfo pubInfo(adr, lastDate, label);
	
	//postscriptGenerator.saveInvoiceFile(pubInfo, amount, message);
	const std::string psPublic = postscriptGenerator.generateInvoice(pubInfo, amount, message);
	
	const std::string pubFilename = pubInfo.pubkey + "_invoice.ps";
	std::cout << "Saving publfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);
	
}





void generatePublicPart(const std::vector<std::string>& args, int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen pubaddr bitcoinAddress [label]" << std::endl;
		return;			
	}
	
	const std::string adr(argv[2]);
	const PubAddress myPub(adr);
	//myPub.validate();


	const std::vector<std::string> extraArgs = getExtraArgs(args, 1);
		
	std::string label;
	if(extraArgs.size() >= 1)
	{	
		label = extraArgs[0];
		std::cout << "Label: " << label << std::endl;
	}


	const Language lang = getLanguage();
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang);
	const PublicPostscriptInfo pubInfo(adr, thetime, label);
				
	const std::string psPublic = postscriptGenerator.postscriptPagePublic(pubInfo);
	
	const std::string pubFilename = adr + "_share.ps";
	std::cout << "Saving publfile: " << pubFilename << std::endl;
	fileWrite(pubFilename, psPublic);	
}





//Todo: Remove this function, the command "info" should be used instead
void verifyWif(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen verifyWIF wif" << std::endl;
		return;			
	}
	
	//const
	std::string wif(argv[2]);
	if(wif == "-")
	{
		std::cout << "Enter the wif value at one row" << std::endl;
		std::cout << "Press enter when all chars are entered" << std::endl;
		
		wif = readStdinLine(10, false, isBase58Char);
	}
	else
	{
		std::cout << "WARNING: The information entered at the command line" << std::endl;
		std::cout << "WARNING: might be stored in a shell history file." << std::endl;
		std::cout << "WARNING: Consider using the \"-\" option for entering information." << std::endl;			
		std::cout << std::endl;		
	}


	const Wif myWif(wif);
		
		
	//const BigInt<256> priv = myWif.toPrivKey();
	const PrivKey pk = myWif.getPriv();
	
	std::cout << "Priv: " << pk.key /*priv*/ << std::endl;
		
	std::cout << "Wif OK" << std::endl;
}





void setLanguage(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen setlang LANGUAGE" << std::endl;
		std::cout << "LANGUAGE: en de" << std::endl;
		return;			
	}

	const std::string lang(argv[2]);

	if(lang == "en")
	{ 
		std::cout << "Setting language to english" << std::endl;
	} 
	else if(lang == "de")
	{ 
		std::cout << "Setting language to deutch" << std::endl;
	}
	else
	{
		std::cout << "Unknown lang: " << lang << std::endl;
		return;
	}
		 
	configureLang(lang);
}


/*
void generatePostscriptKeypair(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen keypair wif [name]" << std::endl;
		return;			
	}

	const std::string wifStr(argv[2]);
	const Wif wif(wifStr);
	const PrivKey privKey = wif.getPriv();
	
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = privKey.getPubkey(ec);

	std::cout << "WIF : " << wif << std::endl;
	std::cout << "PKIF: " << pubKey.toString() << std::endl;

	
	const std::list<std::string> generationExtra;
	const Language lang = getLanguage();
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, lang);
	const PublicPostscriptInfo pubInfo(pubKey.toString(), thetime, "");
	
	const PrivatePostscriptInfo privInfo(wif.toString(), "mnemonic", "GenMethod", privKey.compressed, generationExtra);
	
	postscriptGenerator.saveKeypairFile(privInfo,pubInfo);
	
}
*/

void uncompressPublicKey(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen uncompress COMPRESSEDPUBKEY" << std::endl;
		return;			
	}

	const std::string pubkeyStr(argv[2]);
	//std::cout << "Will convert: " << pubkeyStr << std::endl;
	
	if(pubkeyStr.size() != 66)
	{
		std::cout << "Got key with size: " << pubkeyStr.size() << ", but need 66 chars" << std::endl;
		throw std::string("Error, compressed key size must be 66 charcters");
	}
	

	const PubKey pubKey = PubKey::fromHex(pubkeyStr);
	//const ECPoint pubKey = PubKey::fromHexString(pubkeyStr);
	
	const std::string uncompStr = hexString(pubKey.point.getRaw(false));
	
	std::cout << "Uncompressed: " << uncompStr << std::endl;
}



void compressPublicKey(int argc, char* argv[], const std::string& thetime)
{
	if(argc < 3)
	{
		std::cout << "Usage: bitgen compress VERBOSEPUBKEY" << std::endl;
		return;			
	}

	const std::string pubkeyStr(argv[2]);
	std::cout << "Will convert: " << pubkeyStr << std::endl;
	
	if(pubkeyStr.size() != 130)
	{
		std::cout << "Got key with size: " << pubkeyStr.size() << ", but need 130 chars" << std::endl;
		throw std::string("Error, verbose key size must be 130 charcters");
	}
	

	const PubKey pubKey = PubKey::fromHex(pubkeyStr);

	//const ECPoint pubKey = PubKey::fromHexString(pubkeyStr);
	
	/*
	if(!BitcoinEllipticCurve::verifyECPoint(pubKey))
	{
		throw std::string("Error, point not on curve");
	}
	
	std::cout << "OK, point on curve" << std::endl;
*/
	
	const std::string compStr = hexString(pubKey.point.getRaw(true));
	
	std::cout << "Compressed: " << compStr << std::endl;
}




#ifndef NO_BRAINWALLET_GENERATION

void testArgon(const std::string& passphrase, const std::string& salt,
	const std::string& hashNumStr)
{

	std::cout << "Testing argon... " << std::flush;
	const std::string kdfStr = bitgenArgon2Kdf(passphrase, salt);
	const BigInt<512> hashNumComputed(kdfStr, 'c');
	
	const BigInt<512> hashNum(BigInt<512>::fromHex(hashNumStr)); //     hashNumStr.c_str());
	if(hashNum != hashNumComputed)
	{
		std::cout << std::endl;
		std::cout << "Error, computed kdfStr: " << hexString(kdfStr) << std::endl;
		
		throw std::string("Argon error");
	}

	std::cout << "OK" << std::endl;
	
}


void brainwalletTests()
{	
	const std::string passphrase("1234567890abcdefghijklmnopq");
	const std::string salt("someone@email.com");
	const std::string hashNumStr("9A5D21E6D7F1D3D1C5A7FBE2547F16ABF2DAE34379FA6309F9672491E2D83276");
	testArgon(passphrase, salt, hashNumStr);
	
	
	const std::string passphrase2("abc");
	const std::string salt2("this@abc.org");
	const std::string hashNumStr2("BD8BBBA3F95A59CA5E3100B4EFC488A9395A253F1366DFBCD84632A2F204C23D");	
	testArgon(passphrase2, salt2, hashNumStr2);
	
	
	const std::string passphrase3("");
	const std::string salt3("");
	const std::string hashNumStr3("93687FFD2E4E6A6A4E09188D6E1CCD3D981981597FE95237231A17DB6991E4F7");
	testArgon(passphrase3, salt3, hashNumStr3);
	
}
#endif






void printIntSize()
{
	std::cout << "sizeof(int)        : " << sizeof(int) << " gives " << sizeof(int)*8 << " bits" << std::endl;
	std::cout << "sizeof(long)       : " << sizeof(long) << " gives " << sizeof(long)*8 << " bits" << std::endl;
	std::cout << "sizeof(long long)  : " << sizeof(long long) << " gives " << sizeof(long long)*8 << " bits" << std::endl;
#if __x86_64__ || __ppc64__	
	std::cout << "sizeof(__uint128_t): " << sizeof(__uint128_t) << " gives " << sizeof(__uint128_t)*8 << " bits" << std::endl;	
#endif
}



void printTransactionUsage()
{
	std::cout << "Usage: bitgen transaction txHash1 outIndex1 wif1 prevAmountSatoshi1 out nextAddr1 amountSatoshi1 [nextAddr2 amountSatoshi2]" << std::endl;
}



void parseExtra(const std::vector<std::string>& args, const int posExtra, int& locktime, bool& fixednounce, bool& saveScripts)
{
	for(int currPos = posExtra + 1 ; currPos < args.size() ; currPos++)
	{
		const std::string extraArgName = args[currPos];
		
		if(extraArgName == "locktime")
		{
			currPos++;
			if(currPos >=  args.size())
			{
				throw std::string("No locktime specified");
			}
			
			const std::string locktimeStr = args[currPos];
			locktime = atoi(locktimeStr.c_str());
			std::cout << "Locktime: " << locktime << std::endl;			
		}
		else if(extraArgName == "fixednounce")
		{
			fixednounce = true;
			
			std::cout << std::endl;
			std::cout << "WARNING: USING FIXED NOUNCE, USE **ONLY** FOR TEST" << std::endl;
			std::cout << std::endl;			
		}
		else if(extraArgName == "savescripts")
		{
			saveScripts = true;
			
			std::cout << "Saving transaction scripts" << std::endl;
		}		
		else
		{
			std::cout << "Unknown extra argument: " << extraArgName << std::endl;
			throw std::string("Unknown extra argument");
		}
	}
}




//std::string
BinaryScript createOpreturnScript(const std::string& dataStr, const std::string& typeStr)
{		
	if(typeStr == "OP_RETURN")
	{
		const BinaryScript script = ScriptCreator::createOpReturnScript(dataStr);
		return script;
	}
	else if(typeStr == "op_return")
	{
		const std::string decoded = convertFromHex(dataStr);
		std::cout << "op_return: " << hexString(decoded) << ": " << ScriptDecoder::printable(decoded) << std::endl;
		const BinaryScript script = ScriptCreator::createOpReturnScript(decoded);
		return script;
	}
	else if(typeStr == "DOCPROOF")
	{
		const std::string decodedHash = convertFromHex(dataStr);
		//std::cout << "DOCPROOF: " << hexString(decodedHash) << std::endl;
		const std::string toStore = "DOCPROOF" + decodedHash;
		const BinaryScript script = ScriptCreator::createOpReturnScript(toStore);
		return script;
	}
	else if(typeStr == "DOCPROOFFILE")
	{
		//Read file and calculate doublesha256
		const std::string file = readFile(dataStr);
		const std::string fileHash = doubleSha256(file);
		
		std::cout << "DOCPROOF: " << hexString(fileHash) << std::endl;
		const std::string toStore = "DOCPROOF" + fileHash;
		const BinaryScript script = ScriptCreator::createOpReturnScript(toStore);
		return script;
	}	
	else if(typeStr == "EW")
	{				
		const std::string toStore = convertFromHex("455720") + dataStr;	//"EW "
		const BinaryScript script = ScriptCreator::createOpReturnScript(toStore);
		return script;
	}
	else if(typeStr == "EWA")
	{
		const std::string toStore = convertFromHex("455741") + dataStr;				
		const BinaryScript script = ScriptCreator::createOpReturnScript(toStore);
		return script;
	}
	else
	{
		std::cerr << "Unknown type: " << typeStr << std::endl;
		
		throw std::string("Unknown type");
	}

	return BinaryScript(""); //Should not get here
}










std::list<TxInput> createInputParts(const std::vector<std::string>& args, const bool saveTransaction, const int addPos)
{	
	std::list<TxInput> inputs;
	//const int addPos = saveTransaction ? 0 : 1;
	
	//std::cout << "Will parse inputs" << std::endl;
	for(int i = 0 ; ; i++)
	{
		if(args.size() < (addPos + 4 + i * 4))
		{
			throw std::string("Input argument error");			
		}
		
		const std::string outStr = args[addPos + 1 + i * 4];
		if(outStr == "out")
		{
			break;
		}		
		
		
		const std::string txHashStr = args[addPos + 1 + i * 4];
		const std::string outputIndexStr = args[addPos + 2 + i * 4];
		const std::string wifStrOrig = args[addPos + 3 + i * 4];	
		const std::string prevAmountStr = args[addPos + 4 + i * 4];
		
		std::string wifStr;
		bool usePayToPubkey(false);
		if(wifStrOrig[0] == '_')
		{
			wifStr = wifStrOrig.substr(1);	//Remove the undescore
			usePayToPubkey = true;
		}
		else
		{
			wifStr = wifStrOrig;
		}
		
		//const std::string outStr = args[addPos + 5 + i * 4];

		if(txHashStr.size() != 64)
		{
			std::cout << std::endl;
			std::cout << "Warning, transaction hash not 64 characters: " << txHashStr << std::endl;
			std::cout << "Got " << txHashStr.size() << " characters" << std::endl;			
			
			std::cout << std::endl;			
		}
		
		TxHash txHash(txHashStr);
		const int outputIndexNum = atoi(outputIndexStr.c_str());		
		const BigInt<128> prevAmountBigInt(Amount::parseAmountString(prevAmountStr));
		
		if(wifStr == "anyone")
		{
			std::cout << "Get from anyone-can-spend" << std::endl;

			const BinaryScript script = ScriptCreator::createAnyoneCanSpendFundscript();
			//const BinaryScript binScript(script);
			const BigInt<64> sequence(0);
			const TransactionInput input(txHash, outputIndexNum, script, sequence);
			
			const TxInput txInput(input, prevAmountBigInt, InputExtraInfo::PAYTO_ANYONECANSPEND);
			
			inputs.push_back(txInput);				
			
			std::cout << "Ready with anyone-can-spend" << std::endl;
			continue;
		}
		
		try
		{
			const Wif wif(wifStr);			
			const PrivKey privkey = wif.getPriv();

			const BitcoinEllipticCurve ec;
			const PubKey pubkey = privkey.getPubkey(ec);
			
			if(usePayToPubkey)
			{
				//std::cout << "Using pay-to-pubkey for input" << std::endl;
				const BinaryScript script = ScriptCreator::createOldPubkeyFundscript(pubkey);
				//const BinaryScript binScript(script);
				//const std::string script = Transaction::createPayToPubkeyRedeemUnsigned(privkey);
				const BigInt<64> sequence(0);
				const TransactionInput input(txHash, outputIndexNum, script, sequence);
				
				const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
				inputs.push_back(txInput);				
			}
			else
			{
				//Normal pay-to-pubkey-hash address
				
				const std::string pubAddRaw = pubkey.getRaw();
				const std::string theSha = sha256(pubAddRaw);
				const std::string scriptHashStr = ripemd160(theSha);
				
				const BinaryScript script = ScriptCreator::scriptPubkeyWithHash(scriptHashStr);
				//const BinaryScript binScript(script);
				//const Script unsignedScript = Script::parse(script);
				//std::cout << "The script: " << unsignedScript.toString() << std::endl;
				const BigInt<64> sequence(0);
				const TransactionInput input(txHash, outputIndexNum, script, sequence);
				
				const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEYHASH);
		
				inputs.push_back(txInput);
			}
		}
		catch(const std::string& err)
		{
			const BinaryScript script = endsWith(wifStr, ".script") ? TransactionDecoder::getScriptFromFile(wifStr) : BinaryScript(convertFromHex(wifStr));
			
			
			std::cout << "Got script: " << script.hex() << std::endl;
			const BigInt<64> sequence(0);
			const TransactionInput input(txHash, outputIndexNum, script, sequence);
			
			const TxInput txInput(input, prevAmountBigInt, InputExtraInfo::PAYTO_SCRIPTHASH);
		
			inputs.push_back(txInput);
			
		}		
	}
	
	if(inputs.empty())
	{
		throw std::string("No outputs specified");
	}
	
	return inputs;
}







std::list<TransactionOutput> createOutputParts(const std::vector<std::string>& args, const std::list<TxInput>& inputs, const int addPos)
{
	std::list<TransactionOutput> recipients;
	//std::cout << "Parsing outputs: " << addPos << std::endl;
	
	for(int i = 0 ; ; i++)
	{
		//std::cout << "Parsing output #" << i << std::endl;
		const int pos1 = addPos + 2 + inputs.size() * 4 + i * 2;
		const int pos2 = addPos + 3 + inputs.size() * 4 + i * 2;

		
		if(args.size() <= pos1)
		{
			//std::cout << "Nothing more to parse" << std::endl;
			break;
		}
		
		if(args.size() < pos2)
		{
			throw std::string("No amount for output");
		}
		
		const std::string nextAddrStr = args[pos1];
		//std::cout << "Next addr str: " << nextAddrStr << std::endl;
		
		
		//if(nextAddrStr == "fixnounce")
		//{
		//	std::cout << "Using fixed nounce" << std::endl;
		//	break;
		//}
		
		//std::cout << "NextAddr: " << nextAddrStr << std::endl;
		
		if(nextAddrStr == "extra")
		{
			std::cout << "Has extra arguments" << std::endl;
			break;
		}
				
		const std::string amountStr = args[pos2];
		
		//std::cout << "amountStr: " << amountStr << std::endl;
		

		if(nextAddrStr == "OP_RETURN" || nextAddrStr == "op_return" || nextAddrStr == "DOCPROOF" || nextAddrStr == "DOCPROOFFILE" 
			|| nextAddrStr == "EW" || nextAddrStr == "EWA")
		{
			//std::cout << "op_return" << std::endl;
			const Amount zeroAmountVal;

			const BinaryScript script = createOpreturnScript(amountStr, nextAddrStr);
			const TransactionOutput scriptAndAmount(script, zeroAmountVal);
			recipients.push_back(scriptAndAmount);

			continue;
		}

		//std::cout << "AmountStr: " << amountStr << std::endl;
		const Amount amountVal(Amount::parseAmountString(amountStr));
		
		
		if(nextAddrStr == "anyone")
		{
			std::cout << "Anyone can spend output" << std::endl;
			const BinaryScript script = ScriptCreator::createAnyoneCanSpendScript();	
			//const BinaryScript binScript(script);		
			const TransactionOutput scriptAndAmount(script, amountVal);
			recipients.push_back(scriptAndAmount);			
			continue;
		}

		
		try
		{

			
			const PubAddress nextAddress(nextAddrStr);
			if(nextAddress.isMultisig())
			{
				//std::cout << "Multisig addr not supported: " << nextAddress.toString() << std::endl;
				//std::cout << "Use the multisig script instead" << std::endl;
				
				const BinaryScript script = ScriptCreator::createMultisigFundScript(nextAddress);
				TransactionOutput scriptAndAmount(script, amountVal);
				recipients.push_back(scriptAndAmount);				
			}
			else
			{					
				const BinaryScript script = ScriptCreator::scriptPubAddress(nextAddress);
				TransactionOutput scriptAndAmount(script, amountVal);
				recipients.push_back(scriptAndAmount);
			}
			
			continue;
		}
		catch(const std::string& err)
		{
		}
		
		try
		{
			//std::cout << "Not normal address, assuming multisig script" << std::endl;
			const std::string redeemScript = convertFromHex(nextAddrStr);			
			std::cout << "Multisig redeem script: " << hexString(redeemScript) << std::endl;
			const BinaryScript fundScript = ScriptCreator::createP2SHFundScript(redeemScript);
			std::cout << "Fund script: " << fundScript.hex() << std::endl;
			
			const TransactionOutput scriptAndAmount(fundScript, amountVal);
			recipients.push_back(scriptAndAmount);
			
			continue;
		}
		catch(const std::string& err)
		{
		}
		
		//Try pubkey
		try
		{
			const PubKey pubkey(PubKey::fromPkif(nextAddrStr));
			std::cout << "pubkey: " << pubkey.toString() << std::endl;
			const BinaryScript fundScript = ScriptCreator::createOldPubkeyFundscript(pubkey);
			const TransactionOutput scriptAndAmount(fundScript, amountVal);
			recipients.push_back(scriptAndAmount);
			
			continue;
			
		}
		catch(const std::string& err)
		{
		}

		//Try predefined script file
		//std::cout << "Will check if script" << std::endl;
		
		if(endsWith(nextAddrStr, ".script"))
		{
			std::cout << "This is a script" << std::endl;
			try
			{
				const BinaryScript fundScript = TransactionDecoder::getScriptFromFile(nextAddrStr);
				std::cout << "Got fund script: " << fundScript.hex() << std::endl;
				const TransactionOutput scriptAndAmount(fundScript, amountVal);
				recipients.push_back(scriptAndAmount);
			
				continue;
			}
			catch(const std::string& err)
			{
				std::cout << "Can not open script file: " << nextAddrStr << " : " << err << std::endl;
				throw std::string("Can not open script file");
			}				
		}			
				
		std::cout << "Output value error" << std::endl;
		throw std::string("Output value error");
	}
	
	return recipients;
}




TxData parseTransactionArguments(const std::vector<std::string>& args, bool& saveTransaction, bool& fixednounce, bool& saveScripts)
{
	
	if(args.size() < 7)
	{
		printTransactionUsage();
		throw std::string("Incorrect usage");
	}	
	
	
	if(args[1] == "nosave")
	{
		saveTransaction = false;
	}
		
	
	const int addPos = saveTransaction ? 0 : 1;

	//std::cout << "Will parse inputs" << std::endl;
	const std::list<TxInput> inputs = createInputParts(args, saveTransaction, addPos);

	//std::cout << "Will parse outputs" << std::endl;			
	const std::list<TransactionOutput> recipients = createOutputParts(args, inputs, addPos);
	
	//std::cout << "Parse outputs ready" << std::endl;			
	
	
	const int posExtra = addPos + 2 + inputs.size() * 4 + recipients.size() * 2;
	
	//std::cout << "posExtra: " << posExtra << std::endl;
	//std::cout << "args.size(): " << args.size() << std::endl;
	
	int locktime(0);
	
	parseExtra(args, posExtra, locktime, fixednounce, saveScripts);
	
	const TxData txData(inputs, recipients, locktime);
	return txData;
}





void createInScript(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Create inscript not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	//Todo
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen inscript type [extra]" << std::endl;	
		return;
	}	
	
	const std::string typeStr(args[1]);
	if(typeStr == "addr")
	{
		const std::string pkifStr(args[2]);
	
	
		std::cout << "Creating address in script without signature information" << std::endl;
		std::cout << "Note: The script can not be used directly as in script. " << std::endl;
		std::cout << "      it is used for creating the signature" << std::endl;
	
		const PubKey pubkey(PubKey::fromPkif(pkifStr));
	

		std::cout << "Pubkey: " << pubkey.toString() << std::endl;

/*			
		if(usePayToPubkey)
		{
			std::cout << "Using pay-to-pubkey" << std::endl;
			const std::string script = ScriptCreator::createOldPubkeyFundscript(pubkey);
			//const std::string script = Transaction::createPayToPubkeyRedeemUnsigned(privkey);
			const BigInt<64> sequence(0);
			const TransactionInput input(txHash, outputIndexNum, script, sequence);
				
			const TxInput txInput(input, privkey, prevAmountBigInt, InputExtraInfo::PAYTO_PUBKEY);
			inputs.push_back(txInput);				
		}
		else
		{
*/


								
		const std::string pubAddRaw = pubkey.getRaw();
		const std::string theSha = sha256(pubAddRaw);
		const std::string scriptHashStr = ripemd160(theSha);
				
		const BinaryScript script = ScriptCreator::scriptPubkeyWithHash(scriptHashStr);
	
		const PubAddress pubAddress	= pubkey.getAddress();
	
		const std::string scriptFilename = "redeem_addr_" + getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
		Script::saveScriptWithFilename(script, scriptFilename);	
	}
	else
	{
		std::cout << "Unknown input script type: " << typeStr << std::endl;
		throw std::string("Unknown type");
	}
}















void createOutScript(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Create outscript not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}


	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen outscript addr address" << std::endl;	
		std::cout << "       bitgen outscript pubkey pubkey/pkif" << std::endl;	
		std::cout << "       bitgen outscript opreturn data rettype" << std::endl;
		std::cout << "       bitgen outscript anyone" << std::endl;
		
		return;
	}	
	
	const std::string scriptType(args[1]);
	
	if(scriptType == "addr")
	{
		if(args.size() < 3)
		{
			std::cout << "Usage: bitgen outscript addr address" << std::endl;		
			return;
		}	
		
		std::cout << "Creating address out script" << std::endl;
		
		const std::string nextAddrStr(args[2]);
	
		const PubAddress pubAddress(nextAddrStr);
	
		if(pubAddress.isMultisig())
		{				
			const BinaryScript script = ScriptCreator::createMultisigFundScript(pubAddress);
			const std::string scriptFilename = "fund_multisig_" + getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
			Script::saveScriptWithFilename(script, scriptFilename);	
		}
		else
		{					
			const BinaryScript script = ScriptCreator::scriptPubAddress(pubAddress);
			const std::string scriptFilename = "fund_address_" + getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
			Script::saveScriptWithFilename(script, scriptFilename);	
		}
	}
	else if(scriptType == "pubkey")
	{
		if(args.size() < 3)
		{
			std::cout << "Usage: bitgen outscript pubkey pubkey/pkif" << std::endl;	
			return;
		}	
	
		std::cout << "Creating pubkey out script" << std::endl;
	
		const std::string pkifStr(args[2]);	
		
		const PubKey pubkey(PubKey::fromPkif(pkifStr));
		const PubAddress pubAddress = pubkey.getAddress();
	
		std::cout << "pubkey: " << pubkey.toString() << std::endl;
		const BinaryScript fundScript = ScriptCreator::createOldPubkeyFundscript(pubkey);
			
		const std::string scriptFilename = "fund_pubkey_" + getCurrencyShortName() + "_" + pubAddress.toString() + ".script";
		Script::saveScriptWithFilename(fundScript, scriptFilename);	
	}
	else if(scriptType == "opreturn")
	{

		if(args.size() < 4)
		{
			std::cout << "Usage: bitgen outscript opreturn data rettype" << std::endl;	
			std::cout << "rettype: OP_RETURN op_return DOCPROOF DOCPROOFFILE EW EWA" << std::endl;	
			
			return;
		}	
		std::cout << "Creating OP_RETURN out script" << std::endl;
	
	
		const std::string dataStr(args[2]);
		const std::string typeStr(args[3]);
	
		std::cout << "Data: " << dataStr << std::endl;
		std::cout << "Type: " << typeStr << std::endl;
	
		const BinaryScript script = createOpreturnScript(dataStr, typeStr);
		const std::string scriptHash = doubleSha256(script.raw());

		const std::string scriptFilename = "fund_opreturn_" + getCurrencyShortName() + "_" + hexString(scriptHash) + ".script";
		Script::saveScriptWithFilename(script, scriptFilename);	
	}
	else if(scriptType == "anyone")
	{

		if(args.size() < 2)
		{
			std::cout << "Usage: bitgen outscript anyone" << std::endl;	
			return;
		}	

		std::cout << "Creating anyone-can-spend out script" << std::endl;

		const BinaryScript script = ScriptCreator::createAnyoneCanSpendScript();			
	
		//const std::string scriptHash = doubleSha256(script);

		const std::string scriptFilename = "fund_anyone_" + getCurrencyShortName() + ".script";
		Script::saveScriptWithFilename(script, scriptFilename);
	}
	else
	{
		std::cerr << "Unknown script type: " << scriptType << std::endl;
	}
	
	
}










void saveUnsignedScripts(const TxData& txData, const std::string& transHash)
{
	
	int i(0);
	for(std::list<TxInput>::const_iterator it = txData.inputs.begin() ;
		it != txData.inputs.end() ;
		++it)
	{
		const TxInput& input = (*it);
		
		if(input.scriptIsSigned())
		{
			std::cout << "Script already signed: " << intToString(i) << std::endl;
			continue;
		}
		
		const BinaryScript script = input.input.script;
		
		const std::string scriptFilename = "redeem_" + getCurrencyShortName() + "_" + transHash + "_" + intToString(i) + "_unsigned.script";
		Script::saveScriptWithFilename(script, scriptFilename);
		i++;
	}
}




void saveSignedScripts(const BinaryTransaction& transaction, const std::string& transHash)
{
	const CryptoTransaction trans = CryptoTransaction::parse(transaction);
	
	int i(0);
	for(std::list<TransactionInput>::const_iterator it = trans.inputs.begin() ;
		it !=  trans.inputs.end() ;
		++it)
	{
		const TransactionInput input = (*it);
		const BinaryScript script = input.script;
		const std::string scriptFilename = "redeem_" + getCurrencyShortName() + "_" + transHash + "_" + intToString(i) + "_signed.script";
		Script::saveScriptWithFilename(script, scriptFilename);		
		i++;
	}
}





void createTransaction(const std::vector<std::string>& args)
{	
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Create transaction not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
		
	bool saveTransaction(true);
	bool fixednounce(false);
	bool saveScripts(false);	//Saves the scripts as separate files
	
	const TxData txData = parseTransactionArguments(args, saveTransaction, fixednounce, saveScripts);

	txData.printTransactionInformation();

	const int numTempkeysNeeded = txData.getNumTempkeysNeeded();
	const std::list<BigInt<256> > tempKeys = getTempKeys(numTempkeysNeeded, fixednounce);

	const BinaryTransaction binaryTransaction = txData.createSignedTransaction(tempKeys);
	
	
	const BigInt<128> transactionSize(binaryTransaction.size());	
	const BigInt<128> minerFee(txData.getMinerFee().amount);
	//std::cout << "T size: " << transactionSize.strDec() << std::endl;
	//std::cout << "T fee : " << minerFee.strDec() << std::endl;
	
	
	const Amount feePerByte = minerFee / transactionSize;
	
	//std::cout << "Tempkeys size: " << tempKeys.size() << std::endl;
	//std::cout << "Inputs size: " << txData.inputs.size() << std::endl;
	
	const bool signedAll = (tempKeys.size() == txData.numInputs());
	
	if(!signedAll)
	{
		std::cout << "Unsigned ";
	}
	
	std::cout << "Transaction: " << binaryTransaction.asHex() << std::endl;
		
	if(!signedAll)
	{
		std::cout << "Note: Multisig inputs were not signed" << std::endl;
	}
	
	std::cout << std::endl;	
	
	std::cout << "Transaction size   : " << binaryTransaction.size() << std::endl;
		std::cout << "Fee per byte       : ";
	if(minerFee.isNegative())
	{
		std::cout << "negative " << std::endl;
	}
	else
	{
		std::cout  << feePerByte.amount.strDec() << " satoshi/byte" << std::endl;	
	}
	
	std::cout << "Hash of transaction: " << binaryTransaction.hashStr()  << std::endl;
	
	if(saveTransaction)
	{
		Transaction::saveTranscation(binaryTransaction, signedAll);
	}
	
	if(saveScripts)
	{
		std::cout << "Saving transaction scripts" << std::endl;
		
		saveUnsignedScripts(txData, binaryTransaction.hashStr());
		saveSignedScripts(binaryTransaction, binaryTransaction.hashStr());
	}
}
		




RSVal getSigFromFile(const std::string& filename)
{
	if(!endsWith(filename, ".sig"))
	{
		throw std::string("Signature file must end with .sig");
	}
		
	
	const std::string signature = TransactionDecoder::getSignatureFromFile(filename);

	//Check that the RS value is valid
	ReadStream stream(signature);
	const RSVal rs = ScriptDecoder::decodeSignatureRS(stream);
	return rs;
}




BinaryTransaction getTrans(const std::string& arg)
{
	//Determine if filename or hex string	
	try
	{
		const std::string trans = convertFromHex(arg);
		return BinaryTransaction(trans);
	}
	catch(const std::string& err)
	{
		//Not a hex string, file name?
		const std::string filename = arg;
		if(!endsWith(filename, ".trans"))
		{
			throw std::string("Transaction file must end with .trans");
		}
		
		const BinaryTransaction trans = TransactionDecoder::getTransactionFromFile(filename);
		return trans;
	}
	
	//Should not get here
	return BinaryTransaction("");
}


BinaryScript getScript(const std::string& arg)
{
	//Determine if filename or hex string	
	
	try
	{
		const std::string script = convertFromHex(arg);
		return BinaryScript(script);
	}
	catch(const std::string& err)
	{
		//Not a hex string, file name?
		const std::string filename = arg;
		if(!endsWith(filename, ".script"))
		{
			throw std::string("Script file must have extension .script");
		}
		
		const BinaryScript binScript = TransactionDecoder::getScriptFromFile(filename);
		//script = binScript.raw();
		return binScript;
	}
	
	//Will never get here
	return BinaryScript("");
}







void saveScript(const std::vector<std::string>& args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen savescript hexscript" << std::endl;	
		return;
	}	
	
	
	const std::string argScript(args[1]);
	const BinaryScript script = getScript(argScript);	
	
	const Script parsedScript = Script::parse(script);


	std::cout << std::endl;

	Script::saveTheScript(script);
}





std::string signatureFilename(const PrivKey& givenPriv)
{
	const BitcoinEllipticCurve ec;
	const PubKey pubkey = givenPriv.getPubkey(ec);
	
	const std::string filename = "sig_" + getCurrencyShortName() + "_" + pubkey.toString() + ".sig";
	return filename;
}



void saveSignatureFile(const std::string& signature, const PrivKey& givenPriv)
{
	//const std::string hashSigned = doubleSha256(signedTransaction);		

	const std::string filename = signatureFilename(givenPriv);
		
	const std::string content = hexString(signature) + "\n";
	fileWrite(filename, content);
	std::cout << std::endl;
	std::cout << "Saved signature file: " << filename << std::endl;
	std::cout << std::endl;	
}




BigInt<256> getTempKey(const bool fixedNounce)
{
	if(fixedNounce)
	{
		std::cout << std::endl;
		std::cout << "WARNING, USING FIXED NOUNCE, USE ONLY FOR TEST" << std::endl;
		std::cout << std::endl;
		
		const BigInt<256> tempKey("6722228373827382837283738483837483728384786374837483727383738273");
		return tempKey;		
	}
	else
	{
		std::cout << std::endl;
		std::cout << "Will get random bytes from /dev/random." << std::endl;
		std::cout << "Press keys or move the mouse if needed" << std::endl;
	
		const std::string random = getFromDevRandom(32);				
		const BigInt<256> tempKey(random, 'c');
		return tempKey;
	}
}



void partSignMultisig(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Sign multisig not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen signmultisig unsignedTransaction wif" << std::endl;	
		return;
	}	
	
	bool fixedNounce(false);
	if(args.size() > 3)
	{
		const std::string extra = args[3];
		if(extra == "fixednounce")
		{
			std::cout << "Warning, will use fixed nounce" << std::endl;
			fixedNounce = true;
		}
	}
	const std::string transactionStr(args[1]);
	const std::string wifStr(args[2]);
	
	const BinaryTransaction transaction = getTrans(transactionStr);
	
	const Wif wif(wifStr);
	
	//std::cout << "Wif: " << wif.toString() << std::endl;
	const PrivKey givenPriv = wif.getPriv();
	
	
	const BigInt<256> tempKey = getTempKey(fixedNounce);
	//std::cout << "The tempkey: " << tempKey << std::endl;	
		
	const RSVal rs = TransactionSigner::partlySignMultisigTransaction(transaction, givenPriv, tempKey);
	
	if(rs.r != 0)
	{
		const std::string signature = Script::encodeSignature(rs);
		saveSignatureFile(signature, givenPriv);			
	}
	
		
	//Todo: Save result here
}


//Todo: Remove?????
void not_used_signMultisig(const std::vector<std::string>& args, const bool partSign)
{
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen signmultisig transaction wif" << std::endl;	
		return;
	}	
	
	const std::string transactionStr(args[1]);
	const std::string wifStr(args[2]);
	
	//const std::string rawTransaction
	const BinaryTransaction transaction = getTrans(transactionStr);	
	//const BinaryTransaction	transaction(rawTransaction);
	
	const Wif wif(wifStr);
	
	//std::cout << "Wif: " << wif.toString() << std::endl;
	const PrivKey givenPriv = wif.getPriv();
	
	//Todo: Return signed transaction from function
	
	if(partSign)
	{
		const std::string random = getFromDevRandom(32);				
		const BigInt<256> tempKey(random, 'c');
		
		TransactionSigner::partlySignMultisigTransaction(transaction, givenPriv, tempKey);
	}
	else
	{
		TransactionSigner::signMultisigTransaction(transaction, givenPriv);
	}
	
	
	
	//Todo: Save result here
}



/*
Script addSignaturesToMultisigScript(scriptThatWillHaveSignatures, signatures)
{
	
}
*/


//Todo: Remove?
void not_used_combineNormalSignatures(const std::vector<std::string>& args)
{
	const std::string currency = getCurrencyShortName();
	if(currency != "btc" && currency != "btc" && currency != "doge" && currency != "vtc" && currency != "nmc"  && currency != "ftc")
	{
		std::cerr << "Combine signature not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen combinesig unsignedTransaction sig1 sig2 ... sigN" << std::endl;	
		return;
	}	
	
	const std::string transactionStr(args[1]);	
	const BinaryTransaction transaction = getTrans(transactionStr);	
		
	std::list<RSVal> signatures;	//Todo: Use RSVal for the signatures??
	
	for(int i = 0 ; i < (args.size() - 2) ; i++)
	{
		const std::string sigStr(args[2 + i]);	
		const RSVal signature = getSigFromFile(sigStr);		
		signatures.push_back(signature);
	}
	
	const BinaryTransaction signedTransaction = TransactionSigner::insertSignaturesInNormalTransaction(transaction, signatures);	
	//BinaryTransaction binaryTransaction(signedTransaction);
	
	if(signedTransaction.raw().empty())
	{
		std::cout << "Could not sign transaction" << std::endl;
		throw std::string("Could not sign");
	}
	
	std::cout << "Signed transaction: " << signedTransaction.asHex() << std::endl;
	Transaction::saveTranscation(signedTransaction);
}











void combineMultisigSignatures(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		const std::string currency = getCurrencyShortName();
		std::cout << "Combine multisig not supported for currency: " << currency << std::endl;
		throw std::string("Not supported");
	}
	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen combinemultisig unsignedTransaction sig1 sig2 ... sigN" << std::endl;	
		return;
	}	
	
	const std::string transactionStr(args[1]);	
	//const std::string transaction
	const BinaryTransaction transaction = getTrans(transactionStr);	
	
	//std::cout << "Unsigned transaction: " << hexString(transaction) << std::endl;
	
	
	std::list<RSVal> signatures;
	
	for(int i = 0 ; i < (args.size() - 2) ; i++)
	{
		const std::string sigStr(args[2 + i]);	
		//std::cout << "SigFile: " << sigStr << std::endl;
		const RSVal signature = getSigFromFile(sigStr);	
		
		//std::cout << "Signature: " << hexString(signature) << std::endl;
		
		signatures.push_back(signature);
	}
	
	const BinaryTransaction signedTransaction = TransactionSigner::insertSignaturesInMultisigTransaction(transaction, signatures);	
	//BinaryTransaction binaryTransaction(signedTransaction);
	
	if(!signedTransaction.raw().empty())		
	{
		std::cout << "Signed transaction: " << signedTransaction.asHex()  << std::endl;
		Transaction::saveTranscation(signedTransaction); //signedTransaction);
	}
	else
	{
		std::cout << "Could not sign transaction" << std::endl;
	}
}



void decodeScript(const std::vector<std::string>& args)
{
	if(!transactionSupportedForCurrency())
	{
		const std::string currency = getCurrencyShortName();
		std::cout << "Decode script not supported for currency: " << currency << std::endl;
		throw std::string("Not supported");
	}
	
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen decodescript [verbose] hextransaction" << std::endl;	
		return;
	}	
	
	std::cout << std::endl;
	bool verbose(false);
	
	std::string hexStr;
	const std::string argOne(args[1]);
	if(argOne == "verbose")
	{
		verbose = true;
		hexStr = args[2];
	}
	else
	{
		hexStr = args[1];	
	}	
	
	//Determine if filename or hex string
	
	
	//std::string
	const BinaryScript script = getScript(hexStr);
	
	
	//std::cout << "Script: " << hexString(script) << std::endl;
	
	const Script scr = Script::parse(script);
	
	std::cout << "Decoded: " << scr.toString() << std::endl;
	std::cout << std::endl;
	
	
	const std::string raw = scr.serialize().raw();
	if(raw != script.raw())
	{
		std::cout << std::endl;
		std::cout << "Serialize error" << std::endl;
		
		std::cout << "Orig     : " << script.hex() /*hexString(script.raw())*/ << std::endl;
		std::cout << "Serialzed: " << hexString(raw) << std::endl;
		
		throw std::string("Serialize error");
	}
	
	std::cout << "Script type: " << scr.scriptTypeString() << std::endl;
	
}



void showAscii(const std::vector<std::string>& args)
{	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen showascii hextransaction" << std::endl;	
		return;
	}	

	const std::string hexStr(args[1]);

	const std::string trans = convertFromHex(hexStr);

	for(int i = 0 ; i < trans.size() ; i++)
	{
		const unsigned char c = trans[i];
		
		if(c >= 32 &&  c <= 128)
		{
			std::cout << c;
		}
		else
		{
			std::cout << "_";
		}
	}
	
	std::cout << std::endl;
}





















bool runGivenTransaction(const BinaryTransaction& transaction, /*const std::string& transaction,*/ const bool verbose)
{	
	//std::cout << "RUNGIVENTRANSACTION: " << (verbose ? "VERBOSE" : "silent") << std::endl;
	//std::cout << "Transaction: " << hexString(transaction) << std::endl;


	std::list<SigPos> inputScriptPositions;
	const BinaryTransaction binaryTransaction(transaction);		
	
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(binaryTransaction, inputScriptPositions);

	const TransAndInScriptPos transWithPos(transaction, inputScriptPositions);

	Amount totalIn;

	const bool transactionSuccess = trans.run(transWithPos, totalIn, verbose);

	
	if(transactionSuccess)
	{
		std::cout << "Transaction OK, all inputs OK" << std::endl;

		std::cout << std::endl;
		std::cout << "Total in amount : " << totalIn.amountString() << std::endl;
	
		const Amount totalOut = trans.getTotalOutAmount();
		std::cout << "Total out amount: " << totalOut.amountString() << std::endl;

		const Amount minerFee(totalIn - totalOut);

		std::cout << "Miner fee       : " << minerFee.amountString() << std::endl;
		
	}
	else
	{
		std::cout << "Transaction FAILED" << std::endl;
	}


	return transactionSuccess;	
}









void runTrans(const std::vector<std::string>& args)
{	
	if(!transactionSupportedForCurrency())
	{
		std::cerr << "Run transaction not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen runstrans transaction [verbose]" << std::endl;	
		return;
	}	
	
	bool verbose(false);
	
	std::cout << std::endl;
	
	const std::string argTransaction(args[1]);

	//const std::string transaction
	const BinaryTransaction transaction = getTrans(argTransaction);
	
	if(args.size() > 2)
	{
		const std::string argVerb(args[2]);	
		if(argVerb == "verbose")
		{
			verbose = true;
		}
		else
		{
			std::cout << "Argument error: " << argVerb << std::endl;
			throw std::string("Argument error");
		}
	}	
	
	runGivenTransaction(transaction, verbose);	
}






void saveTheTransaction(const std::vector<std::string>& args)
{	
	const std::string currency = getCurrencyShortName();
	if(currency != "btc" && currency != "btc" && currency != "doge" && currency != "vtc" && currency != "nmc"  && currency != "ftc")
	{
		std::cerr << "Save transactions not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen savetrans hextransaction [verbose]" << std::endl;
		std::cout << "verbose: Prints information about the transaction" << std::endl;
		return;
	}	

	bool printInfo(false);
	const std::string content(args[1]);

	if(args.size() > 2)
	{
		const std::string argVerb(args[2]);	
		if(argVerb == "verbose")
		{
			printInfo = true;
		}
		else
		{
			std::cout << "Argument error: " << argVerb << std::endl;
			throw std::string("Argument error");
		}
	}	


	//const std::string trans
	const BinaryTransaction trans = getTrans(content);

	if(printInfo)
	{
		std::cout << "Will parse, len: " << trans.size() << std::endl;
	}

	
	const CryptoTransaction cryptoTrans = CryptoTransaction::parse(trans, printInfo);
	
	//OK, no parse errors
	Transaction::saveTranscation(trans);

	//Serialize to make sure the result is the same
	//const std::string
	const BinaryTransaction newTrans = cryptoTrans.serialize();
	//const BinaryTransaction binaryTransaction = cryptoTrans.serialize();
	
	if(trans != newTrans)
	{
		std::cout << std::endl;
		std::cout << "Original transaction binary  : " << trans.asHex() /*hexString(trans.raw())*/ << std::endl;
		std::cout << "Serialized transaction binary: " << newTrans.asHex() /*hexString(newTrans.raw())*/ << std::endl;
		
		std::cout << "Parsed info: " << std::endl;
		cryptoTrans.printTransactionInfo(true);
		std::cout << std::endl;
		
		throw std::string("Error serialized version does not equal original transaction");
	}
	
}




void parseTransaction(const std::vector<std::string>& args)
{	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen parsetrans transaction" << std::endl;	
		return;
	}	
	
	const std::string hexStr = args[1];	
	
	//const std::string trans
	const BinaryTransaction trans = getTrans(hexStr);	
	//std::cout << "Decoded: " << hexString(trans) << std::endl;

	const BinaryTransaction binaryTransaction(trans);	
	const CryptoTransaction ct = CryptoTransaction::parse(binaryTransaction /*trans*/, true);
	
	std::cout << "Parse ready" << std::endl;
}



void getCppString(const std::vector<std::string>& args)
{	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen cppstr hextransaction" << std::endl;	
		return;
	}	
	
	std::cout << std::endl;
	
	const std::string hexStr(args[1]);
	
	
	//Determine if filename or hex string	
	//const std::string trans
	const BinaryTransaction trans = getTrans(hexStr);
	
	const std::string hex = trans.asHex();
		
	std::cout << "Orig trans: " << std::endl;
	
	for(int i = 0 ; i < hex.size() ; i++)
	{
		if(i % 79 == 0)
		{
			std::cout << "\\\n";			
		}
				
		const unsigned char c = hex[i];		
		std::cout << c;
	}
	std::cout << "\n";				
}


bool likelySignature(const std::string& sigStr)
{
	//std::cout << "Checking if signature: " << hexString(sigStr) << std::endl;	
	if(sigStr.size() < 2)
	{
		return false;
	}
	
	const unsigned char first = sigStr[0];
	const unsigned char last = sigStr[sigStr.size() - 1];
	
	if(first == 0x30 && last == 0x01)
	{
		return true;
	}
	
	return false;
}



void getTransactionInputHashes(const std::vector<std::string>& args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen transinputhashes transaction" << std::endl;	
		std::cout << "Displays the hashes for the inputs" << std::endl;
		return;
	}	
	
	std::cout << std::endl;
	
	const std::string hexStr(args[1]);
	
	
	//Determine if filename or hex string
	//const std::string trans
	const BinaryTransaction binaryTransaction = getTrans(hexStr);
	//const BinaryTransaction binaryTransaction(trans);		
	const CryptoTransaction myTrans = CryptoTransaction::parse(binaryTransaction); //trans);
	const std::list<TxHash> hashes = myTrans.getInputHashes();
	
	std::cout << "Num inputs: " << hashes.size() << std::endl;
	
	for(std::list<TxHash>::const_iterator it = hashes.begin() ;
		it != hashes.end() ;
		++it)
	{
		const TxHash& hash = (*it);
		
		std::cout << "Input hash: " << hash.toString() << std::endl;
	}
}



void fixLowSTransaction(const std::vector<std::string>& args)
{	
	const std::string currency = getCurrencyShortName();
	if(currency != "btc" && currency != "btc" && currency != "doge" && currency != "vtc" && currency != "nmc"  && currency != "ftc")
	{
		std::cerr << "Fix low-s not supported for " << getCurrencyName() << std::endl;
		throw std::string("Currency not supported");
	}
	
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen fixlowstrans hextransaction" << std::endl;	
		return;
	}	
	
	std::cout << std::endl;
	
	const std::string hexStr(args[1]);
	
	
	//Determine if filename or hex string	
	//const std::string trans
	const BinaryTransaction trans = getTrans(hexStr);
	//const std::string hash = trans.hash();
	
	//std::cout << "Orig trans: " << trans.asHex() << std::endl;
	
	const CryptoTransaction myTrans = CryptoTransaction::parse(trans);
	std::list<TransactionInput> newInputs;

	//Go through all input scripts to find high-s value
	for(std::list<TransactionInput>::const_iterator it = myTrans.inputs.begin() ;
		it != myTrans.inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		const Script parsedScript = Script::parse(ti.script);
		Script newScript;
		//Find Signature data
		for(std::list<ScriptItem>::const_iterator it = parsedScript.items.begin() ;
			it != parsedScript.items.end() ;
			++it)
		{
			const ScriptItem& si = (*it);
			if(si.object == ScriptItem::DATA)
			{
				if(likelySignature(si.data))
				{
					//std::cout << "This is a signature" << std::endl;
					
					const RSVal rs = ScriptDecoder::decodeSignatureRS(si.data);
					//std::cout << "R: " << rs.r << std::endl;
					//std::cout << "S: " << rs.s << std::endl;
					if(!rs.lowS())
					{
						std::cout << "Found signature with non-low S" << std::endl;
						const RSVal newRs = rs.getLowS();
						//std::cout << "Replaced S: " << newRs.s << std::endl;
						const std::string encodedRs = Script::encodeSignature(newRs);

						const ScriptItem newSi(ScriptItem::DATA, encodedRs);
						newScript.items.push_back(newSi);
						continue;
					}
					
				}
			}
			newScript.items.push_back(si);
		}
	
		const BinaryScript binScript(newScript.serialize());
		const TransactionInput newTi(ti.txHash, ti.index, binScript, ti.sequence);
		newInputs.push_back(newTi);

	}
	const BigInt<64> version(1);	//Todo
	const CryptoTransaction theNewTrans(version, newInputs, myTrans.outputs, myTrans.locktime);
	
	const BinaryTransaction newTrans = theNewTrans.serialize();
	
	if(newTrans == trans)
	{
		std::cout << "No non-low-s signature found" << std::endl;
		return;
	}
	
	std::cout << std::endl;
	std::cout << "Created transaction with low-s" << std::endl;
	
	std::cout << "New transaction: " << newTrans.asHex() << std::endl;
	
	Transaction::saveTranscation(newTrans);	
}





void decodeTransaction(const std::vector<std::string>& args)
{	
	if(!transactionSupportedForCurrency())
	{
		const std::string currency = getCurrencyShortName();
		std::cout << "Decode transaction not supported for currency: " << currency << std::endl;
		throw std::string("Not supported");
	}
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen decodetransaction hextransaction [verbose | savescript] " << std::endl;	
		std::cout << "Option savescript: Saves all scripts in the transaction " << std::endl;
		std::cout << "Option verbose: Prints extra information" << std::endl;

		return;
	}	
	
	std::cout << std::endl;
	bool verbose(false);
	bool saveScript(false);
	
	const std::string hexStr(args[1]);
	
	std::string extraArg;
	if(args.size() > 2)
	{
		extraArg = args[2];
	}

	if(extraArg == "verbose")
	{
		verbose = true;
	}
	else if(extraArg == "savescript")
	{
		saveScript = true;
	}

	
	
	//Determine if filename or hex string	
	//const std::string trans
	const BinaryTransaction binaryTransaction = getTrans(hexStr);
	//BinaryTransaction binaryTransaction(trans);
	
	if(verbose)
	{
		//const std::string hash = doubleSha256(trans);		
		std::cout << "Hash of transaction: " << binaryTransaction.hashStr() /*
			hexString(Transaction::getReversedStr(hash))*/ << std::endl;
	}
	
	//std::cout << "Decoded: " << hexString(trans) << std::endl;
	
	const CryptoTransaction myTrans = CryptoTransaction::parse(binaryTransaction); //trans);


	if(verbose)
	{
		myTrans.printTransactionInfo(verbose);
	}
	else if(saveScript)
	{
		myTrans.saveScripts();
	}	
	else
	{
		myTrans.printSimple();
	}
	
	
	//if(saveTransaction)
	//{
	//	Transaction::saveTranscation(trans);		
	//}
}





//Todo: Move to separate file
RSVal RSVal::getLowS() const
{
	BitcoinEllipticCurve ec;

	const BigInt<1024> nHalf = (ec.n >> 1);
	
	if(s > nHalf)
	{
		//std::cout << "CALCULATING THE LOW S VALUE" << std::endl;
		const BigInt<1024> s2 = ec.n - s;
		const RSVal rs2(r, s2);
		return rs2;
	}
	else
	{
		return (*this);
	}
}




void signatureInfo(const std::vector<std::string>& args)
{	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen signatureinfo hexsignature" << std::endl;
		std::cout << "Prints the R and S values for a signature" << std::endl;
		std::cout << "The signature should be given in blockchain hex format, starting with 30..." << std::endl;
		return;
	}	

	//Todo: Accept .sig file as argument

	const std::string hexStr(args[1]);
	const std::string signature = convertFromHex(hexStr);
	//std::cout << "Sig: " << hexString(signature) << std::endl;

	ReadStream sigStream(signature);
	
	const RSVal rs = ScriptDecoder::decodeSignatureRS(sigStream);
		
	std::cout << "R: " << rs.r << std::endl;
	std::cout << "S: " << rs.s << std::endl;


	if(!rs.lowS())
	{
		std::cout << "The signature does not use low-s value" << std::endl;		
		const RSVal rsLowS = rs.getLowS();
		std::cout << "Corresponding LowS:" << std::endl;				
		std::cout << "R: " << rsLowS.r << std::endl;
		std::cout << "S: " << rsLowS.s << std::endl;
		
	}
		
}






void infoForAnyExtended(const std::vector<std::string>& args)
{	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen extended infopriv PRIVEXT" << std::endl;	

		return;
	}	
	
	
	const std::string extended(args[2]);
		
	const ExtendedKey extendedKey = decodeHD(extended);
	
	if(extendedKey.isPrivate)
	{
		const ExtendedPrivateKey getExtendedPrivateKey = decodePrivateHD(extendedKey);	
		const ExtendedPublicKey extendedPublicKey = getExtendedPrivateKey.getPub();

		std::cout << "Extended info:\n" << getExtendedPrivateKey.infoString() << std::endl;
		std::cout << "Extended priv: " << getExtendedPrivateKey.toString() << std::endl;	
		std::cout << "Extended pub : " << extendedPublicKey.toString() << std::endl;			
	}
	else
	{
		//const ExtendedPrivateKey getExtendedPrivateKey = decodePrivateHD(extendedKey);	
		//std::cout << "Extended priv: " << getExtendedPrivateKey.toExtFormat() << std::endl;	
		
		const ExtendedPublicKey extendedPublicKey = decodePublicHD(extendedKey);
		std::cout << "Extended pub : " << extendedPublicKey.toString() << std::endl;			
		std::cout << std::endl;

		std::cout << "Extended info:\n" << extendedPublicKey.infoString() << std::endl;
	}

}



//Todo: REMOVE, use infoForAnyExtended() instead
void infoForPrivateExtended(const std::vector<std::string>& args)
{	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen extended infopriv PRIVEXT" << std::endl;	

		return;
	}	
	
	
	const std::string xpriv(args[2]);
		
	const ExtendedKey extendedKey = decodeHD(xpriv);
	if(!extendedKey.isPrivate)
	{
		std::cout << "This is not a private extended key" << std::endl;
		return;
	}

	const ExtendedPrivateKey getExtendedPrivateKey = decodePrivateHD(extendedKey);	
	const ExtendedPublicKey extendedPublicKey = getExtendedPrivateKey.getPub();

	std::cout << "Extended info:\n" << getExtendedPrivateKey.infoString() << std::endl;
	std::cout << "Extended priv: " << getExtendedPrivateKey.toString() << std::endl;	
	std::cout << "Extended pub : " << extendedPublicKey.toString() << std::endl;	
}


//Todo: REMOVE, use infoForAnyExtended() instead
void infoForPublicExtended(const std::vector<std::string>& args)
{	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen extended infopublic PUBEXT" << std::endl;	

		return;
	}	
	
	
	const std::string xpub(args[2]);
	
	
	const ExtendedKey extendedKey = decodeHD(xpub);
	if(extendedKey.isPrivate)
	{
		std::cout << "This is not a public extended key" << std::endl;
		return;
	}

	const ExtendedPrivateKey getExtendedPrivateKey = decodePrivateHD(extendedKey);	
	const ExtendedPublicKey extendedPublicKey = decodePublicHD(extendedKey);

	std::cout << "Extended info:\n" << getExtendedPrivateKey.infoString() << std::endl;
	//std::cout << "Extended priv: " << getExtendedPrivateKey.toExtFormat() << std::endl;	
	std::cout << "Extended pub : " << extendedPublicKey.toString() << std::endl;	
}







void calcAddressDerivation(const std::vector<std::string>& args)
{	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen addressderivation extAddr path [endnum] ..." << std::endl;
		return;
	}	
	
	const std::string givenxpriv(args[1]);
	
	
	std::string xpriv(givenxpriv);
	std::string givenpath;
	std::string endNumStr;
	
	bool printExtendedLegacy(false);
	
	//std::cout << "XPRIV: " << xpriv << std::endl;
	
	if(xpriv == "mnemonic" || xpriv == "electrummnemonic")
	{
		if(args.size() < 4)
		{
			std::cout << "Usage: bitgen addressderivation mnemonic MNEMONIC path [endnum] ..." << std::endl;
			return;
		}	
		
		
		const std::string mnemonicStr(args[2]);
		
		givenpath = args[3];		
		
		if(args.size() > 4)
		{
			endNumStr = args[4];
		}

		if(args.size() > 5)
		{
			std::cout << "Got extra arg" << std::endl;
			const std::string extra(args[5]);
			if(extra == "printextendedlegacy")
			{
				std::cout << "PRINTING EXTENDED LEGACY" << std::endl;
				printExtendedLegacy = true;				
			}
			else
			{
				std::cout << "Unknown arg: " << extra << std::endl;
				throw std::string("Unknown command line arg");
			}
		}
		
		
		if(xpriv == "mnemonic")
		{
			const Mnemonic mnemonic(mnemonicStr);
		
			//Todo: Passphrase????
			const ExtendedPrivateKey extendedPrivate = mnemonic.getExtendedPrivFromMnemonic("");
			const ExtendedPublicKey extendedPublic = extendedPrivate.getPub();
		
			const std::string extendedPriv = extendedPrivate.toString();
			std::cout << "Extended private: " << extendedPriv << std::endl;
			std::cout << "Extended public : " << extendedPublic.toString() << std::endl;
			
			if(printExtendedLegacy)
			{
				std::cout << "Ext priv legacy: " << extendedPrivate.toString(true) << std::endl;
				std::cout << "Ext pub legacy : " << extendedPublic.toString(true) << std::endl;			
			}
			
			xpriv = extendedPriv;
		}
		else
		{
			std::cout << "Using electrum mnemonic" << std::endl;
			const ElectrumMnemonic mnemonic(mnemonicStr);
		
			const ExtendedPrivateKey extendedPrivate = mnemonic.getPrivateFromElectrumMnemonic();	
			const ExtendedPublicKey extendedPublic = extendedPrivate.getPub();

			//Todo: Passphrase????
			//const ExtendedPrivateKey extendedPrivate = getExtendedPrivFromMnemonic(mnemonic.toStr(), "");
		
			const std::string extendedPriv = extendedPrivate.toString();
			std::cout << "Extended private: " << extendedPriv << std::endl;
			std::cout << "Extended public : " << extendedPublic.toString() << std::endl;
			
			xpriv = extendedPriv;
			
		}
		
		
		
	}
	else
	{
		givenpath = args[2];
		
		if(args.size() > 3)
		{
			endNumStr = args[3];
		}
		
		if(args.size() > 4)
		{
			//std::cout << "Got extra arg" << std::endl;
			const std::string extra(args[4]);
			if(extra == "printextendedlegacy")
			{
				std::cout << "PRINTING EXTENDED LEGACY" << std::endl;
				printExtendedLegacy = true;
			}
			else
			{
				std::cout << "Unknown arg: " << extra << std::endl;
				throw std::string("Unknown command line arg");
			}
		}
		
	}
	
	const ExtendedKey extendedKey = decodeHD(xpriv, true);
	
	
	
	const std::string path = DerivationPath::expandShorthands(givenpath);

	std::cout << "Got pathstring: " << givenpath << " Endnumstr: " << endNumStr << std::endl;

	int endNum(-1);
	if(!endNumStr.empty())
	{
		endNum = atoi(endNumStr.c_str());
	}
	

	std::cout << "Performning address derivation" << std::endl;

	const DerivationPath origDerivationPath(path); //  = DerivationPath::getDerivationPath(path);

	if(!extendedKey.isPrivate)	//Public
	{
		std::cout << "Converting path to public" << std::endl;
	}	

	const DerivationPath derivationPath = extendedKey.isPrivate ? (origDerivationPath 
																	/*DerivationPath::getDerivationPath(path)*/) : 
																	origDerivationPath.getPub();
	
	
	
	std::cout << std::endl;
	std::cout << "Derivation path: " << derivationPath.toString();

	std::string content;
	content += derivationPath.toString();


	if(endNum == -1)
	{
		endNum = derivationPath.getLast().val.intVal();
	}
	else
	{
		std::cout << " to " << endNum;
		content += " to ";
		content += intToString(endNum);		
	}
	
	std::cout << std::endl;
	content += "\n\n";


	const ChildNum startItem = derivationPath.getLast();
	const DerivationPath derivationPathShort = derivationPath.withoutLast();
	const BigInt<32> finalNum(endNum + 1);
	
	//std::cout << "Last: " << last.toString() << std::endl;		
	//std::cout << "Derivation without last: " << derivationPathShort.toString() << std::endl;

	const std::string filenameStart = std::string("derddr_") +  getCurrencyShortName() + "_" + 
			startItem.toString() + "_" + intToString(endNum) + "_";

	if(extendedKey.isPrivate)
	{
		const ExtendedPrivateKey extendedPrivKey = decodePrivateHD(extendedKey);

		const ExtendedPrivateKey derived = extendedPrivKey.derive(derivationPathShort);
		std::cout << "Derived private key: " << derived.toString() << std::endl;
		
		const ExtendedPublicKey myderivedPub = derived.getPub();
		std::cout << "Derived public key : " << myderivedPub.toString() << std::endl;
		
		if(printExtendedLegacy)
		{
			std::cout << "Private key legacy: " << derived.toString(true) << std::endl;
			std::cout << "public key legacy : " << myderivedPub.toString(true) << std::endl;			
		}
		
		std::cout << std::endl;
		
		const BitcoinEllipticCurve ec;
		

		for(BigInt<32> i = startItem.val ; i < finalNum ; i++)
		{
			std::list<ChildNum> thisPath;
			thisPath.push_back(i);
			const DerivationPath thisDerPath(thisPath, false);
			
			const ExtendedPrivateKey leaf = derived.derive(thisDerPath);				

			const PrivKey privKey = leaf.privKey;

			const PubAddress pubAddress = privKey.getAddress(ec);
		
			std::cout << i.strDec() << " Private: " << privKey.toString() << " ";
			std::cout << "Public: " << pubAddress.toString() << std::endl;
			
			content += i.strDec();
			content += " ";
			content += privKey.toString();
			content += " ";			
			content += pubAddress.toString();
			content += "\n";
			
		}
		
		const ExtendedPublicKey derivedPub = derived.getPub();
		
		const std::string filename = filenameStart +  derivedPub.toString().substr(40) + "_private.txt";
		
		std::cout << std::endl;
		std::cout << "Saving file: " << filename << std::endl;
		fileWrite(filename, content);
	}
	else
	{
		const ExtendedPublicKey extendedPubKey = decodePublicHD(extendedKey);
		
		const ExtendedPublicKey derived = extendedPubKey.derive(derivationPathShort);
		
		std::cout << std::endl;

		for(BigInt<32> i = startItem.val ; i < finalNum ; i++)
		{
			std::list<ChildNum> thisPath;
			thisPath.push_back(i);
			
			const DerivationPath thisDerPath(thisPath, true);
			const ExtendedPublicKey leaf = derived.derive(thisDerPath);				
				
			const PubKey pubKey = leaf.pubKey;
			const PubAddress pubAddress = pubKey.getAddress();
				
			std::cout << i.strDec() << " Public: " << pubAddress.toString() << std::endl;

			content += i.strDec();
			content += " ";
			content += pubAddress.toString();
			content += "\n";
			
		}
		
		const std::string filename = filenameStart + derived.toString().substr(40) +  "_public.txt";
		
		std::cout << std::endl;
		std::cout << "Saving file: " << filename << std::endl;
		fileWrite(filename, content);
	}

}



void calcDerivation(const std::vector<std::string>& args)
{	
	if(args.size() < 3)
	{
		std::cout << "Usage: bitgen derivation extPriv derivationPath ..." << std::endl;
		return;
	}	
	
	std::string xpriv(args[1]);
	std::string givenPath;
	bool useLegacy(false);
	
	if(xpriv == "mnemonic")
	{
		if(args.size() < 4)
		{
			std::cout << "Usage: bitgen derivation mnemonic MNEMONIC derivationPath ..." << std::endl;
			return;
		}	
		
		std::cout << "Got private as mnemonic" << std::endl;
		const std::string mnemonicStr(args[2]);
		const Mnemonic mnemonic(mnemonicStr);
		
		//Todo: Passphrase????
		const ExtendedPrivateKey extendedPrivate = mnemonic.getExtendedPrivFromMnemonic("");
		
		const std::string extendedPriv = extendedPrivate.toString();
		std::cout << "Extended private: " << extendedPriv << std::endl;
		xpriv = extendedPriv;
		
		
		givenPath = (args[3]);		
		
		if(args.size() >= 5)
		{
			//std::cout << "Got extra arg" << std::endl;
			const std::string extra(args[4]);
			if(extra == "printextendedlegacy")
			{
				std::cout << "Using Legacy format for extended keys for currency: " << getCurrencyName() << std::endl;
				useLegacy = true;
			}
			else
			{
				std::cout << "Unknown argument: " << extra << std::endl;
				throw std::string("Unknown argument");
			}
		}		
	}
	else
	{
		givenPath = args[2];
		
		if(args.size() >= 4)
		{
			//std::cout << "Got extra arg" << std::endl;
			const std::string extra(args[3]);
			if(extra == "printextendedlegacy")
			{
				std::cout << "Using Legacy format for extended keys for currency: " << getCurrencyName() << std::endl;
				useLegacy = true;
			}
			else
			{
				std::cout << "Unknown argument: " << extra << std::endl;
				throw std::string("Unknown argument");
			}
		}
		
	}
	
	
	//std::cout << "Original path: " << givenPath << std::endl;	
	const std::string mypath = DerivationPath::expandShorthands(givenPath);
	//std::cout << "Expanded path: " << mypath << std::endl;	
	
	
	const DerivationPath derivationPath(mypath); //DerivationPath::getDerivationPath(mypath));
	
	std::cout << "Derivation path: " << derivationPath.toString() << std::endl;



	const ExtendedKey extendedKey = decodeHD(xpriv, true);
	
	//std::cout << "READY DECODING HD key" << std::endl;
	
	if(extendedKey.isPrivate)
	{
		//throw std::string("Internal error, not extended private");
		const ExtendedPrivateKey privKey = decodePrivateHD(extendedKey);


		//std::cout << "Extended priv key: \n" << privKey.toString() << std::endl;
		//std::cout << "Start priv key : " << privKey.toExtFormat() << std::endl;
		//std::cout << std::endl;
	

		//std::cout << "Derivation path: " << derivationPath.toString() << std::endl;
		//std::cout << std::endl;

		const ExtendedPrivateKey resultKey = privKey.derive(derivationPath);

		const ExtendedPublicKey pubKey = resultKey.getPub();
		const ExtendedPublicKey pubKeyClean = pubKey.getWithoutDeptAndParent();


		std::cout << "Result priv key : " << resultKey.toString() << std::endl;
		std::cout << "Result pub key  : " << pubKey.toString() << std::endl;
		std::cout << "Clean pub key   : " << pubKeyClean.toString() << std::endl;
		//std::cout << "Result pub clean info: " << std::endl << pubKeyClean.infoString() << std::endl;
		
		if(useLegacy)
		{
			std::cout << std::endl;
			std::cout << "Priv key legacy: " << resultKey.toString(true) << std::endl;
			std::cout << "Pub key legacy : " << pubKey.toString(true) << std::endl;
		}
		
		
		if(false)
		{
			std::cout << std::endl;
			std::cout << "Result priv key info: \n" << resultKey.infoString() << std::endl;
		}
	}
	else
	{
		const ExtendedPublicKey pubKey = decodePublicHD(extendedKey);

		std::cout << "Start public key : " << pubKey.toString() << std::endl;

		const ExtendedPublicKey derived = pubKey.derive(derivationPath);

		std::cout << "Result pub key: " << derived.toString() << std::endl;
		std::cout << std::endl;
		std::cout << "Result pub key info: \n" << derived.infoString() << std::endl;	
	}
}




		/*
		const ChildNum num(0, false);
		const ExtendedPublicKey derived = CKDpub(pubKey, num);
		*/


/*
	//const std::string xpub("xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8");

		//I = HMAC-SHA512(Key = cpar, Data = serP(point(kpar)) || ser32(i)).
		//const std::string data = 
		const BitcoinEllipticCurve e;
		const std::string serPoint = getExtendedPrivateKey.privKey.point(e).getRaw(true);
		
		BigInt<32> i(345);

	}
	
	std::cout << std::endl << "-----------------------------" << std::endl;
	const ExtendedKey extendedpubKey = decodeHD(xpub);
	std::cout << "Will get public HD" << std::endl;
	const ExtendedPublicKey getExtendedPublicKey = decodePublicHD(extendedpubKey);
	std::cout << "Extended public key: " << getExtendedPublicKey.toString() << std::endl;
	std::cout << "Extended public key ext format: " << getExtendedPublicKey.toExtFormat() << std::endl;
}
*/







GenerationParameters parseTheExtraParams(const std::vector<std::string>& args, const int startPos, const bool useCurrencySpecific = false)
{
	//std::cout << "In parseTheExtraParams" << std::endl;
	GenerationParameters params;	

	const std::vector<std::string> allExtraArgs = getExtraArgs(args, startPos);

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
		else if(useCurrencySpecific && params.useCurrencySpecific(arg))
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
	return params;
}




int getNumWords(const std::vector<std::string>& args, const int pos = 3)
{
	int numWords(24);
	if(args.size() > pos)
	{
		std::string numStr(args[pos]);
		//std::cout << "NUMWORDS: " << numStr << std::endl;
		numWords = atoi(numStr.c_str());
		if(numWords == 0)
		{
			std::cout << "Incorrect number: \"" << numStr << "\"" << std::endl;			
			throw std::string("Incorrect number argument");
		}
		else if(numWords > 24)
		{
			std::cout << "Max 24 seed words allowed: \"" << numStr << "\"" << std::endl;
			throw std::string("Incorrect number argument, too large");
		}
	}
		
	if(numWords < 12)
	{
		std::cout << "Mnemonic too short, mnemomic with " << numWords << " words is NOT SECURE" << std::endl;
		std::cout << std::endl;
		throw std::string("Mnemonic too short");
	}

	return numWords;
}




void createElectrumAccount(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen electrum-" << getCurrencyShortName() << " genType ..." << std::endl;
		std::cout << " genType: random, hash, mnemonic, hex, dice, brain" << std::endl;
		std::cout << std::endl;
		std::cout << " Options can be added in the end of the command line" << std::endl;
		std::cout << "Example 1:" << std::endl;
		std::cout << "   bitgen electrum random 13 name \"My test account\"" << std::endl;
		std::cout << "   This will generate a random 13 word mnemonic with the name \"My test account\"" << std::endl;		
		std::cout << std::endl;
		std::cout << "Example 2:" << std::endl;		
		std::cout << "   bitgen electrum hash HASH 15 name \"My hash test\"" << std::endl;
		std::cout << std::endl;		
		std::cout << "Example 3:" << std::endl;		
		std::cout << "   bitgen electrum mnemonic \"distance curve job orchard riot solve rice regular combine bonus ski beef rail chief coyote material\" name \"My test with 16 words\"" << std::endl;
		std::cout << std::endl;		
		std::cout << "Example 4:" << std::endl;
		std::cout << "   bitgen electrum hex 7289BCF837900FE8788BC78A111678ABB 12 name \"Hex test\"" << std::endl;
		std::cout << std::endl;		
		std::cout << "Example 5:" << std::endl;	
		std::cout << "   bitgen electrum dice 1231236626256261235123656661616625662656626252661265 12 name \"Dice test\"" << std::endl;
		std::cout << std::endl;		
		std::cout << "Example 6:" << std::endl;			
		std::cout << "   bitgen electrum brain me@gmail.com MyBrainwal... 13 name \"Brainwallet test\"" << std::endl;
		return;
	}	
	
	const std::string genType(args[1]);
	

	if(genType == "random")
	{
#ifndef __linux
		std::cerr << "Random is not supported on Windows. Use \"hash\" instead" << std::endl;
		return;
#endif

		const int numWords = getNumWords(args, 2);

		const GenerationParameters params = parseTheExtraParams(args, 2, true);

		std::cout << "Generating electrum seed with " << numWords << " words from random" << std::endl;
		GenerationMethods::createElectrumRandom(thetime, numWords, params);
	}	
	else if(genType == "hash")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen electrum hash HASHVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		
		const int numWords = getNumWords(args);
		
		const GenerationParameters params = parseTheExtraParams(args, 3);
		
		std::string hashval(args[2]);
		GenerationMethods::createElectrumHash(thetime, numWords, hashval, params);
	}
	else if(genType == "mnemonic")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen electrum mnemonic MNEMONICVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string mnemonic(args[2]);
		
		const std::list<ExtraInfo> extra;
		GenerationMethods::createElectrumMnemonic(thetime, mnemonic, params, extra);
	}
	else if(genType == "hex")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended hex HEXVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	

		const int numWords = getNumWords(args);


		const GenerationParameters params = parseTheExtraParams(args, 3);
		
		std::string hexval(args[2]);
		GenerationMethods::createElectrumHex(thetime, numWords, hexval, params);
	}
	
	else if(genType == "dice")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen electrum dice DICEVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const int numWords = getNumWords(args);


		const GenerationParameters params = parseTheExtraParams(args, 3);
		
		std::string diceval(args[2]);
		GenerationMethods::createElectrumDice(thetime, numWords, diceval, params);
	}
	else if(genType == "brain")
	{		
		if(args.size() < 4)
		{
			std::cerr << "Usage: bitgen electrum brain email/salt BRAINVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	

		const int numWords = getNumWords(args, 4);

		const GenerationParameters brainParams = parseTheExtraParams(args, 4);
		
		const std::string saltVal(args[2]);
		const std::string extbrainVal(args[3]);
				
		GenerationMethods::createElectrumBrain(thetime, numWords, saltVal, extbrainVal, brainParams);
	}	
	else
	{
		std::cerr << "Unknown electrum option: " << genType << std::endl;
		throw std::string("Unknown electrum option");
	}
}



void createExtendedAccount(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen extended genType ..." << std::endl;
		std::cout << " genType: random, mnemonic, seed, hash, dice, hex, brain" << std::endl;
		
		return;
	}	
	
	const std::string genType(args[1]);
	
	if(genType == "random")
	{
#ifndef __linux
		std::cerr << "Random is not supported on Windows. Use \"hash\" instead" << std::endl;
		return;
#endif
		
		//std::cout << "Will parse all extra params" << std::endl;
		const GenerationParameters params = parseTheExtraParams(args, 1, true);

		std::cout << "Generating random" << std::endl;
		GenerationMethods::createExtendedRandom(thetime, params);
	}
	else if(genType == "hash")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended hash HASHVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string hashval(args[2]);
		GenerationMethods::createExtendedHash(thetime, hashval, params);
	}
	else if(genType == "seed")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended seed SEEDVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string seedval(args[2]);
		GenerationMethods::createExtendedSeed(thetime, seedval, params);
	}
	else if(genType == "mnemonic")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended mnemonic MNEMONICVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string mnemonic(args[2]);
		GenerationMethods::createExtendedMnemonic(thetime, mnemonic, params);
	}		
	else if(genType == "hex")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended hex HEXVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	

		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string hexval(args[2]);
		GenerationMethods::createExtendedHex(thetime, hexval, params);
	}
	else if(genType == "dice")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended dice DICEVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	

		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string diceval(args[2]);
		GenerationMethods::createExtendedDice(thetime, diceval, params);
	}
	else if(genType == "priv")
	{
		if(args.size() < 3)
		{
			std::cerr << "Usage: bitgen extended extpriv EXTPRIVVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	
		const GenerationParameters params = parseTheExtraParams(args, 2);
		
		std::string extprivVal(args[2]);
		GenerationMethods::createExtendedExtPriv(thetime, extprivVal, params);
	}
	else if(genType == "brain")
	{
		//std::cout << "OK, brain choosen" << std::endl;
		
		if(args.size() < 4)
		{
			std::cerr << "Usage: bitgen extended brain email/salt BRAINVAL ..." << std::endl;
			throw std::string("Incorrect usage");
		}	

		//std::cout << "Will parse extra params" << std::endl;
		const GenerationParameters brainParams = parseTheExtraParams(args, 3);
		
		
		std::string saltVal(args[2]);
		std::string extbrainVal(args[3]);
		
		//std::cout << "Will call createExtendedExtBrain" << std::endl;
		
		GenerationMethods::createExtendedExtBrain(thetime, saltVal, extbrainVal, brainParams);
	}
	else if(genType == "infopriv")	//Todo: REMOVE, use "info" instead
	{
		infoForPrivateExtended(args);
	}
	else if(genType == "infopub")	//Todo: REMOVE, use "info" instead
	{
		infoForPublicExtended(args);
	}	
	else if(genType == "info")
	{
		infoForAnyExtended(args);	//Todo
	}		
	else
	{
		std::cerr << "Unknown extended option: " << genType << std::endl;
		throw std::string("Unknown extended option");
	}
}





void saveMnemonicResultInternal(const Mnemonic& mnemonic,
						const std::string& thetime,
						const GenerationParameters& params, const int numWords,
						const std::string& genMethod, const std::list<ExtraInfo>& generationExtra,
						const std::string& xpriv,
						const std::string& xpub,
						const std::string& derivedXpub,						
						const bool includedExtendedAddress)
{	
	//std::cout << "Mnemonic: " << mnemonic.toStr() << std::endl;
	
	const std::string privKeyAgain = mnemonic.getAsNumber();

	std::string currencyExtra;
	if(params.getCurrencySpecific())
	{
		currencyExtra += getCurrencyShortName();
		currencyExtra += "_";
	}

	//std::cout << "Taking hash of: \"" << xpub << "\"" << std::endl;
	
	const std::string hash = sha256(xpub);		//?????

		
	{	
		
		//Todo: Two object for private parts and public parts
		
		const FilePair content = generateMnemonicFile(genMethod, generationExtra, thetime, params.getAccountName(), mnemonic.toStr(),
				versionNumber, applicationName, params.getCurrencySpecific(), xpriv, xpub, derivedXpub, includedExtendedAddress, numWords);
	
	

	
		const std::string filename("mnemonic_" + currencyExtra + intToString(numWords) + "_" + hexString(hash) + "_private.txt");

		std::cout << std::endl;
		std::cout << "Saving file: " << filename << std::endl;
	
		fileWrite(filename, content.privateFile);
		
		
		if(includedExtendedAddress)
		{
			const std::string publicFilename("mnemonic_" + currencyExtra + intToString(numWords) + "_" + hexString(hash) + "_share.txt");

			std::cout << "Saving file: " << publicFilename << std::endl;
	
			fileWrite(publicFilename, content.publicFile);
		}
		
	}
	
	
	
	
	const PostscriptGenerator postscriptGenerator(versionNumber, applicationName, LANG_EN, 
		PostscriptGenerator::ADDRTYPE_NORMAL, params.getCurrencySpecific());
	
	
	
	{
		const std::string postscript = postscriptGenerator.generatePrivateMnemonic(mnemonic.toStr(), numWords, params.getAccountName(), 
																					genMethod, thetime, generationExtra, xpriv, xpub, derivedXpub);

		const std::string psFilename("mnemonic_" + currencyExtra + intToString(numWords) + "_" + hexString(hash) + "_private.ps");

		std::cout << "Saving file: " << psFilename << std::endl;
	
		fileWrite(psFilename, postscript);
	}
	
		
	if(includedExtendedAddress) //!xpub.empty())
	{
		const std::string pubPostscript = postscriptGenerator.generatePublicMnemonic(mnemonic.toStr(), numWords, params.getAccountName(), 
																					genMethod, thetime, generationExtra, xpriv, xpub, derivedXpub);

		const std::string pubPsFilename("mnemonic_" + currencyExtra + intToString(numWords) + "_" + hexString(hash) + "_share.ps");
	
		std::cout << "Saving file: " << pubPsFilename << std::endl;
	
		fileWrite(pubPsFilename, pubPostscript);
		
	}
	
}





/*
std::string getLastBytes(const BigInt<256>& privKey, const int numBytes)
{	
	const std::string privBytes = privKey.getBinaryLong();
	const int startPos = privBytes.size() - numBytes;
	const std::string priv = privBytes.substr(startPos);
	
	return priv;
}
*/


int calculateNumDiceNeeded(const int numBits)
{
	BigInt<512> val(BigInt<512>::onePattern());
	const int numShift = 512 - numBits;
	BigInt<512> valStart(val >> numShift);
		
	BigInt<512> zero;
	BigInt<512> six(6);
		
	int i = 0;
	for( ; valStart != zero ; i++)
	{
		//std::cout << "Val: " << valStart << std::endl;
		valStart /= six;
	}

	const int numDice(i);
	return numDice;
}





void saveResultForMnemonic(const Mnemonic & mnemonic, const std::string& thetime,
						const GenerationParameters& params, const std::string& random, const int numWords,
						const std::string& genMethod, std::list<ExtraInfo>& generationExtra)
{
	const ExtendedPrivateKey extendedPrivate = mnemonic.getExtendedPrivFromMnemonic(params.getPassword());
	const ExtendedPublicKey extendedPublicKey = extendedPrivate.getPub();


	const std::string derivationStr = DerivationPath::getCurrencyDerivationPath();

	const DerivationPath derivationPath(derivationStr);
	
	const ExtendedPrivateKey derivedPrivateKey = extendedPrivate.derive(derivationPath);
	const ExtendedPublicKey derivedPublicKey = derivedPrivateKey.getPub();
	const ExtendedPublicKey canonicalDerivedPublicKey = derivedPublicKey.getWithoutDeptAndParent();

	const std::string extPrivStr = extendedPrivate.toString();
	const std::string extPubStr = extendedPublicKey.toString();

	const std::string derivedPrivStr = derivedPrivateKey.toString();
	const std::string derivedPubStr = canonicalDerivedPublicKey.toString();

	std::cout << "Mnemonic: " << mnemonic.toStr() << std::endl;

	
	if(params.getGenerateExtended())
	{
		//std::cout << "Generating extended address" << std::endl;

		std::cout << "Extended privkey: " << extPrivStr << std::endl;
		std::cout << "Extended pubkey : " << extPubStr << std::endl;
		
		std::cout << "Derivation for " << getCurrencyName() << " (" << derivationStr << "):" << std::endl; 
		std::cout << "Derived privkey : " << derivedPrivStr << std::endl;		
		std::cout << "Derived pubkey  : " << derivedPubStr << std::endl;

		if(!params.getPassword().empty())
		{
			const ExtraInfo extraPass("Password", params.getPassword());
			generationExtra.push_back(extraPass);
		}
	}

	saveMnemonicResultInternal(mnemonic, thetime, params, numWords, genMethod,
			generationExtra, extPrivStr, extPubStr, derivedPubStr, 
			params.getGenerateExtended());			
}







int parseNumTextWords(const std::string& numWordsStr, std::string& wallet)
{
	if(numWordsStr == "jaxx")
	{
		wallet = "Jaxx";
		return 12;
	}
	else if(numWordsStr == "coinomi")
	{
		wallet = "Coinomi";		
		return 18;
	}
	else if(numWordsStr == "exodus")
	{
		wallet = "Exodus";		
		return 12;
	}
	else if(numWordsStr == "mycelium")
	{
		wallet = "Mycelium";
		return 12;
	}
	else if(numWordsStr == "multibit")
	{
		wallet = "Multibit HD";
		return 12;
	}
	
	
	const int numWords = atoi(numWordsStr.c_str());
	return numWords;
}





void createMnemonicAccount(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen genMnemonic numWords genType..." << std::endl;
		std::cout << "        genType: random, hex, hash, dice, mnemonic, " << std::endl;
		std::cout << " Example: bitgen genMnemonic 24 random" << std::endl;
		return;
	}	
	
	const std::string numWordsStr(args[1]);
	std::string genType;
	int commandLineStartPos(2);
	
	std::string wallet;

	int numWords(0);
	
	if(numWordsStr == "random" || numWordsStr == "hex" || numWordsStr == "hash" || numWordsStr == "dice" || numWordsStr == "mnemonic")
	{
		numWords = 24;
		genType = args[1];
		commandLineStartPos = 1;
	}
	else
	{
		if(args.size() < 3)
		{
			std::cout << "Not enough arguments" << std::endl;
			std::cout << "Usage: bitgen genMnemonic numWords genType..." << std::endl;
			std::cout << "        genType: random, hex, hash, dice, mnemonic, " << std::endl;
			std::cout << " Example: bitgen genMnemonic 24 random" << std::endl;
			return;			
		}
		
		numWords = parseNumTextWords(numWordsStr, wallet);
		genType = args[2];
		commandLineStartPos = 2;
	}
		
	
	/*numWords != 3 && numWords != 6 && numWords != 9 &&*/ 
	if(numWords != 12 && numWords != 15 && numWords != 18
		 && numWords != 21 && numWords != 24)
	{
		std::cout << "Incorrect number of words: " << numWordsStr << std::endl;
		std::cout << "Allowed values: " << "12, 15, 18, 21 or 24" << std::endl;
		throw std::string("Incorrect number text words");
	}

	//const std::string genType(args[2]);


	if(numWords < 12)
	{
		std::cout << std::endl;
		std::cout << "WARNING:" << std::endl;		
		std::cout << "WARNING: This random number does not have enough entropy for crypto currency!" << std::endl;
		std::cout << "WARNING:" << std::endl;
		std::cout << std::endl;
	}


	const int num32bit = numWords / 3;
	const int numBytes = 4 * num32bit;
	const int numBits = 8 * numBytes;

	std::cout << "Text words     : " << numWords << std::endl;
	//std::cout << "Num 32 bit items: " << num32bit << std::endl;
	std::cout << "Entropy [bits] : " << numBits << std::endl;
	std::cout << std::endl;

	if(genType == "random")
	{
		const GenerationParameters params = parseTheExtraParams(args, commandLineStartPos, true);
		
		
		std::cout << "Generating " << numBytes << " random bytes" << std::endl;
		const std::string random = getFromDevRandom(numBytes);
		std::cout << std::endl;
	
		if(random.size() != numBytes)
		{
			std::cout << "Buf size: " << random.size() << " num bytes: " << numBytes << std::endl;
			throw std::string("Internal error, incorrect number random bytes");
		}
	
		
	
		const BigInt<256> privkey(random, 'c');

		const Mnemonic mnemonicObj(privkey, numWords);
		const std::string mnemonic = mnemonicObj.toStr();
		
		std::list<ExtraInfo> generationExtra;
		
		if(!wallet.empty())
		{
			const ExtraInfo extraInfo("Wallet", numWordsStr);
			generationExtra.push_back(extraInfo);	
		}


		saveResultForMnemonic(mnemonicObj, thetime, params, random, numWords, "/dev/random", generationExtra);
	}
	else if(genType == "hex")
	{
		if(args.size() < (commandLineStartPos + 2))
		{
			std::cout << "Usage: bitgen genMnemonic numWords hex HEXVAL ..." << std::endl;
			return;
		}	
		
		const GenerationParameters params = parseTheExtraParams(args, commandLineStartPos + 1);
		
		const std::string hexVal(args[commandLineStartPos + 1]);		
		
		if(hexVal.size() != (numBytes * 2))
		{
			std::cerr << std::endl;
			std::cerr << "Incorrect amount data given. Got " << hexVal.size() << " hex digits, want " << (numBytes * 2) << " hex digits" << std::endl;
			throw std::string("Not enough data");
		}
		
		
		
		std::cout << "HexVal:   " << hexVal << std::endl;
		
		const std::string binStr = convertFromHex(hexVal);
		
		const std::string hexValUpper = hexString(binStr);
		
		
		const BigInt<256> privkey = BigInt<256>::fromHex(hexValUpper);
		std::cout << "PrivData: " << privkey << std::endl;
		
		const Mnemonic mnemonicObj(privkey, numWords);
		const std::string mnemonic = mnemonicObj.toStr();
	
		std::cout << "Mnemonic: " << mnemonic << std::endl;

		std::list<ExtraInfo> generationExtra;
		const ExtraInfo extraInfo("Hex", hexValUpper);
		generationExtra.push_back(extraInfo);	

		saveResultForMnemonic(mnemonicObj, thetime, params, binStr, numWords, "hex", generationExtra);		
	}
	else if(genType == "mnemonic")
	{
		std::cout << "Creating from mnemonic" << std::endl;
		
		if(args.size() < (commandLineStartPos + 2))
		{
			std::cout << "Usage: bitgen genMnemonic numWords mnemonic MNEMONICVAL ..." << std::endl;
			return;
		}	
		
		const GenerationParameters params = parseTheExtraParams(args, commandLineStartPos + 1);
		
		const std::string mnemonicStr(args[commandLineStartPos + 1]);
		std::cout << "Got mnemonic: " << mnemonicStr << std::endl;
		
		const Mnemonic mnemonicObj(mnemonicStr);
		

		const BigInt<256> privKey = mnemonicObj.toPrivKey();
		const std::string privkeyStr = privKey.getBinary();
		
		std::list<ExtraInfo> generationExtra;

		saveResultForMnemonic(mnemonicObj, thetime, params, privkeyStr, numWords, "mnemonic", generationExtra);
	}	
	else if(genType == "hash")
	{
		//std::cout << "Creating from hash" << std::endl;
		
		if(args.size() < (commandLineStartPos + 2))
		{
			std::cout << "Usage: bitgen genMnemonic numWords hash HASHVAL ..." << std::endl;
			return;
		}	
		
		const GenerationParameters params = parseTheExtraParams(args, commandLineStartPos + 1);
		
		const std::string hashStr(args[commandLineStartPos + 1]);
		
		std::cout << "Got hashStr: " << hashStr << std::endl;
	
		const int minHashSize = numWords * 10;
		std::cout << "Minimum permitted hash size: " << minHashSize << ", got: " << hashStr.size() << std::endl;
		if(hashStr.size() < minHashSize)
		{
			std::cerr << "Error, at least " << minHashSize << " characters needed, got: " << hashStr.size() << std::endl;
			
			throw std::string("Need larger hash");
		}
	
		const std::string sha = sha256(hashStr);
		const BigInt<256> privKey(sha, 'c');
	
	
		const std::string priv = privKey.getLastBytes(numBytes);
	
			
		std::cout << std::endl;
		std::cout << "Genrated private data: " << hexString(priv) << std::endl;

		const Mnemonic mnemonicObj(privKey, numWords);
		
		std::list<ExtraInfo> generationExtra;

		saveResultForMnemonic(mnemonicObj, thetime, params, priv, numWords, "hash", generationExtra);
	}
	else if(genType == "dice")
	{
		std::cout << std::endl;
		std::cout << "Creating from dice" << std::endl;
		
		if(args.size() < 3)
		{
			std::cout << "Usage: bitgen genMnemonic [numWords] dice DICEVAL ..." << std::endl;
			return;
		}	
		
		const GenerationParameters params = parseTheExtraParams(args, commandLineStartPos + 1);
		
		const std::string diceStr(args[commandLineStartPos + 1]);
		//std::cout << "Got diceval: " << diceStr << std::endl;
		
		const int numBits = numBytes * 8;
		
		const int numDice = calculateNumDiceNeeded(numBits);
		
		
		if(diceStr.size() != numDice)
		{
			std::cerr << "Should have exactly " << numDice << " dice values, got: " << diceStr.size() << std::endl;
			std::cerr << "Need " << numDice << " dice values" << std::endl;
			throw std::string("Incorrect num dice");
		}
		
		const BigInt<256> privKey = getRandomFrom6SideDice(diceStr, numDice);
		
		const Mnemonic mnemonicObj(privKey, numWords);

		std::list<ExtraInfo> generationExtra;
		const ExtraInfo extraInfo("Dice", diceStr);
		generationExtra.push_back(extraInfo);	

		const std::string priv = privKey.getLastBytes(numBytes);

		saveResultForMnemonic(mnemonicObj, thetime, params, priv, numWords, "dice", generationExtra);		
	}	
	else
	{
		std::cout << "Unknown generation type: " << genType << std::endl;
		throw std::string("Unknown generation type");		
	}
}




void listMnemonicSize(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 1)
	{
		std::cout << "Usage: bitgen listMnemonic" << std::endl;
		return;
	}	

	std::cout << "WALLET      Default Minsize  MaxSize      (TextWords)" << std::endl;
	std::cout << "=====================================================" << std::endl;	
	std::cout << "Coinomi:    18      12       24" << std::endl;
	std::cout << "Mycelium:   12                 " << std::endl;
	std::cout << "Exodus      12                 " << std::endl;
	std::cout << "Jaxx        12                 " << std::endl;
	std::cout << "MultibitHD  12      12       24" << std::endl;
	
	
	std::cout << std::endl;
}






void getMnemonicInformation(const std::vector<std::string>& args, const std::string& thetime)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitgen mnemonicinfo MNEMONIC ..." << std::endl;
		return;
	}	

	std::string mnemonicStr(args[1]);
	std::cout << "Mnemonic: " << mnemonicStr << std::endl;
	
	const Mnemonic mnemonic(mnemonicStr/*, false*/); 
	
	const std::string privKey = mnemonic.getAsNumber();
	
	std::cout << "Private data (without checksum):     " << hexString(privKey) << std::endl;
	
	const ExtendedPrivateKey extPk = calculateMasterKey(privKey);
	
	/*
	std::cout << "##############################" << std::endl;
	const std::string seed(convertFromHex("000102030405060708090a0b0c0d0e0f"));
	calculateMasterKey(seed);
	
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	
	const std::string seed2(convertFromHex("fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542"));
	calculateMasterKey(seed2);
	*/
}














//From bitcoin
void doBitgenWork(int argc, char* argv[])
{			
	time_t t = time(0);
	const std::string thetime = removeNewline(asctime(localtime(&t)));
	
	
	std::vector<std::string> args;
	for(int i = 1 ; i < argc ; i++)
	{
		args.push_back(argv[i]);		
	}
	
	
	/*
	for(std::vector<std::string>::const_iterator it = args.begin() ;
		it != args.end() ;
		++it)
	{
		const std::string& a = (*it);
		std::cout << "arg: " << a << std::endl;	
	}
	*/
	
	
	
	if(argc < 2)
	{
		//printUsage();
		std::cout << "For help, use \"bitgen-" << getCurrencyShortName() << " help\"" << std::endl;
		throw std::string("Missing argument");
		return;
	}

	const std::string firstArg(argv[1]);

	if(firstArg == "version" || firstArg == "-v")
	{
		std::cout << "bitgen version " << versionNumber << std::endl;
		std::cout << std::endl;
		std::cout << "using bigint " << BigInt<0>::version() << std::endl;
		std::cout << "using bitlib version " << bitlibVersionNumber << std::endl;
		
		std::cout << "http://www.bitgen.org/" << std::endl;		
	}
	else if(firstArg == "setlang")
	{
		setLanguage(argc, argv, thetime);
	}	
	else if(firstArg == "brainwallet")
	{
#ifdef NO_BRAINWALLET_GENERATION
		std::cout << "No brainwallet support" << std::endl;
#else		
		GenerationMethods::generateBrainWallet(args, thetime);
#endif		
	}
	else if(firstArg == "bulk")
	{
		GenerationMethods::generateBulk(args, thetime);
	}	
	else if(firstArg == "dice")
	{
		GenerationMethods::generateWithDice(args, thetime);
	}
	else if(firstArg == "hash")
	{
		GenerationMethods::generateHash(args, thetime);	
	}
	else if(firstArg == "hex")
	{
		GenerationMethods::generateHex(args, thetime);
	}
	else if(firstArg == "mnemonic")
	{
		GenerationMethods::generateFromMnemonic(args, thetime);		
	}		
	else if(firstArg == "random")
	{
		GenerationMethods::generateRandom(args, thetime);
	}
	else if(firstArg == "wif")
	{
		GenerationMethods::generateFromWif(args, thetime);
	}
	
	//Mini key commands
	else if(firstArg == "mini")
	{
		miniKeyGenerate(argc, argv, thetime);
	}
	else if(firstArg == "miniToWif")
	{
		generateWifFromMiniPrivKey(argc, argv, thetime);
	}	
	
	//else if(firstArg == "infoPrivate")
	//{	
	//	std::cout << "This command is deprecated. Use \"info\" instead" << std::endl;
	//	std::cout << std::endl;
	//	validatePrivateWif(argc, argv, thetime);		
	//}
	else if(firstArg == "info")
	{
		informationAboutEntity(args, thetime);
	}	
	else if(firstArg == "verifyWIF") //Todo: Remove??
	{
		std::cout << "This command is deprecated. Use \"info\" instead" << std::endl;		
		std::cout << std::endl;		
		verifyWif(argc, argv, thetime);		
	}	
	//else if(firstArg == "infoAddress")
	//{
	//	std::cout << "This command is deprecated. Use \"info\" instead" << std::endl;
	//	std::cout << std::endl;		
	//	verifyAddress(argc, argv, thetime);
	//}
	
	//Vanity commands
	else if(firstArg == "vanity")
	{
		generateVanity(argc, argv, thetime, false);
	}
	else if(firstArg == "vanityPool")
	{
		generateVanity(argc, argv, thetime, true);
	}
	else if(firstArg == "vanityPoolResult")
	{
		generateVanityPoolResult(argc, argv, thetime, true);
	}
	
	//Commands for Hierarcial random addresses
	else if(firstArg == "genRoot")
	{
		generateRootKey(thetime, argc, argv);
	}
	else if(firstArg == "genPub")
	{
		generatePublicFromRoot(argc, argv, thetime);
	}
	else if(firstArg == "genPriv")
	{
		generatePrivateFromRoot(argc, argv, thetime);
	}
	
	//Commands that display a public address
	else if(firstArg == "pub")
	{
		generatePublicPart(args, argc, argv, thetime);
	}
	else if(firstArg == "invoice")
	{
		generatePaymentRequest(argc, argv, thetime);
	}
	
	//Commands for public keys
	
	//else if(firstArg == "keypair")	//Generates the pub-priv keypair from wif
	//{
	//	generatePostscriptKeypair(argc, argv, thetime);
	//}	
	else if(firstArg == "uncompress")	//Uncompresses the public key in hex format
	{
		uncompressPublicKey(argc, argv, thetime);
	}
	else if(firstArg == "compress")		//Compresses the public key in hex format
	{
		//std::cout << "Will compress public key" << std::endl;
		compressPublicKey(argc, argv, thetime);
	}
	else if(firstArg == "help")
	{
		printUsage();	
	}
	else if(firstArg == "license")
	{
		printLicense();
	}
	else if(firstArg == "intsize")
	{
		printIntSize();
	}
	else if(firstArg == "savetrans")
	{
		saveTheTransaction(args);
	}	
	else if(firstArg == "decodetrans")
	{
		decodeTransaction(args);
	}
	else if(firstArg == "parsetrans")		//Redundant?
	{
		parseTransaction(args);
	}
	else if(firstArg == "transinputhashes")	//Redundant?
	{
		getTransactionInputHashes(args);
	}	
	else if(firstArg == "savescript")
	{
		saveScript(args);
	}	
	else if(firstArg == "decodescript")
	{
		decodeScript(args);
	}
	else if(firstArg == "runtrans")
	{
		runTrans(args);
	}
	//-----------	
	else if(firstArg == "genMnemonic")
	{
		createMnemonicAccount(args, thetime);
	}		
	else if(firstArg == "transaction")
	{
		createTransaction(args);
	}
	else if(firstArg == "hmacsha512")
	{
		std::cout << "Moved to unit tests" << std::endl;
		//testHmacSha512(args);
	}
/*	
	else if(firstArg == "derivation")
	{
		calcDerivation(args);
	}
	else if(firstArg == "addressderivation")
	{
		calcAddressDerivation(args);
	}
	*/		
	//else if(firstArg == "testderivation")
	//{
	//	testPerormDerivation(args);
	//}	
/*
	else if(firstArg == "electrum")
	{
		createElectrumAccount(args, thetime);
	}
	else if(firstArg == "extended")
	{
		createExtendedAccount(args, thetime);
	}
	else if(firstArg == "mnemonicInfo")
	{
		getMnemonicInformation(args, thetime);
	}	
	else if(firstArg == "listMnemonic")
	{
		listMnemonicSize(args, thetime);
	}
*/
	//else if(firstArg == "pbkdf2")
	//{
	//	testPbKdb2(args, thetime);
	//}	


/*	
	else if(firstArg == "timelock")
	{
		createTimelock(args);
	}
	else if(firstArg == "multisig")
	{
		createMultisig(args);
	}
	else if(firstArg == "signmultisig")	
	{
		partSignMultisig(args);
	}
	else if(firstArg == "combinemultisig")
	{
		combineMultisigSignatures(args);
	}
	else if(firstArg == "not_used_combinesig")	//Todo: Remove
	{
		not_used_combineNormalSignatures(args);
	}		
	else if(firstArg == "signatureinfo")	
	{
		signatureInfo(args);
	}	
	else if(firstArg == "showascii")
	{
		showAscii(args);
	}
	else if(firstArg == "fixlowstrans")
	{
		fixLowSTransaction(args);
	}
	else if(firstArg == "getcpp")
	{
		getCppString(args);
	}
	else if(firstArg == "outscript")
	{
		createOutScript(args);
	}
	else if(firstArg == "inscript")		//Todo: Remove, same function as outscript
	{
		createInScript(args);
	}	
*/
	//else if(firstArg == "not_used_partialsignmultisig")	//Todo: Remove???
	//{
	//	not_used_signMultisig(args, false);
	//}	
	else
	{
		std::cerr << "Unknown command: " << argv[1] << std::endl;
	}
	
}





