/**
 * bitcry.cpp
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
 

//#include <unistd.h>

#include "bigint.h"
#include "sha2.h"
#include "bitcoinEC.h"
#include "util.h"
#include "FileUtil.h"
#include "hash.h"
#include "base58.h"
#include "addressConversion.h"
#include "random.h"
#include "bitcoinKeyPair.h"

#include "randomUtils.h"
#include "version.h"

#include "commandLine.h"
#include "simpleFile.h"

#include <set>
#include <vector>
#include <iostream>


#include "aes.h"
#include "base64.h"

#include <string.h>

#include "encrypt.h"
#include "pkif.h"
#include "bitsigConfig.h"
#include "encryptionFileFormats.h"
#include "keychainFunc.h"
#include "signature.h"
#include "EncryptedResult.h"
#include "hmac.h"
#include <ctime>














BigInt<256> decyptXor(const ECPoint& R, const BigInt<1024>& privKey)
{
	const BitcoinEllipticCurve ec;
	
	const ECPoint otherS =  ec.multiply(R, privKey);
	//std::cout << "Other S: " << otherS << std::endl;
	const std::string sRawOther = otherS.getRaw(false); //getRawNonCompressed();
	//std::cout << "sRaw: " << printBin(sRaw) << std::endl;
	const std::string secretKeyOther = doubleSha256(sRawOther);
	const BigInt<256> secretOther(secretKeyOther, 'c');

	return secretOther;
}




//Todo: Return Pkif instead???!??!?
PubKey parsePointFromString(const bool isPkif, const std::string& str, bool& compressed)
{
	if(isPkif)
	{
		bool onlyIndicatedUsage(false);
		bool signVerify(false);

		const ECPoint Q = Pkif::parsePkif(str, compressed, onlyIndicatedUsage, signVerify);
		const PubKey pubKey(Q, compressed);
		return pubKey;
	}
	else
	{
		const PubKey pubKey = PubKey::fromHex(str);
		return pubKey;
	}

}


//Todo: Return Pkif instead???!??!?
ECPoint getPointForName(const std::string str, bool& compressed)
{
	const Pkif pkif = getPublicKeyForName(str);
	const PubKey pubKey = pkif.getPubKey();
	compressed = pubKey.compressed;
	
	const ECPoint Q = pubKey.point;
	return Q;
}




PubKey getPkifFromPubString(const std::string& publicKey)
{
	//Is this the PKIF format?
	if(publicKey.empty())
	{
		throw std::string("Error, empty recipient/public key");
	}

	const std::string givenRecipient(publicKey);
	
	const bool isPkif = (givenRecipient[0] == '7');
	const bool isAddress = (givenRecipient[0] == '1');

	const bool useName = (!isPkif && !isAddress);

	bool compressed(false);
	
	if(useName)
	{
		const ECPoint Q = getPointForName(givenRecipient, compressed);
		const PubKey pubKey(Q, compressed);
		return pubKey;		
	}
	else
	{
		const PubKey pubKey = parsePointFromString(isPkif, publicKey, compressed);
		//const ECPoint Q = parsePointFromString(isPkif, publicKey, compressed);
		//const PubKey pubKey(Q, compressed);
		return pubKey;		
	}
	
	/*
	//Receivers public key	
	const ECPoint Q = useName ? getPointForName(givenRecipient, compressed) : parsePointFromString(isPkif, publicKey, compressed);
	//const ECPoint Q = isPkif ? Pkif::parsePkif(publicKey, compressed) : parsePublicKey(publicKey, compressed);

	const PubKey pubKey(Q, compressed);
	return pubKey;
	*/
}







std::string encryptXorToFile(const BigInt<1024>& random, const PubKey& pubKey, const std::string& plainText, 
	std::string& filenameStart)
{
	if(plainText.size() > 32)
	{
		throw std::string("Error, at most 32 bytes can be encrypted");
	}
	
	const ECPoint userPublicPoint = pubKey.point;

	
	const EncryptedResult encR = EncryptedResult::encrypt(random, userPublicPoint);



	const std::string sRaw =  encR.secretPoint.getRaw(false); //getRawNonCompressed();	
	const std::string secretKey = doubleSha256(sRaw);	
	//std::cout << "SecretKey     : " << printBin(secretKey) << std::endl;	
	const BigInt<256> secret(secretKey, 'c');	
		

	const BigInt<256> plain = plainToBigInt(plainText);
	const BigInt<256> encrypted = plain ^ secret;
	
	const std::string encStr = encrypted.getBinaryLong(); //getBinaryVal(true);
	
	const std::string fileContent = createXorEncryptedStr(pubKey, encR.publicPoint, encrypted, plainText.size());

	filenameStart = encrypted.strHex().substr(0, 8);

	return fileContent;
}








void encryptXor(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cout << "Usage: bitcry encryptXor message recipient" << std::endl;
		std::cout << std::endl;
		std::cout << "Recipient can be pubkey, pkif or alias" << std::endl;
		return;			
	}

	std::string plainText(argv[2]);
	//std::cout << "Plaintext: " << plainText << std::endl;


	std::string recipient(argv[3]);
	//std::cout << "Using public key: " << publicKey << std::endl;

	
	if(plainText.size() > 32)
	{
		std::cout << "Plaintext size: " << plainText.size() << std::endl;
		throw std::string("Plaintext must be 32 chars or less");		
	}

	const PubKey pubKey = getPkifFromPubString(recipient);
	
	std::cout << "Generating random number" << std::endl;
	const std::string rStr = getFromDevRandom(32);
	
	const BigInt<1024> random(rStr, 'c');
	
	std::string filenameStart;
	const std::string fileContent = encryptXorToFile(random, pubKey, plainText, filenameStart);

	const std::string filename(filenameStart + ".bitcryx");
	std::cout << "Will write: " << filename << std::endl;
	
	fileWrite(filename, fileContent);	
}




std::string decryptXorEncrypted(const EncryptedFileData& encData, const Wif& wif)
{	
	const ECPoint R = encData.getR();
	
	std::string cParse = encData.enc;
	while(cParse.size() < 32)
	{
		cParse += '\0';
	}
	
	const BigInt<256> encrypted(cParse, 'c');
			
	const PrivKey privKey = wif.getPriv();
	
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = privKey.getPubkey(ec);
	const Pkif myPkif = pubKey.getPkif();
	
	const Pkif pkifFromEncFile = encData.pkif;
	if(myPkif.toString() != pkifFromEncFile.toString())
	{
		std::cout << "My pkif: " << myPkif.toString() << std::endl;
		std::cout << "Given pkif: " << pkifFromEncFile.toString() << std::endl;
		
		throw std::string("Error, encrypted with another key");
	}
	
	
	const BigInt<1024> priv(privKey.key); 


	const BigInt<256> secretOther = decyptXor(R, priv);
	

	const BigInt<256> decrypted = encrypted ^ secretOther;
	const std::string decStr = decrypted.getBinaryLong(); //getBinaryVal(true);
	const std::string plain = decStr.substr(0, encData.enc.size());
	
	return plain;
}




//Todo: Move to bitsigConfig
//Finds the wif corresponding to the given pkif
/*
Wif getMyWif(const std::string& wifStr, const Pkif& givenPkif)
{
	if(wifStr.empty())
	{
		std::cout << "Looking in key chain" << std::endl;
		std::string defPrivContent;
		const std::set<BitcoinKeyPair> privKeys = BitsigConfig::getOwnKeypairs(defPrivContent);
		//std::cout << "Num priv keys: " << privKeys.size() << std::endl;
		
		for(std::set<BitcoinKeyPair>::const_iterator it = privKeys.begin() ;
			it != privKeys.end() ;
			++it)
		{
			const BitcoinKeyPair& keypair = (*it);
			const Pkif myPkif = keypair.privaddr.getPkif();
			//std::cout << "Trying: " << keypair.pubaddr << std::endl;
			if(myPkif == givenPkif )
			{
				std::cout << "Found matching private key" << std::endl;
				std::cout << "  pkif: " << myPkif.toString() << std::endl;
				return keypair.privaddr;
			}
		}
		
		std::cout << "Did not find any matching private key for: " << givenPkif.toString() << std::endl;
		std::cout << "The private key should be imported or given on the command line" << std::endl;
		throw std::string("No matching private key found");
	}
	

	const Wif wif(wifStr);	
	return wif;
}
*/


//Todo: Move to bitsigConfig
//Finds the wif corresponding to the given pkif
Wif findTheWif(const std::string& wifStr, const Pkif& pkif)
{	
	if(wifStr.empty())
	{
		std::cout << "Looking in key chain" << std::endl;
		std::string defPrivContent;
		const std::set<BitcoinKeyPair> privKeys = BitsigConfig::getOwnKeypairs(defPrivContent);
		//std::cout << "Num priv keys: " << privKeys.size() << std::endl;
		
		for(std::set<BitcoinKeyPair>::const_iterator it = privKeys.begin() ;
			it != privKeys.end() ;
			++it)
		{
			const BitcoinKeyPair& keypair = (*it);
			//std::cout << "Trying: " << keypair.pubaddr << std::endl;
			
			//
			const Wif& mywif = keypair.privaddr;
			const PrivKey privKey = mywif.getPriv();
			BitcoinEllipticCurve ec;
			const PubKey pubKey = privKey.getPubkey(ec);
			const Pkif mypkif = pubKey .getPkif(); //mywif.getPkifDoNotUse();
			
			if(mypkif == pkif)
			{
				std::cout << "Found matching pkif: " << mypkif.toString() << std::endl;
				//std::cout << "  Pkif   : " <<  << std::endl;
				return mywif;
			}
		}
		
		std::cout << "Did not find any matching private key for: " << std::endl;
		std::cout << "  " << pkif.toString() << std::endl;
		std::cout << "The private key should be imported or given on the command line" << std::endl;
		throw std::string("No matching private key found");
	}
	

	const Wif wif(wifStr);	
	return wif;
}






	
void decryptXor(int argc, char* argv[])
{	
	if(argc < 3)
	{
		std::cout << "Usage: bitcry decryptXor messagefile [wif]" << std::endl;		
		return;			
	}

	const std::string filename(argv[2]);
	if(!endsWith(filename, ".bitcryx"))
	{
		throw std::string("Error, filename must end with .bitcryx");
	}


	std::string wifStr;
	if(argc > 3)
	{
		wifStr = argv[3];
		std::cout << "WIF: " << wifStr << std::endl;
	}
	//std::cout << "Filename: " << filename << std::endl;


	const std::string content = readFile(filename);
	
	//std::cout << "Wif     : " << wifStr << std::endl;
	
	const EncryptedFileData encData = parseXorEncryptedStr(content);
		
	const Wif wif = findTheWif/*getMyWif*/(wifStr, encData.pkif);
	const std::string plain = decryptXorEncrypted(encData, wif);	
	std::cout << "Decrypted plain: \"" << plain << "\"" << std::endl;
		
}









std::string decyptPointElectrum(const ECPoint& R, const BigInt<1024>& privKey, const bool compressed)
{
	const BitcoinEllipticCurve ec;
	
	const ECPoint otherS =  ec.multiply(R, privKey);
	const std::string sRawOther = otherS.getRaw(compressed);
	const std::string secretKeyOther = sha512(sRawOther);
	return secretKeyOther;	
}

















std::string decryptElectrumFormat(const std::string& encStr, const Wif& wif)
{
	const PrivKey myPrivKey = wif.getPriv();
	//bool compressed(false);	
	//const BigInt<256> myprivShort = wif.wifToPrivKey(compressed);
	const BigInt<1024> priv(BigInt<1024>::asPositive(myPrivKey.key/*myprivShort*/));
	
	//std::cout << "Priv: " << priv << std::endl;
	
	
	
	const std::string decoded = base64_decode(encStr);
	
	const std::string magic = decoded.substr(0, 4);
	const std::string emperalPubkey = decoded.substr(4, 33);
	const std::string receivedHmac = decoded.substr(decoded.size() - 32);	//????????

	const int cipherTextSize = decoded.size() - 37 - 32;

	
	//std::cout << "Magic: " << magic << std::endl;
	if(magic != "BIE1")
	{
		throw std::string("Incorrect magic string");
	}
	
	//std::cout << "EmpPubkey: " << printBin(emperalPubkey) << std::endl;
	
	bool comp(false);
	const ECPoint empKey = ECPoint::parsePointBin(emperalPubkey, comp);
	//std::cout << "EmpKey: " << empKey << std::endl;
	
	//Electrum always uses compressed format
	const std::string secret = decyptPointElectrum(empKey, priv, true);
	//std::cout << "Secret: " << printBin(secret) << std::endl;
	//std::cout << "Secret size: " << secret.size() << std::endl;
	
	std::string iv = secret.substr(0, 16);
	std::string keyE = secret.substr(16, 16);
	std::string keyM = secret.substr(32);
	
	//std::cout << "IV  : " << printBin(iv) << std::endl;
	//std::cout << "KeyE: " << printBin(keyE) << std::endl;
	//std::cout << "KeyM: " << printBin(not_used_keyM) << std::endl;
	
	
		
	//std::cout << "CipherTextSize: " << cipherTextSize << std::endl;
	
	const std::string ciphertext = decoded.substr(37, cipherTextSize);
	
	const std::string res = decryptAes128(keyE, iv, ciphertext);
	//std::cout << "Result size: " << res.size() << std::endl;
	

	
	const std::string toCalcMac = decoded.substr(0, decoded.size() - 32);	
	
	//std::cout << "TOTAL SIZE: " << decoded.size() << std::endl;
	//std::cout << "TO CALC MAC SIZE: " << toCalcMac.size() << std::endl;
	
	const std::string calcHmac = hmacSha256(keyM, toCalcMac);
	
	
	if(calcHmac != receivedHmac)
	{
		//std::cout << "Size ocalc mac: " << otherCalcMac.size() << std::endl;		
		//std::cout << "OCalcMa  : " << printBin(otherCalcMac) << std::endl;
		
		std::string errMsg("Error, hmac error:");
		
		errMsg += "CalcMac  : ";
		errMsg += hexString(calcHmac);
		
		errMsg += "ShouldMac: ";
		errMsg += hexString(receivedHmac);
		
		throw errMsg;
	}
	
	//std::cout << "RESULT: \"" << printBin(res) << "\"" << std::endl;
	//std::cout << "RESULT: \"" << res << "\"" << std::endl;

	return res;
}





void decryptElectrum(int argc, char* argv[])
{	
	if(argc < 3)
	{
		std::cout << "Usage: bitcry decryptElectrum encstr [wif]" << std::endl;
		return;			
	}

	const std::string encStr(argv[2]);
	
	
	std::string wifStr;
	
	if(argc > 3)
	{
		wifStr = argv[3];
	}
		
	if(!wifStr.empty())
	{
		const Wif wif(wifStr);
	
		const std::string message = decryptElectrumFormat(encStr, wif);
		std::cout << "Message: " << message << std::endl;

		return;
	}
	

	//Try to decrypt the message with all keys until success or no more keys		
	std::cout << "Looking in key chain" << std::endl;
	std::string defPrivContent;
	const std::set<Wif> privKeys = BitsigConfig::getOwnPrivate(defPrivContent);
	//std::cout << "Num priv keys: " << privKeys.size() << std::endl;
		
	for(std::set<Wif>::const_iterator it = privKeys.begin() ;
		it != privKeys.end() ;
		++it)
	{
		const Wif& priv = (*it);
		//std::cout << "Trying: " << keypair.pubaddr << std::endl;
		try
		{
			const std::string message = decryptElectrumFormat(encStr, priv);
			std::cout << std::endl;
			const PrivKey privKey = priv.getPriv(); 

			std::cout << "Success for key: " << privKey.toString() << std::endl;
			std::cout << "Message: " << message << std::endl;
			return;
		}
		catch(const std::string& err)
		{
			//std::cout << "Err: " << err << std::endl;
		}
			
		std::cout << "* " << std::flush;			
	}
		
	std::cout << std::endl;
	std::cout << "Could not decrypt message" << std::endl;
	throw std::string("Decrypt failed");	
}





std::string encryptElectrumToBase64(const BigInt<1024>& r, const std::string& plain, 
	const PubKey& pubKey)
{
	const ECPoint Q = pubKey.point;
	const bool compressed = pubKey.compressed;
	
	//const Encrypted enc = encrypt(r, Q);
	BitcoinEllipticCurve ec;
	
	const ECPoint R = ec.multiplyGenPoint(r);
	//std::cout << "R to publish: " << R << std::endl;
	
	const ECPoint S = ec.multiply(Q, r);	
	//std::cout << "S: " << S << std::endl;
	
	const std::string sRaw = S.getRaw(true); //Should always be compressed
	const std::string secret = sha512(sRaw);

	std::string iv = secret.substr(0, 16);
	std::string keyE = secret.substr(16, 16);
	std::string keyM = secret.substr(32);
	
	const std::string ciphertext = encryptAes128(keyE, iv, plain);
	
	//Create the compund data structure
	std::string encData;
	encData += "BIE1";
	encData += R.getRaw(true); //Always compressed for Electrum format
	encData += ciphertext;
	const std::string hmac = hmacSha256(keyM, encData);
	encData += hmac;

	const std::string b64data = base64_encode(encData);
	return b64data;
}




void encryptElectrum(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cout << "Usage: bitcry encryptElectrum plain receiverpubkey" << std::endl;		
		std::cout << "       Receiver can be alias or pubkey" << std::endl;		
		
		return;			
	}

	const std::string plain(argv[2]);
	const std::string receiver(argv[3]);
	
	std::cout << "Plain: " << plain << std::endl;
	std::cout << "Receiver: " << receiver << std::endl;


	const PubKey pubKey = getPkifFromPubString(receiver);
		
	std::cout << "Generating random number" << std::endl;
	const std::string rStr = getFromDevRandom(32);
	
	const BigInt<1024> r(rStr, 'c');
		
	const std::string b64data = encryptElectrumToBase64(r, plain, pubKey);
	
	std::cout << "EncStr: " << b64data << std::endl;
}





std::string decryptFileAesEncrypted(const EncryptedFileData& encData, const Wif& wif)
{	
	const ECPoint R = encData.getR();
	
	const PrivKey myPrivKey = wif.getPriv();
		
	//bool compressed(false);	
	//const BigInt<256> myprivShort = wif.wifToPrivKey(compressed);
	const BigInt<1024> priv(BigInt<1024>::asPositive(myPrivKey.key/*myprivShort*/));

	const PrivKey privKey = wif.getPriv();

	BitcoinEllipticCurve ec;

	//Get the pkif for the wif			
	const PubKey mypubKey = privKey.getPubkey(ec);
	const Pkif mypkif = mypubKey.getPkif();

	
	const Pkif bitAddressFromEncFile = encData.pkif;
	if(mypkif.toString() != bitAddressFromEncFile.toString())
	{
		std::cout << "My bitcoin key   : " << mypkif.toString() << std::endl;
		std::cout << "Given bitcoin key: " << bitAddressFromEncFile.toString() << std::endl;
		
		throw std::string("Error, encrypted with another key");
	}
	
	


	const ECPoint myS = EncryptedResult::decypt(R, priv);

	//std::cout << "The shared secret point:" << std::endl;
	//std::cout << myS << std::endl;
	
	const std::string sRaw = myS.getRaw(myPrivKey.compressed);
	const std::string secret = sha512(sRaw);

	std::string iv = secret.substr(0, 32);
	std::string keyE = secret.substr(32, 32);
	//std::cout << "IV : " << printBin(iv) << std::endl;
	//std::cout << "KEY: " << printBin(keyE) << std::endl;
		
	const std::string decrypted = decryptAes256(keyE, iv, encData.enc);
	return decrypted;
}











void decryptVerifyFile(const std::vector<std::string>& args)
{
	//std::cout << "WILL VERIFY AND DECRYPT" << std::endl;
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitcry decryptVerifyFile bitcrysig [publicKey]" << std::endl;
		return;			
	}
		
	if(args.size() > 4)
	{
		throw std::string("Too many arguments");
	}
		
	const std::string signedFile(args[1]);
	std::cout << "The sign file: " << signedFile << std::endl;


	std::string wifStr;
	if(args.size() > 2)
	{
		wifStr = args[2];
	}
	
	//std::cout << "WifStr: " << wifStr << std::endl;
			
	//std::cout << "The signature file   : " << signedFile << std::endl;
	//Check that it end with ".bitsig"	
	
	const std::string fileEnd(".bitcrysig");
	//const std::string ending = signedFile.substr(signedFile.size() - 8);

	if(!endsWith(signedFile, fileEnd))
	{
		std::cout << "The signature file must end with " << fileEnd << ":" << signedFile << std::endl;
		throw std::string("Incorrect signature file");
	}

	std::string alias;

	std::string pubadd;
	if(args.size() > 3)		//Address given on command line
	{
		pubadd = args[3];
	}


	std::string verifiedAddress;
	const std::string signedContent = SignatureImplementation::verifySignatureForFile(signedFile, pubadd, alias, verifiedAddress, true);


	std::cout << "Verify OK for address: " << verifiedAddress << std::endl;
	if(!alias.empty())
	{
		std::cout << "Address alias: " << alias << std::endl;
	}

	std::cout << "Will decrypt data" << std::endl;
	const EncryptedFileData encData = parseFileEncryptedStr(signedContent);


	const Wif theWif = findTheWif(wifStr, encData.pkif);
	const PrivKey privKey =theWif.getPriv();

	
	const std::string decryptedStr = decryptFileAesEncrypted(encData, theWif);

	std::cout << std::endl;
	std::cout << std::endl;	
	std::cout << "Verified from: " << verifiedAddress << std::endl;
	std::cout << "Decrypted to : " << privKey.toString() << std::endl;


	//Save embedded file 
	const std::string saveFilename = std::string("out_") + removeEnding(signedFile, fileEnd);
		
	std::cout << "Saving to: " << saveFilename << std::endl;
	fileWrite(saveFilename, decryptedStr);		
	
}




std::string getEncryptdFile(const BigInt<1024>& r, const PubKey& pubKey, const std::string& plainText)
{
	const ECPoint Q = pubKey.point;

	const EncryptedResult e = EncryptedResult::encrypt(r, Q);

	//std::cout << "The shared secret point:" << std::endl;
	//std::cout << e.secretPoint << std::endl;
	
	const std::string sRaw = e.secretPoint.getRaw(pubKey.compressed);
	const std::string secret = sha512(sRaw);

	const std::string iv = secret.substr(0, 32);
	const std::string keyE = secret.substr(32, 32);
	
	//std::cout << "IV : " << printBin(iv) << std::endl;
	//std::cout << "KEY: " << printBin(keyE) << std::endl;
	
	
	
	const std::string encrypted = encryptAes256(keyE, iv, plainText);
	
	const std::string fileContent = createFileEncryptedStr(pubKey, e.publicPoint, encrypted, plainText.size());
		
	return fileContent;
}







void encryptSignFile(int argc, char* argv[])
{	
	if(argc < 4)
	{
		std::cout << "Usage: bitcry encryptSign filename recipient [wif]" << std::endl;
		std::cout << std::endl;
		std::cout << "Recipient can be pubkey, pkif or alias" << std::endl;
		
		throw std::string("Missing argument");
		//return;
	}

	std::string plainFilename(argv[2]);
	std::cout << "Filename: " << plainFilename << std::endl;

	const std::string plainText = readFile(plainFilename);


	std::string recipient(argv[3]);	

	const PubKey pubKey = getPkifFromPubString(recipient);

	const ECPoint Q = pubKey.point;
	
	//Get the bitcoin address from the public key
	//PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pkif.point, pkif.compressed);
	//std::cout << "Encrypting for bitcoin address: " << bitcoinAddress << std::endl;
	
	std::cout << "Encrypting for: " << pubKey.toString() << std::endl;

	std::cout << "Generating random number" << std::endl;
	const std::string rStr = getFromDevRandom(32);
	
	const BigInt<1024> randomNum(rStr, 'c');
	

	const std::string encryptedFileContent = getEncryptdFile(randomNum, pubKey, plainText);

	
		
	//Now sign the encrypted content
	
	bool compressed(false);	
	const BigInt<256> privKey = getPrivKeyForSigning(compressed);
		
	std::cout << "Generating 32 random bytes for signature. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');
		
	const std::string sigEncResult = SignatureImplementation::signString(encryptedFileContent, privKey, tempKey, compressed, true);
		
	const std::string filename(plainFilename + ".bitcrysig");
	std::cout << "Will write: " << filename << std::endl;	
	fileWrite(filename, sigEncResult);	
}










void encryptFile(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cout << "Usage: bitcry encrypt filename recipient" << std::endl;
		std::cout << std::endl;		
		std::cout << "Recipient can be pubkey, pkif or alias" << std::endl;
		throw std::string("Missing argument");
		//return;	
	}

	std::string plainFilename(argv[2]);
	std::cout << "Filename: " << plainFilename << std::endl;

	const std::string plainText = readFile(plainFilename);

	std::string recipient(argv[3]);	

	const PubKey pubKey = getPkifFromPubString(recipient);
		
	std::cout << "Encrypting to: " << pubKey.toString() << std::endl;

	std::cout << "Generating random number" << std::endl;
	const std::string rStr = getFromDevRandom(32);
	
	const BigInt<1024> random(rStr, 'c');
	
	const std::string fileContent = getEncryptdFile(random, pubKey, plainText);

	const std::string filename(plainFilename + ".bitcry");
	std::cout << "Will write: " << filename << std::endl;
	
	fileWrite(filename, fileContent);	
}












std::string getPlainFilename(const std::string& cryptFilename)
{
	const std::string bitcry(".bitcry");
	
	if(!endsWith(cryptFilename, bitcry))
	{
		throw std::string("Error, filename must end with .bitcry");
	}

	std::string withoutEnding = cryptFilename.substr(0, cryptFilename.size() - bitcry.size());
	
	const int slashPos = withoutEnding.rfind("/");
	if(slashPos != std::string::npos)
	{
		withoutEnding = withoutEnding.substr(slashPos + 1);
	}
	
	const std::string filename = "out_" + withoutEnding;
	return filename;
}


bool isWhitespace(unsigned char c)
{
	if(c == ' ' || c == '\t' || c == '\r')
	{
		return true;
	}
	
	return false;
}







std::string trimRow(const std::string& row)
{
	//std::cout << "Will trim: " << row << std::endl;
	int startPos(0);
	for(int i = 0 ; i < row.size() ; i++)
	{
		const unsigned char c = row[i];
		if(!isWhitespace(c))
		{
			startPos = i;
			break;
		}
	}
	
	int endPos(row.size() - 1);
	for(int i = (row.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char c = row[i];
		if(!isWhitespace(c))
		{
			endPos = i + 1;
			break;			
		}
	}
	
	//std::cout << "Start: " << startPos << " End: " << endPos << std::endl;
	
	const std::string trimmed = row.substr(startPos, endPos - startPos);
	return trimmed;
}



std::string canonicalizeWhitespace(const std::string& str)
{
	//std::cout << "Will canonicalize" << std::endl;
	
	std::list<std::string> rows;
	int lastPos(0);
	for(int i = 0 ; i < str.size() ; i++)
	{
		const unsigned char c = str[i];
		if(c == '\n')
		{
			const std::string row = str.substr(lastPos, i - lastPos);
			rows.push_back(row);
			lastPos = i + 1;
		}
	}

	//std::cout << "Will create result" << std::endl;

	std::string ret;
	for(std::list<std::string>::const_iterator it = rows.begin() ;
		it != rows.end() ;
		++it)
	{
		const std::string& row = (*it);
		const std::string trimmedRow = trimRow(row);
		
		ret += trimmedRow;
		ret += "\n";
	}

	//std::cout << "Ready canon: " << ret << std::endl;
	return ret;
}





void decryptFile(int argc, char* argv[])
{	
	if(argc < 3)
	{
		std::cout << "Usage: bitcry decrypt messagefile [wif]" << std::endl;		
		throw std::string("Missing argument");
		//return;
	}

	const std::string filename(argv[2]);
	if(!endsWith(filename, ".bitcry"))
	{
		throw std::string("Error, filename must end with .bitcry");
	}


	std::string wifStr;
	if(argc > 3)
	{
		wifStr = argv[3];
		std::cout << "WIF: " << wifStr << std::endl;
	}

	const std::string rawContent = readFile(filename);
	const std::string content = canonicalizeWhitespace(rawContent);
		
	const EncryptedFileData encData = parseFileEncryptedStr(content);
		
	const Wif theWif = findTheWif(wifStr, encData.pkif);
	
	const std::string decryptedStr = decryptFileAesEncrypted(encData, theWif);

	//const std::string decryptedStr = findWifAndDecrypt(wifStr, encData);
	
	const std::string outFile = getPlainFilename(filename);
	std::cout << "Writing file: " << outFile << std::endl;
	fileWrite(outFile, decryptedStr);		
}



void listCryptoKeys(int argc, char* argv[])
{
	std::cout << "Public keys for own private keys used for signing or decryption: " << std::endl;
		
	std::cout << std::endl;
	BitsigConfig::listOwnPublicKeys();

	std::cout << std::endl;
	std::cout << std::endl;
		
	std::cout << "Public keys from others (without any private key)" << std::endl;
	std::cout << " used for encryption or signature verification: " << std::endl;
	std::cout << std::endl;	
	BitsigConfig::listPublicKeys();
	
	std::cout << std::endl;
}		



void testOneEncryption(const Pkif& pkif, const Wif& wif, const BigInt<1024>& random)
{
	std::cout << "Testing encrypt/decrypt " << std::flush;
	const PubKey pubKey = pkif.getPubKey();

	const ECPoint Q = pubKey.point;
	const PrivKey pk = wif.getPriv();
	const BigInt<1024> privKey(BigInt<1024>::asPositive(pk.key)); //wif.toPrivKey()));
	
	const EncryptedResult encRes = EncryptedResult::encrypt(random, Q);
		
	const ECPoint secretPoint = EncryptedResult::decypt(encRes.publicPoint, privKey);		
				
	if(encRes.secretPoint != secretPoint)
	{
		throw std::string("Encrypt/decrypt error");
	}

	std::cout << "OK" << std::endl;
}



void testEncryptionFunction(const std::string& pkifStr, const BigInt<1024>& random, 
		const std::string& encPubPointRes, const std::string& encSecPointRes)
{
	std::cout << "Testing encryption..." << std::flush;
	//const Pkif pkif = Pkif::parsePkif(pkifStr);
	const PubKey pubKey = PubKey::fromPkif(pkifStr);
	
	const ECPoint publicPoint = pubKey.point;
	//std::cout << "The public point: " << publicPoint.getCompressedHex() << std::endl;
		
	
	
	const EncryptedResult encRes = EncryptedResult::encrypt(random, publicPoint);
	
	const std::string encPubPoint = hexString(encRes.publicPoint.getRaw(true)); //getCompressedHex();
	const std::string encSecPoint = hexString(encRes.secretPoint.getRaw(true)); //getCompressedHex();
	
	//std::cout << "Encrypted public point: " << std::endl << encPubPoint << std::endl;
	//std::cout << "Encrypted secret point: " << std::endl << encSecPoint << std::endl;

	if(encPubPoint != encPubPointRes)
	{
		throw std::string("Encryption error");
	}
	
	if(encSecPoint != encSecPointRes)
	{
		throw std::string("Encryption error");
	}
	
	std::cout << "OK" << std::endl;
}



void testElectrumEncryptDecrypt(const BigInt<1024>& random, const std::string& plain, 
	const Pkif& pkif, const Wif& wif)
{
	std::cout << "Testing Electrum encrypt/decrypt..." << std::flush;
	
	const PubKey pubKey = pkif.getPubKey();
	const std::string b64data = encryptElectrumToBase64(random, plain, pubKey);
	
	const std::string plainAgain1 = decryptElectrumFormat(b64data, wif);

	if(plainAgain1 != plain)
	{
		throw std::string("Encrypt/decrypt error");
	}
	
	std::cout << "OK" << std::endl;
}







void testFileEncryptAndDecrypt(const BigInt<1024>& fileRandom1, const Pkif& filePkif1, 
	const std::string& filePlaintext1, const Wif& fileWif1)
{	
	std::cout << "Testing file encrypt/decrypt..." << std::flush;
	
	const PubKey pubKey = filePkif1.getPubKey();
	
	const std::string fileContent = getEncryptdFile(fileRandom1, pubKey, filePlaintext1);
		
	const EncryptedFileData encData = parseFileEncryptedStr(fileContent);
		
	
	const std::string decryptedStr = decryptFileAesEncrypted(encData, fileWif1);
	//std::cout << "DECRYPTED STR: " << decryptedStr << std::endl;
	if(decryptedStr !=  filePlaintext1)
	{
		throw std::string("File encryption/decryption error");
	}

	std::cout << "OK" << std::endl;
}








void testXorEncryption(const BigInt<1024>& xorRandom, const Pkif& xorPkif, 
	const std::string& xorPlaintext, const Wif& xorWif)
{
	std::cout << "Testing xor encryption..." << std::flush;
	std::string filenameStart;
	
	const PubKey pubKey = xorPkif.getPubKey();
	
	const std::string xorFileContent = encryptXorToFile(xorRandom, pubKey, xorPlaintext, filenameStart);

	//std::cout << "XOR file content: " << xorFileContent << std::endl;


	const EncryptedFileData xorEncData = parseXorEncryptedStr(xorFileContent);
		
	const std::string xorPlain = decryptXorEncrypted(xorEncData, xorWif);		
	//std::cout << "PLAIN: " << xorPlain << std::endl;

	if(xorPlain != xorPlaintext)
	{
		throw std::string("xor encryption/decryption error");
	}
	
	std::cout << "OK" << std::endl;
}




void testEncrypt(int argc, char* argv[])
{
	std::cout << "Encryption and decryption test" << std::endl;

// = Pkif::parsePkif
	const Pkif my_pkif("7UX4y34nDMb47JPAikfrrJcBtg8hMbs1Be5pKYu85zpjbUUanNq");
	const Wif my_wif("5JFsYBHRSthm5iq5gKRZmkvJ9eE3S2US364zQff5T7jVzh9z4bc");
	const BigInt<1024> my_random(BigInt<1024>::fromHex("123456789012345678901234567890"));
	
	testOneEncryption(my_pkif, my_wif, my_random);


	const BigInt<1024> xorRandom(BigInt<1024>::fromHex("7483748374837378378378378374234293429749837372387427348383883492"));
	const BigInt<1024> xorRandom2(BigInt<1024>::fromHex("7848893838398383983948394839849384384938493849384938493893883889"));	
	const Pkif xorPkif("7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z");
	const std::string xorPlaintext("Test this xor encryption");
	const Wif xorWif("5JPDuHCAdqqqKc9npzReULWEJKyHVxwQ3QNHRZWUVxY4K2owyG7");


	testXorEncryption(xorRandom, xorPkif, xorPlaintext, xorWif);
	testXorEncryption(xorRandom, xorPkif, "tesing 1234", xorWif);
	testXorEncryption(xorRandom, xorPkif, xorPlaintext, xorWif);
	testXorEncryption(xorRandom, xorPkif, "testing 1234", xorWif);
	testXorEncryption(xorRandom, xorPkif, "12345678901234567890123456789012", xorWif);
	
	
	std::string filenameStart;
	const PubKey xorPubKey = xorPkif.getPubKey();

	const std::string xorFileContent = encryptXorToFile(xorRandom, xorPubKey, xorPlaintext, filenameStart);

	//std::cout << "XOR file content: " << xorFileContent << std::endl;

	
const std::string shouldBeXorVal("------ BEGIN BITCOIN SECP256K1 XOR ENCRYPT ------\n\
7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z\n\
03110B49CC48273F86DE4168E9E5A2A606DD15F92CA17B27EA0FCE610FC0CE0003\n\
8197AE58A74C25B89EA0C97AB86C366D4FA6F160E200EE6A\n\
------ END BITCOIN SECP256K1 XOR ENCRYPT ------\n");

	if(xorFileContent != shouldBeXorVal)
	{
		throw std::string("Xor encrypt error");
	}

	std::cout << "Will test encrypt and decrypt 1" << std::endl;


	const BigInt<1024> fileRandom2(BigInt<1024>::fromHex("4574774477474747747477477477474747747474774747474747477474747422"));
	const std::string filePlaintext2("A little test text");
	const Pkif filePkif2("7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z");
	const Wif fileWif2("5JPDuHCAdqqqKc9npzReULWEJKyHVxwQ3QNHRZWUVxY4K2owyG7");

	testFileEncryptAndDecrypt(fileRandom2, filePkif2, filePlaintext2, fileWif2);

	std::cout << "Will test encrypt and decrypt 2" << std::endl;

	const BigInt<1024> fileRandom3(BigInt<1024>::fromHex("4748738437843783748374837483748374874837483748374837438748787888"));
	const std::string filePlaintext3("abcdefghijklmnopqrstuvwxyz");
	const Pkif filePkif3("7SKGaq2Xe7h2PWcS2Pzr3BUnJNFZnc9M26krF2hBb9afKHWhDup");
	const Wif fileWif3("5Jwj48UEmN3pQxVPFGwFhn1UUZc7wCTcDfdwuDAPkfV8r1f7es6");

	testFileEncryptAndDecrypt(fileRandom3, filePkif3, filePlaintext3, fileWif3);

	std::cout << "Will test encrypt and decrypt 3" << std::endl;


	const BigInt<1024> fileRandom1(BigInt<1024>::fromHex("1122334455667788990011223344556677889900112233445566778899001122"));
	const std::string filePlaintext1("My text");
	const Pkif filePkif1("7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z");
	const Wif fileWif1("5JPDuHCAdqqqKc9npzReULWEJKyHVxwQ3QNHRZWUVxY4K2owyG7");



	testFileEncryptAndDecrypt(fileRandom1, filePkif1, filePlaintext1, fileWif1);
	
	const PubKey filePubKey1 = filePkif1.getPubKey();
	
	const std::string fileContent = getEncryptdFile(fileRandom1, filePubKey1, filePlaintext1);

	//std::cout << "File content: " << fileContent << std::endl;
const std::string shouldBe = "------ BEGIN BITCOIN SECP256K1 FILE ENCRYPT ------\n\
7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z\n\
0380038951DF186D2F439AA633C332F30F1D78DE48E09DDDFBB42F6118BCC91170\n\
7B34B27E58B23B3405DFE8D31E872D97D9E5F1A164918CA39895C7AC7D970434\n\
------ END BITCOIN SECP256K1 FILE ENCRYPT ------\n";

	if(fileContent != shouldBe)
	{
		throw std::string("Incorrect encrypted file");
	}

		
		
	const EncryptedFileData encData = parseFileEncryptedStr(fileContent);
		
	
	const std::string decryptedStr = decryptFileAesEncrypted(encData, fileWif1);
	//std::cout << "DECRYPTED STR: " << decryptedStr << std::endl;
	if(decryptedStr !=  filePlaintext1)
	{
		throw std::string("File encryption/decryption error");
	}


	const BigInt<1024> eran2(BigInt<1024>::fromHex("1122334455667788990011223344556677889900112233445566778899001122"));
	const std::string plain2("Testing testing");
	const Pkif epkif2("7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z");
	const Wif mywif2("5JPDuHCAdqqqKc9npzReULWEJKyHVxwQ3QNHRZWUVxY4K2owyG7");
	

	testElectrumEncryptDecrypt(eran2, plain2, epkif2, mywif2);
	

	const BigInt<1024> eran3(BigInt<1024>::fromHex("8273483274832742874832748783783728374328748327428748237482738437"));
	const std::string plain3("This is a test with longer text");
	const Pkif epkif3("7SY2SGt1eNrdNudi1hjX1EH1m4BEYYUn6Gm3Hi6K5DET6GukxqE");
	const Wif mywif3("5KgJXBNGx4R86o9vSR4kkcvMJFdyuVzoY3T5rnq8frBnSscbD1L");
	
	testElectrumEncryptDecrypt(eran3, plain3, epkif3, mywif3);

	const BigInt<1024> eran4(BigInt<1024>::fromHex("8273483274832742874832748783783728374328748327428748237482738437"));
	const std::string plain4("");	//Empty
	const Pkif epkif4("7UvgNYitF2gwWx1YuPbHWaufSY5VvTKgrMSmbshmHdVN8hSAAHR");
	const Wif mywif4("5Ki2TEngU4QqF29HYtbbX6ihYGYMMZia94ugoTAaWBtnV1DrzPV");
	
	testElectrumEncryptDecrypt(eran4, plain4, epkif4, mywif4);


	const BigInt<1024> eran1(BigInt<1024>::fromHex("1122334455667788990011223344556677889900112233445566778899001122"));
	const std::string plain1("Testing testing");
	const Pkif epkif1("7Tdsk4ThHZn2n5eDXqEg5ZXn9zFh5qTrUqh6WUaaMranuicCY2Z");
	const Wif mywif1("5JPDuHCAdqqqKc9npzReULWEJKyHVxwQ3QNHRZWUVxY4K2owyG7");
	
	const PubKey epPubKey1 = epkif1.getPubKey();
	
	const std::string b64data = encryptElectrumToBase64(eran1, plain1, epPubKey1);
	
	//std::cout << "B64: " << b64data << std::endl;
	if(b64data != "QklFMQOAA4lR3xhtL0OapjPDMvMPHXjeSOCd3fu0L2EYvMkRcOWQXlhjZojGrg/eOOAIxxkmHGFGlnCwq3drb1+D83b1yTy9xyMeZSZ55j3hyQyAOQ==")
	{
		throw std::string("Electrum encrypt error");
	}

	const std::string plainAgain1 = decryptElectrumFormat(b64data, mywif1);

	if(plainAgain1 != plain1)
	{
		throw std::string("Encrypt/decrypt error");
	}

	const std::string mypkif1("7SSoHYNShXH1YA2wu4HUvjEEwPAgnfTuw99fw6h59Y8GFvynK6d");
	const BigInt<1024> ran1(BigInt<1024>::fromHex("1122334455667788990011223344556677889900112233445566778899001122"));
	const std::string encPubPointRes1("0380038951DF186D2F439AA633C332F30F1D78DE48E09DDDFBB42F6118BCC91170");
	const std::string encSecPointRes1("02E5FFD54CCEFB2BCE4AC4BB2AE62058CCBA77BFA9248E6BBBEBA46B29070DA587");

	testEncryptionFunction(mypkif1, ran1, encPubPointRes1, encSecPointRes1);
	
	const std::string mypkif2("7SK6PeMz4AXx9YMU2mGhtftLRMYSiR6qTW4EaWqcsgf1eJYxATx");
	const BigInt<1024> ran2(BigInt<1024>::fromHex("11228787878787ABBCBBBCFEFDFFFE8884993939399939398329842983493893"));
	const std::string encPubPointRes2("03CE5EDFAE9FEB255DC4D2CE2DC542441F44523E3FD8655ADBF5D478416FEBFD9D");
	const std::string encSecPointRes2("020E1B2BFF1ABFC222D0A992F06156764AF9050424BC75A68904A791CAD4D3D1EB");

	testEncryptionFunction(mypkif2, ran2, encPubPointRes2, encSecPointRes2);
	

	const std::string mypkif3("7V4xjdS8efJbUANNDNRQ1qWQXJxcqt3pG3GvNTirFL769h7So6q");
	const BigInt<1024> ran3(BigInt<1024>::fromHex("7272828828273912873198273128739817987837384278743874382374982891"));
	const std::string encPubPointRes3("026D6CE978CCE9DE6FFEA46FEFC2215DD921DA5DB25233003F6F56DF772A5041C8");
	const std::string encSecPointRes3("02F5503C0311EA3890BA15A4CE527EFA9CC40F8B718CD3B896EF01A619659AC413");

	testEncryptionFunction(mypkif3, ran3, encPubPointRes3, encSecPointRes3);




	const Pkif pkif1("7UfcqyLa85XqxpXdzpKzHiyXaub4op2Sd14FkiawTosqgkZgkGc");	
	const Wif wif1("5KS4DkEiFdTgmLqbnZtHKzsKjtoBEfuxCFc4ReEZiHtWoD8ptqT");
	const BigInt<1024> random1(BigInt<1024>::fromHex("123456789012345678901234567890"));
	
	testOneEncryption(pkif1, wif1, random1);


	const Pkif pkif2("7VAY8CKDXDzUdzKauCAVSEqxX3U62TcmAQSKCXVHmub98H4aNme");
	//const std::string pkif2("7VAY8CKDXDzUdzKauCAVSEqxX3U62TcmAQSKCXVHmub98H4aNme");
	const Wif wif2("5Hvp2h47HpMz3fstVypvsVAuBxBQZKbeZDvtSpnR4T6etiDDFoH");
	const BigInt<1024> random2(BigInt<1024>::fromHex("456456456456456456445435345345345"));
	
	testOneEncryption(pkif2, wif2, random2);
	
	std::cout << "All tests passed OK" << std::endl;
}







void printCryptUsage()
{
	std::cout << std::endl;						
	std::cout << "Usage: bitcry command [options]" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Commands for key creation:" << std::endl;
	std::cout << "    random" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Commands for key import:" << std::endl;
	std::cout << "    import | importPrivate" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Commands for key chain display:" << std::endl;
	std::cout << "    list" << std::endl;
	std::cout << std::endl;

	
	std::cout << "Commands for file encryption:" << std::endl;
	std::cout << "   encrypt | decrypt" << std::endl;
	std::cout << std::endl;

	std::cout << "Commands for Electrum message encryption:" << std::endl;
	std::cout << "  encryptElectrum | decryptElectrum" << std::endl;
	std::cout << std::endl;
		
	std::cout << "Project homepage: http://bitcoin-gen.org/" << std::endl;
	std::cout << "Email: bitgen@openmailbox.org" << std::endl;
	std::cout << std::endl;
}




void doWork(int argc, char* argv[])
{			
	time_t t = time(0);
	const std::string thetime = removeNewline(asctime(localtime(&t)));
	
	
	std::vector<std::string> args;
	for(int i = 1 ; i < argc ; i++)
	{
		args.push_back(argv[i]);		
	}
	
	
	
	
	if(argc < 2)
	{
		//printSigUsage();
		std::cout << "For help, use \"bitcry help\"" << std::endl;
		std::cout << std::endl;
		
		return;
	}


	const std::string firstArg(argv[1]);

	if(firstArg == "version" || firstArg == "-v")
	{
		std::cout << "bitcry version " << versionNumber << std::endl;
		std::cout << "http://www.bitcoin-gen.org/" << std::endl;		
	}
	else if(firstArg == "help")
	{
		printCryptUsage();
	}
	else if(firstArg == "dice")
	{
		generatePrivateFromDice(argc, argv);
	}		
	else if(firstArg == "random")
	{
		generatePrivateFromRandom(args);
	}
	else if(firstArg == "brainwallet")
	{
		generatePrivateFromBrainwallet(argc, argv);		
	}
	else if(firstArg == "hash")
	{
		generatePrivateFromHash(argc, argv);		
	}
	else if(firstArg == "hex")
	{
		createAddressFromHex(argc, argv);
	}	
	else if(firstArg == "encryptSign")
	{
		encryptSignFile(argc, argv);
	}
	else if(firstArg == "decryptVerify")
	{
		decryptVerifyFile(args);
	}			
	else if(firstArg == "encrypt")
	{
		encryptFile(argc, argv);
	}	
	else if(firstArg == "decrypt")
	{
		decryptFile(argc, argv);	
	}	
	else if(firstArg == "encryptXor")
	{
		encryptXor(argc, argv);
	}	
	else if(firstArg == "decryptXor")
	{
		decryptXor(argc, argv);	
	}
	else if(firstArg == "decryptElectrum")
	{	
		decryptElectrum(argc, argv);
	}
	else if(firstArg == "encryptElectrum")
	{	
		encryptElectrum(argc, argv);
	}
	else if(firstArg == "test")
	{				
		testEncrypt(argc, argv);	
	}	
	else if(firstArg == "list")
	{
		listCryptoKeys(argc, argv);	
	}
	else if(firstArg == "listPublic")
	{
		listThePublicKeys(args);		
	}		
	else if(firstArg == "import")
	{
		importPublicKey(argc, argv);		
	}
	else if(firstArg == "importPrivate")
	{	
		importPrivateKey(argc, argv);
	}
	else if(firstArg == "setDef")
	{
		setDefaultPrivatekey(argc, argv);		
	}
	else if(firstArg == "delete")
	{
		deleteKey(argc, argv);		
	}		
	else
	{
		//std::cerr << "Unknown bitcry command: " << argv[1] << std::endl;
		std::string errMsg("Unknown bitcry command: ");
		errMsg += argv[1];
		throw errMsg;
		//return;
	}

	
	return;
}




int main(int argc, char* argv[])
{				
	try
	{
		doWork(argc, argv);
	}
	catch(const std::string& err)
	{
		std::cerr << "Error: " << err << std::endl;
		return 1;
	}
	
	return 0;
}


	
