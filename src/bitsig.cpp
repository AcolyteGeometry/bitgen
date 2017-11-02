/**
 * bitsig.cpp
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
#include <sys/types.h>
//#include <pwd.h>
#include <ctime>
#include <dirent.h>

#include <set>



#include "hash.h"
#include "base58.h"
#include "addressConversion.h"
#include "signature.h"
#include "sigFile.h"
#include "random.h"
#include "bitcoinKeyPair.h"

#include "bitsigFileFormat.h"
#include "bitsigConfig.h"

#include "randomUtils.h"
#include "version.h"
#include "base64.h"
#include "signature.h"
#include "signatureFileFormats.h"

#include "commandLine.h"
#include "simpleFile.h"
#include "pkif.h"

#include "keychainFunc.h"







void printSigUsage()
{
	std::cout << std::endl;						
	std::cout << "Usage: bitsign command [options]" << std::endl;
		
	std::cout << "Commands for generating private keys (stored in keychain): " << std::endl;
	std::cout << "   hex|dice|random|hash|(brainwallet)" << std::endl;		
	std::cout << std::endl;
	

	std::cout << "Commands for listing keys in the keychain: " << std::endl;	
	std::cout << "    list|listOwn|listPublic" << std::endl;
	std::cout << std::endl;

	std::cout << "Commands for importing keys to the keychain: " << std::endl;	
	std::cout << "    import|importPrivate" << std::endl;
	std::cout << std::endl;

	std::cout << "Other keychain commands: " << std::endl;		
	std::cout << "    setDef|delete" << std::endl;	
	std::cout << std::endl;

	
	std::cout << "Commands for signing and signature verifcation of files: " << std::endl;	
	std::cout << "   sign|verify" << std::endl;	
	std::cout << std::endl;

	std::cout << "Commands for raw signing and signature verifcation: " << std::endl;	
	std::cout << "   signRaw|verifyRaw" << std::endl;	
	std::cout << std::endl;

	std::cout << "Commands for signing messages in specific formats: " << std::endl;	
	std::cout << "  signMultibit | signArmory | signAb64 | signAclear | signInputsIo" << std::endl;	
	std::cout << std::endl;

	std::cout << "Commands for verifying messages in specific formats: " << std::endl;	
	std::cout << "  verifyMultibit | verifyArmory | verifyAb64 | verifyAclear | verifyInputsIo " << std::endl;	
	std::cout << std::endl;

	std::cout << "Command for generic message verification" << std::endl;
	std::cout << "  verifyMessage" << std::endl;
	std::cout << std::endl;
		
	std::cout << std::endl;
	std::cout << "Example, create a new private key in keychain: " << std::endl;
	std::cout << "    bitsign random" << std::endl;		
	std::cout << std::endl;
		
	std::cout << "Project homepage: http://bitcoin-gen.org/" << std::endl;
	std::cout << "Email: bitgen@openmailbox.org" << std::endl;
	std::cout << std::endl;
}













bool not_used_checkSig(const BigInt<256>& hash, const ECPoint& pubkey, const RSVal& rs)
{
	//Does this public key correspond to the bitcoin address?

	if(!SignatureImplementation::verifySign(rs, pubkey, hash))
	{
		std::cout << "Error in signature!!!!!" << std::endl;
		throw std::string("Error in signature!!!!!");
	}
	
	//std::cout << "Verify OK for alias: " << std::endl;
	
	return true;
}




void testSigning(const Wif& myWif)
{
	//BigInt<256> privKey = myWif.toPrivKey();
	const PrivKey pk = myWif.getPriv();
	const BigInt<256> tempKey(BigInt<256>::fromHex("83738783783738738787877d7d77d7d7d8888d88d88d8d8d888d8d98a88a7a89"));
	const BigInt<256> hashNum(BigInt<256>::fromHex("1276356356636663678388888383838838383883987239872893787f9f987f88"));

	//const PrivKey priv(
	BitcoinEllipticCurve ec;

	//const ECPoint pubkey = getPublicPointFromPrivateBinary(pk.key);
	const PubKey p = pk.getPubkey(ec); //(pubkey, false);
	const PubAddress pubAddr = p.getAddress();
	
	//PubAddress pubAddr = PubAddress::mybitcoinAddresFromPubkey(pubkey, false);
	
	std::cout << "Test sig for: " << pubAddr;
		
	const RSVal rs = SignatureImplementation::sign(pk.key, tempKey, hashNum);
		
	//Verify signature to make sure it is valid		
		
		
	if(!SignatureImplementation::verifySign(rs, p.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in signature!!!!!" << std::endl;
		throw std::string("Error in signature!!!!!");
	}
	
	std::cout << "      OK" << std::endl;
}





std::string lenBytes(const int inSize)
{
	const BigInt<64> size(inSize);
	std::string str;
	
	if(inSize < 0xfd)
	{
		const unsigned char lenByte(inSize);
		str += lenByte;
		return str;
	}
	else if(inSize < 0xffff)
	{
		const BigInt<8> fd(0xfd);		
		str += fd.getBinaryLong(); //getBinaryVal(true);
		
		const BigInt<8> low(size);
		str += low.getBinaryLong(); //getBinaryVal(true);

		const BigInt<8> high(size >> 8);		
		str += high.getBinaryLong(); //getBinaryVal(true);
		
		//std::cout << "Will return: " << printBin(str);
				
		return str;
	}
	else if(inSize < 0xffffffff)
	{
		const BigInt<8> fe(0xfe);		
		str += fe.getBinaryLong(); //getBinaryVal(true);
		
		const BigInt<8> b1(size);
		str += b1.getBinaryLong(); //getBinaryVal(true);

		const BigInt<8> b2(size >> 8);		
		str += b2.getBinaryLong(); //getBinaryVal(true);

		const BigInt<8> b3(size >> 16);
		str += b3.getBinaryLong(); //getBinaryVal(true);

		const BigInt<8> b4(size >> 24);
		str += b4.getBinaryLong(); //getBinaryVal(true);
		
		//std::cout << "Will return: " << printBin(str);
				
		return str;		
	}
	else
	{
		throw std::string("Error, message too large");
	}
	
	return "";
}



















//Todo: Make it possible to sign terabyte files
std::string hashStringFullFile(const std::string& message)
{	
	const std::string sigStart("Bitcoin Signed File ");
	const std::string lengthStr = intToString(message.size());
	const std::string fullMsg(sigStart + lengthStr + " bytes:" + message);
	//std::cout << "Will hash: " << fullMsg << std::endl;
	
	return fullMsg;
}






std::string hashStringQt(const std::string& message)
{
    const unsigned char eighteen(0x18);	
	std::string hashInputStart;
	hashInputStart += eighteen;
	hashInputStart += "Bitcoin Signed Message:\n";
	hashInputStart += lenBytes(message.size());
	
	
	const std::string toHash(hashInputStart + message);

	return toHash;
}


std::string hashStringArmory(const std::string& message)
{
	const std::string sigStart("Bitcoin Signed Message:\n");
	const std::string fullMsg(sigStart + message);
	
	return fullMsg;
}






void signArmoryMessage(const std::vector<std::string>& args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signArmory Message" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
		
	bool compressed(false);


	const BigInt<256> privKey = getPrivKeyForSigning(compressed);
	
	//Armory does NOT use compressed addresses	
	
	if(compressed)
	{
		throw std::string("Error, armory does not support compressed address");
	}

	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');

	const std::string fullMsg = hashStringArmory(message);
	
	const std::string theHash = doubleSha256(fullMsg);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);

	BitcoinEllipticCurve ec;
		
		
	//Verify signature to make sure it is valid		
	const PrivKey pk(privKey, compressed);
	const PubKey myPub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = myPub.getAddress();  // pk.getAddress(BitcoinEllipticCurve ec);

	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);			
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, compressed);
		
	if(!SignatureImplementation::verifySign(rs, myPub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in armory signature!!!!!" << std::endl;
		throw std::string("Error in armory signature!!!!!");
	}


	const std::string sigResult = generateArmoryFilFormat(myPub.point /*pubkey*/, rs, bitcoinAddress, message);
		
	//const std::string outname = "signed.armory";
	const BigInt<32> hashStart(hashNum >> 224);		
	const std::string outname = hexString(hashStart.getBinaryLong() /*getBinaryVal(true)*/) +".armory";
	
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
	
	
}









void signInputsIoMessage(const std::vector<std::string>& args)
{
	//todo
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signInputsIo Message" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
		
/*
	std::string WIFStr;

	if(args.size() >= 3)
	{
		WIFStr = args[2];
	}
*/
	bool compressed(false);


	const BigInt<256> privKey = getPrivKeyForSigning(/*WIFStr,*/ compressed);

	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	const std::string toHash = hashStringQt(message);
	
	
	const std::string theHash = doubleSha256(toHash);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);
		
		
	//Verify signature to make sure it is valid
	BitcoinEllipticCurve ec;
	
	//Armory does NOT use compressed addresses	
	const PrivKey pk(privKey, false);
	const PubKey pub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = pub.getAddress();	
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	//Armory does NOT use compressed addresses	
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		
	if(!SignatureImplementation::verifySign(rs, pub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in iio signature!!!!!" << std::endl;
		throw std::string("Error in iio signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pub.point /*pubkey*/, hashNum);


	const std::string sigResult = generateInputsIoFileFormat(pub.point /*pubkey*/, rs, bitcoinAddress, message, iap, false);
		
	const BigInt<32> hashStart(hashNum >> 224);		
	const std::string outname = hexString(hashStart.getBinaryLong() /*getBinaryVal(true)*/) +".inputsio";
			
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
	
	
}










void signRaw(const std::vector<std::string>& args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signRaw Message" << std::endl;
		return;			
	}
	
	if(args.size() > 2)
	{
		throw std::string("Too many arguments");
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
		
	bool compressed(false);


	const BigInt<256> privKey = getPrivKeyForSigning(compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	
	const std::string toHash = hashStringQt(message);
	
	
	const std::string theHash = doubleSha256(toHash);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);

	BitcoinEllipticCurve ec;

		
	//Armory does NOT use compressed addresses	
	const PrivKey pk(privKey, false);
	const PubKey pub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = pub.getAddress();
		
	//Verify signature to make sure it is valid
		
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	//Armory does NOT use compressed addresses	
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		
	if(!SignatureImplementation::verifySign(rs, pub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in raw signature!!!!!" << std::endl;
		throw std::string("Error in raw signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pub.point /*pubkey*/, hashNum);

	const std::string sigResult = encodeRs(rs, iap, compressed); 
	std::cout << "Signature: " << sigResult << std::endl;	
}












void signMultibitMessage(const std::vector<std::string>& args)
{
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signMultibit Message" << std::endl;
		return;			
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
		
/*
	std::string WIFStr;

	if(args.size() >= 3)
	{
		WIFStr = args[2];
	}
*/
	bool compressed(false);


	const BigInt<256> privKey = getPrivKeyForSigning(/*WIFStr,*/ compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	
	const std::string toHash = hashStringQt(message);		
	const std::string theHash = doubleSha256(toHash);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);

	BitcoinEllipticCurve ec;
	//Armory does NOT use compressed addresses	
	const PrivKey pk(privKey, false);
	const PubKey pub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = pub.getAddress();
		
		
	//Verify signature to make sure it is valid
		
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	//Armory does NOT use compressed addresses	
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		
	if(!SignatureImplementation::verifySign(rs, pub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in mb signature!!!!!" << std::endl;
		throw std::string("Error in mb signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pub.point /*pubkey*/, hashNum);


	const std::string sigResult = generateMultibitFileFormat(pub.point /*pubkey*/, rs, bitcoinAddress, message, iap, false);
	
	const BigInt<32> hashStart(hashNum >> 224);		
	const std::string outname = hexString(hashStart.getBinaryLong()) +".multibit";
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
	
	
}









void signFile(const std::vector<std::string>& args)
{
	bool compressed(false);
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsign signFile messageFile [priveateHex]" << std::endl;
		return;			
	}
		
	if(args.size() > 2)
	{
		throw std::string("Too many arguments");
	}
	const std::string messageFile(args[1]);
		
	std::cout << "Signing message file: " << messageFile << std::endl;

	const std::string content = readFile(messageFile);

	
	//SimpleFile theFile(messageFile);

	const BigInt<256> privKey = getPrivKeyForSigning(compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	const std::string sigResult = SignatureImplementation::signString(content, privKey, tempKey, compressed, false);

	const std::string outname = messageFile + ".bitfsig";
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
}






void signMessage(const std::vector<std::string>& args)
{
	bool compressed(false);
	
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsign sign messageFile [priveateHex]" << std::endl;
		return;			
	}
		
	if(args.size() > 2)
	{
		throw std::string("Too many arguments");
	}
	const std::string messageFile(args[1]);
		
	std::cout << "Signing message file: " << messageFile << std::endl;

	
	SimpleFile theFile(messageFile);

	const BigInt<256> privKey = getPrivKeyForSigning(compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');
	
			
	
	//Find the length of the file

	Sha256 sha256obj;		
	
	const int fileSize = theFile.fileSize();
	//std::cout << "File size: " << fileSize << std::endl;
	const std::string hashStart = SignatureImplementation::hashStringStart(fileSize);
	sha256obj.add(hashStart);
		
	for(;;)
	{
		const std::string chunk = theFile.getChunk();
		if(chunk.empty())
		{
			break;
		}
		
		sha256obj.add(chunk);
	}	
	
	//std::cout << "Ready with first hash" << std::endl;
	const std::string firstHash = sha256obj.getHash();
		
	const std::string theHash = sha256(firstHash);
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);
		
		
	//Verify signature to make sure it is valid
	BitcoinEllipticCurve ec;
	
	const PrivKey pk(privKey, compressed);
	const PubKey pubk = pk.getPubkey(ec);
		
	const ECPoint pubkey = pubk.point; //getPublicPointFromPrivateBinary(privKey);
		
	if(!SignatureImplementation::verifySign(rs, pubkey, hashNum))
	{
		std::cout << "Error in message signature!!!!!" << std::endl;
		std::cout << "r: " << rs.r << std::endl;
		std::cout << "s: " << rs.s << std::endl;
		std::cout << "pubkey: " << pubkey << std::endl;
		std::cout << "hashnum: " << hashNum << std::endl;
		
		throw std::string("Error in message signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pubkey, hashNum);

	const BigInt<256> r(rs.r);
	const BigInt<256> s(rs.s);
		
	std::string sigResult = createSignatureContent(hashNum, r, s, iap.indexZero, iap.parityOdd, compressed);
			
	const std::string outname = messageFile + ".bitsig";
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
}








void signArmoryClear(const std::vector<std::string>& args)
{
	std::cout << "Will sign armory clear" << std::endl;
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signAclear Message" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
		
	bool compressed(false);
	const BigInt<256> privKey = getPrivKeyForSigning(compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	
	const std::string toHash = hashStringQt(message);		
	const std::string theHash = doubleSha256(toHash);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);
		


	BitcoinEllipticCurve ec;
	//Armory does NOT use compressed addresses	
	const PrivKey pk(privKey, false);
	const PubKey pub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = pub.getAddress();
		
	//Verify signature to make sure it is valid
		
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	//Armory does NOT use compressed addresses	
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		
	if(!SignatureImplementation::verifySign(rs, pub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in signature!!!!!" << std::endl;
		throw std::string("Error in signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pub.point /*pubkey*/, hashNum);


	const std::string sigResult = generateArmoryClearFileFormat(message, rs, iap, false);
	
	const BigInt<32> hashStart(hashNum >> 224);		
	const std::string outname = hexString(hashStart.getBinaryLong() /* getBinaryVal(true) */) +".armoryclear";
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;	
}






void signArmoryBase64(const std::vector<std::string>& args)
{
	std::cout << "Will sign armory base 64" << std::endl;
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig signAb64 Message" << std::endl;
		throw std::string("Missing argument");
		//return;
	}
	
	const std::string message(args[1]);
		
	std::cout << "Signing message: " << message << std::endl;
	
	if(args.size() > 2)
	{
		throw std::string("Too many arguments given");
	}
	
		
	bool compressed(false);
	const BigInt<256> privKey = getPrivKeyForSigning(compressed);


	std::cout << "Generating 32 random bytes. " << std::endl;
	std::cout << "Press random keys or move the mouse if needed" << std::endl;
	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');


	
	const std::string toHash = hashStringQt(message);		
	const std::string theHash = doubleSha256(toHash);	
	const BigInt<256> hashNum(theHash, 'c');


	std::cout << "Signing in progress... " << std::endl;
		
	const RSVal rs = SignatureImplementation::sign(privKey, tempKey, hashNum);
		
		
	BitcoinEllipticCurve ec;
	//Armory does NOT use compressed addresses	
	const PrivKey pk(privKey, false);
	const PubKey pub = pk.getPubkey(ec);
	const PubAddress bitcoinAddress = pub.getAddress();
		
		
	//Verify signature to make sure it is valid
		
	//const ECPoint pubkey = getPublicPointFromPrivateBinary(privKey);
		
	//Armory does NOT use compressed addresses	
	//const PubAddress bitcoinAddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
		
	if(!SignatureImplementation::verifySign(rs, pub.point /*pubkey*/, hashNum))
	{
		std::cout << "Error in signature!!!!!" << std::endl;
		throw std::string("Error in signature!!!!!");
	}


	const IndexAndParity iap = getIndexAndParity(rs, pub.point /*pubkey*/, hashNum);


	const std::string sigResult = generateArmoryA46FileFormat(message, rs, iap, false);
	
	const BigInt<32> hashStart(hashNum >> 224);		
	const std::string outname = hexString(hashStart.getBinaryLong() /*getBinaryVal(true)*/) +".armoryb64";
	
	fileWrite(outname, sigResult);
	std::cout << std::endl << "Result written to: " << outname << std::endl;
	
	
}
	






void checkArmoryClear(const std::string& armData, const std::string& message, const std::string& pubadd)
{
	const std::string infobyteAndRs = base64_decode(armData);
	
	if(infobyteAndRs.size() != 65)
	{
		throw std::string("Signature string size error");
	}
	
	std::cout << "Signed message: \"" << message << "\"" << std::endl;


	bool firstX(false);
	bool oddVal(false);
	bool compressed(false);
	
	const RSVal rs = decode65SignatureBytes(infobyteAndRs, firstX, oddVal, compressed);
	
	const std::string toHash = hashStringQt(message);	
	const std::string hash = doubleSha256(toHash);
	const BigInt<256> hashNum(hash, 'c');


	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(firstX, oddVal, rs, hashNum);
	const PubKey pub(pubkey, compressed);
	const PubAddress bitaddress = pub.getAddress();
	
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, compressed);
	std::cout << "Signed by address: " << bitaddress << std::endl;
	
	
	
	std::string alias;


	
	SignatureImplementation::verifyPubaddrSpecifiedByUser(pubadd, bitaddress, alias);
		
	std::cout << "Verifying signature..." << std::endl;
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, rs))
	{
		std::cout << "Verify OK for Armory clear file" << std::endl;
		std::cout << "Signature address: " << bitaddress << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
}







void verifyArmoryClear(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig verifyAclear filename" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
				
	const std::string filename(argv[2]);
		
	std::string pubadd;
	if(argc > 3)
	{
		pubadd = argv[3];
	}
		
		
	const std::string content = readFile(filename);
	std::cout << "Verifying Armory clear signed message: " << filename << std::endl;
		
	//std::cout << "Will parse: " << filename << std::endl;
		
	std::string message;	
	const std::string armData = parseArmoryClear(content, message);

	checkArmoryClear(armData, message, pubadd);	
}





void checkArmoryBase64(const std::string& armData, const std::string& pubadd)
{
	const std::string theSigAndMessage = base64_decode(armData);
	//std::cout << "Decoded size: " << theSigAndMessage.size() << std::endl;
	if(theSigAndMessage.size() < 65)
	{
		throw std::string("Signature string too short");
	}
	

	const std::string infobyteAndRs = theSigAndMessage.substr(0, 65);
	const std::string message = theSigAndMessage.substr(65);
	std::cout << "Signed message: \"" << message << "\"" << std::endl;


	bool firstX(false);
	bool oddVal(false);
	bool compressed(false);
	
	const RSVal rs = decode65SignatureBytes(infobyteAndRs, firstX, oddVal, compressed);
	
	const std::string toHash = hashStringQt(message);	
	const std::string hash = doubleSha256(toHash);
	const BigInt<256> hashNum(hash, 'c');


	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(firstX, oddVal, rs, hashNum);
	const PubKey pub(pubkey, compressed);
	const PubAddress bitaddress = pub.getAddress();
		
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, compressed);
	std::cout << "Signed by address: " << bitaddress << std::endl;
	
	
	
	std::string alias;


	
	SignatureImplementation::verifyPubaddrSpecifiedByUser(pubadd, bitaddress, alias);
		
	std::cout << "Verifying signature..." << std::endl;
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, rs))
	{
		std::cout << "Verify OK for Armory base 64 file" << std::endl;
		std::cout << "Signature address: " << bitaddress << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
}	








void verifyArmoryBase64(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig verifyAb64 filename" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
		
	std::string pubadd;
	if(argc > 3)
	{
		pubadd = argv[3];
	}
		
	const std::string filename(argv[2]);
		
	const std::string content = readFile(filename);
	std::cout << "Verifying Armory base64 signed message: " << filename << std::endl;
		
	//std::cout << "Will parse: " << filename << std::endl;
		
	const std::string armData = parseArmoryBase64(content);

	checkArmoryBase64(armData, pubadd);	
}











void verifyFile(const std::vector<std::string>& args)
{
	//std::cout << "Verifying message sign" << std::endl;
		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig verifyFile bitfsigFile [publicKey]" << std::endl;
		return;			
	}
		
	if(args.size() > 4)
	{
		throw std::string("Too many arguments");
	}
		
	const std::string signedFile(args[1]);
	std::cout << "The sign file: " << signedFile << std::endl;
			
	//std::cout << "The signature file   : " << signedFile << std::endl;
	//Check that it end with ".bitsig"		
	const std::string ending = signedFile.substr(signedFile.size() - 8);
		
	if(ending != ".bitfsig")
	{
		std::cout << "The signature file must end with .bitfsig, this ends with " << ending << std::endl;
		throw std::string("Incorrect signature file");
	}

	std::string alias;

	std::string pubadd;
	if(args.size() > 3)		//Address given on command line
	{
		pubadd = args[3];
	}


	std::string verifiedAddress;
	const std::string signedContent = SignatureImplementation::verifySignatureForFile(signedFile, pubadd, alias, verifiedAddress, false);

	std::cout << "Verify OK for address: " << verifiedAddress << std::endl;
	if(!alias.empty())
	{
		std::cout << "Address alias: " << alias << std::endl;
	}
		
	//Save embedded file 
	const std::string saveFilename = std::string("out_") + removeEnding(signedFile, ".bitfsig");
		
	std::cout << "Saving to: " << saveFilename << std::endl;
	fileWrite(saveFilename, signedContent);		
}










void verifyMessage(const std::vector<std::string>& args)
{
	//std::cout << "Verifying message sign" << std::endl;
		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig verify signedFile [bitsigFile] [publicKey]" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
		
	if(args.size() > 4)
	{
		throw std::string("Too many arguments");
	}
		
	const std::string signedFile(args[1]);
	std::cout << "The signed file      : " << signedFile << std::endl;
		


	std::string signFile;
	if(args.size() < 3)
	{
		signFile = signedFile + ".bitsig";
		std::cout << "Assuming sig file    : " << signFile << std::endl;
	}
	else
	{
		signFile = args[2];
	}
	
	std::cout << "The signature file   : " << signFile << std::endl;
	//Check that it end with ".bitsig"		
	const std::string ending = signFile.substr(signFile.size() - 7);
		
	if(ending != ".bitsig")
	{
		std::cout << "The signature file must end with .bitsig, this ends with " << ending << std::endl;
		throw std::string("Incorrect signature file");
	}

		
	const SigFileContent sigParam = parseSignFile(signFile);
	
	//Calculate the hash of the file		
	
	SimpleFile theFile(signedFile);
	
	Sha256 sha256obj;		
	
	const int fileSize = theFile.fileSize();
	//std::cout << "File size: " << fileSize << std::endl;
	const std::string hashStart = SignatureImplementation::hashStringStart(fileSize);
	sha256obj.add(hashStart);
		
	for(;;)
	{
		const std::string chunk = theFile.getChunk();
		if(chunk.empty())
		{
			break;
		}
		
		sha256obj.add(chunk);
	}	
	
	//std::cout << "Ready with first hash" << std::endl;
	const std::string firstHash = sha256obj.getHash();
		
	const std::string theHash = sha256(firstHash); 		
	const BigInt<256> hashNum(theHash, 'c');


	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(sigParam.firstX, sigParam.oddY, sigParam.rs, hashNum);
	const PubKey pub(pubkey, sigParam.compressed);
	const PubAddress bitaddress = pub.getAddress();
			
	//Does this pubkey give the bitcoin address?			
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, sigParam.compressed);			
	
	std::cout << "Calculated address   : " << bitaddress << std::endl;
	
	std::string alias;

	std::string pubadd;
	if(args.size() > 3)		//Address given on command line
	{
		pubadd = args[3];
	}

	SignatureImplementation::verifyPubaddrSpecifiedByUser(pubadd, bitaddress, alias);
					
	std::cout << "Verifying signature...." << std::endl;
		
	//Require lowS value
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, sigParam.rs, true))
	{		
		std::cout << "Verify OK for address: " << bitaddress << std::endl;
		if(!alias.empty())
		{
			std::cout << "Address alias: " << alias << std::endl;
		}
	}


		
}







void checkInputsIo(const InputsIoData& data)
{
	//Todo: Make class for the three bools
	bool firstX(false);
	bool oddVal(false);
	bool compressed(false);
	
	const RSVal rs = decodeBase64Signature(data.signature, firstX, oddVal, compressed);
	
	//Calculate the hash
	const std::string toHash = hashStringQt(data.message);
	const std::string hash = doubleSha256(toHash);
	const BigInt<256> hashNum(hash, 'c');

	//Now calculate the public key x and y
	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(firstX, oddVal, rs, hashNum);
	const PubKey pub(pubkey, compressed);
	const PubAddress bitaddress = pub.getAddress();	
		
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, compressed);
	//std::cout << "The address: " << bitaddress << std::endl;
	if(bitaddress.toString()/*.address*/ != data.address)
	{
		std::cout << "Addresses do not match: " << std::endl;
		std::cout << "Recovered from rs: " << bitaddress.toString() /*address*/ << std::endl;
		std::cout << "Specified in file: " << data.address << std::endl;		
		
		throw std::string("Error, bitcoin address does not match");
	}
	
	
	std::cout << "Verifying signature..." << std::endl;
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, rs))
	{
		std::cout << "Verify OK for input.io file" << std::endl;
		std::cout << "Signature address: " << bitaddress << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
}








void verifyInputsIo(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig verifyInputsIo filename" << std::endl;
		return;			
	}
		
		
	const std::string filename(argv[2]);
		
	const std::string content = readFile(filename);
	std::cout << "Verifying Inputs IO signed message: " << filename << std::endl;
		
	//std::cout << "Will parse: " << filename << std::endl;
		
	const InputsIoData data = parseInputsIo(content);


	checkInputsIo(data);	
}







void verifyRaw(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::cout << "Usage: bitsig verifyRaw message signature [signAddress]" << std::endl;
		return;			
	}
	
	if(argc > 4)
	{
		throw std::string("Too many arguments");
	}

				
	const std::string message(argv[2]);
	const std::string signature(argv[3]);

	std::string givenAddress = (argc > 4) ? argv[4] : "";
	//std::cout << "The given address: " << givenAddress << std::endl;
	
		
	std::cout << "Verifying raw message: " << message << std::endl;
		

	bool firstX(false);
	bool oddVal(false);
	bool compressed(false);
	
	const RSVal rs = decodeBase64Signature(signature, firstX, oddVal, compressed);
	
	//Calculate the hash

    //"\x18Bitcoin Signed Message:\n" + len(message) + message
    	
	const std::string toHash = hashStringQt(message);	
	const std::string hash = doubleSha256(toHash);
	const BigInt<256> hashNum(hash, 'c');


	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(firstX, oddVal, rs, hashNum);
	const PubKey pub(pubkey, compressed);
	const PubAddress bitaddress = pub.getAddress();	
	
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, compressed);
	if(!givenAddress.empty())
	{
		if(bitaddress.toString() /*address*/ != givenAddress)
		{
			std::cout << "Got address: " << bitaddress.toString() /*address*/ << std::endl;
			throw std::string("Address mismatch");
		}
	}
		
	std::cout << "Verifying signature..." << std::endl;
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, rs))
	{
		std::cout << "Signed by the address: " << bitaddress << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
	
}



	

void checkMultibitMessage(const InputsIoData& data)
{
	//std::cout << std::endl;


	bool firstX(false);
	bool oddVal(false);
	bool compressed(false);
	
	const RSVal rs = decodeBase64Signature(data.signature, firstX, oddVal, compressed);
	
	//Calculate the hash

    //"\x18Bitcoin Signed Message:\n" + len(message) + message
    
	const std::string toHash = hashStringQt(data.message);	
	const std::string hash = doubleSha256(toHash);
	const BigInt<256> hashNum(hash, 'c');


	//Now calculate the public key x and y
	const ECPoint pubkey = SignatureImplementation::recoverThePublicKey(firstX, oddVal, rs, hashNum);
	//std::cout << "The point: " << pubkey << std::endl;
	const PubKey pub(pubkey, false);
	const PubAddress bitaddress = pub.getAddress();
		
	//const PubAddress bitaddress = PubAddress::bitcoinAddresFromPubkey(pubkey, false);
	//std::cout << "The address: " << bitaddress << std::endl;
	if(bitaddress.toString() /* address */ != data.address)
	{
		std::cout << "Addresses do not match: " << std::endl;
		std::cout << "Recovered from rs: " << bitaddress.toString() /*address*/ << std::endl;
		std::cout << "Specified in file: " << data.address << std::endl;		
		
		throw std::string("Error, bitcoin address does not match");
	}
	
	//std::cout << "Will check signature" << std::endl;
	
	std::cout << "Verifying signature..." << std::endl;
	if(SignatureImplementation::checkSigLowS(hashNum, pubkey, rs))
	{
		std::cout << "Verify OK for multibit file" << std::endl;
		std::cout << "Signature address: " << bitaddress << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
}





void verifyMultibitMessage(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "Usage: bitsig verifyInputsIo filename" << std::endl;
		return;			
	}
	
	//std::cout << "VERIFYING MULTIBIT" << std::endl;	
		
	const std::string filename(argv[2]);
		
	const std::string content = readFile(filename);
	std::cout << "Verifying Multibit signed message: " << filename << std::endl;
		
	//std::cout << "Will parse: " << filename << std::endl;
		
	const InputsIoData data = parseMultibit(content);

	checkMultibitMessage(data);
}















void checkArmoryMessage(const ArmoryFileContent& sigParam)
{	
	const PubKey pub(sigParam.pubkey, false);
	const PubAddress b58 = pub.getAddress();
	
	//PubAddress b58 = PubAddress::bitcoinAddresFromPubkey(sigParam.pubkey, false);
	if(sigParam.address != b58)
	{
		throw std::string("Error, bitcoin address does not match");
	}

		
	if(SignatureImplementation::checkSigLowS(sigParam.hash, sigParam.pubkey, sigParam.rs))
	{
		std::cout << "Verify OK for armory file" << std::endl;
		std::cout << "Signature address: " << b58 << std::endl;
	}
	else
	{
		std::cout << "Signature error" << std::endl;
	}
}






void verifyArmoryMessage(int argc, char* argv[])
{		
	if(argc < 3)
	{
		std::cout << "Usage: bitsig verifyArmory filename" << std::endl;
		throw std::string("Missing argument");		
		//return;			
	}
		
		
	const std::string filename(argv[2]);
		
	const std::string content = readFile(filename);
	std::cout << "Verifying armory hex signed message: " << filename << std::endl;
		
	//std::cout << "Will parse: " << filename << std::endl;
		
	const ArmoryFileContent sigParam = ParseArmory(content);
	
	checkArmoryMessage(sigParam);	
}









std::string determineFormat(const std::string& content)
{	
	const std::string armoryClearStart("-----BEGIN BITCOIN SIGNED MESSAGE-----");
	const std::string armoryHexStart("-----BEGIN-SIGNATURE-BLOCK-------------------------------------");
	const std::string bitcoinStart("-----BEGIN BITCOIN MESSAGE-----");
	const std::string inputsIoMiddle("-----BEGIN SIGNATURE-----");

	
	if(content.find(armoryClearStart) != std::string::npos)
	{
		//std::cout << "Armory clear, inputsio or multibit message" << std::endl;
		
		if(content.find(inputsIoMiddle) != std::string::npos)
		{
			//std::cout << "InputsIo" << std::endl;
			return "i";
		}
		else
		{
			//std::cout << "Armory clear or multibit" << std::endl;
			const std::string armoryMiddle("-----BEGIN BITCOIN SIGNATURE-----\n");
			
			const int middlePos = content.find(armoryMiddle);
			if(middlePos == std::string::npos)
			{
				throw std::string("Could not determine format");
			}
			
			const int sectionStart = middlePos + armoryMiddle.size();
			const std::string firstChar = content.substr(sectionStart, 1);
			if(firstChar.size() != 1)
			{
				throw std::string("Could not determine the format");				
			}
			//std::cout << "First char: " << firstChar << std::endl;
			const char c = firstChar[0];
			if(c == '\n')
			{
				//std::cout << "armory clear" << std::endl;
				return "ac";				
			}
			else
			{
				//std::cout << "multibit" << std::endl;
				return "m";
			}
		}
		
		throw std::string("Could not determine the format");
	}

	if(content.find(bitcoinStart) != std::string::npos)
	{
		//std::cout << "Armory b64 message" << std::endl;
		return "ab";
	}

	if(content.find(armoryHexStart) != std::string::npos)
	{
		//std::cout << "Armory hex message" << std::endl;
		return "ah";		
	}


	std::cout << "Unknown format" << std::endl;
	
	throw std::string("Unknown format");
	return "unknown";
}








void verifyAnyMessage(const std::vector<std::string>& args)
{		
	if(args.size() < 2)
	{
		std::cout << "Usage: bitsig verifyMessage filename" << std::endl;
		throw std::string("Missing argument");
		//return;			
	}
		
		
	const std::string filename(args[1]);
	std::cout << "The filename: " << filename << std::endl;

	std::string pubaddr;
	if(args.size() > 2)
	{
		pubaddr = args[2];
	}

			
	const std::string content = readFile(filename);

	//Determine which format it is
	const std::string formatStr = determineFormat(content);
	//std::cout << "Format: " << formatStr << std::endl;

	if(formatStr == "ah")
	{	
		std::cout << "Format: Armory hex" << std::endl;
		const ArmoryFileContent sigParam = ParseArmory(content);
		checkArmoryMessage(sigParam);		
	}
	else if(formatStr == "m")
	{	
		std::cout << "Format: Multibit" << std::endl;
		
		const InputsIoData data = parseMultibit(content);
		checkMultibitMessage(data);		
	}
	else if(formatStr == "ab")
	{	
		std::cout << "Format: Armory base64" << std::endl;
		
		const std::string armData = parseArmoryBase64(content);
		checkArmoryBase64(armData, pubaddr);
		
	}
	else if(formatStr == "ac")
	{
		std::cout << "Format: Armory clear" << std::endl;
		
		std::string message;	
		const std::string armData = parseArmoryClear(content, message);
		
		checkArmoryClear(armData, message, pubaddr);		
	}
	else if(formatStr == "i")
	{
		std::cout << "Format: InputsIo" << std::endl;
		
		const InputsIoData data = parseInputsIo(content);
		checkInputsIo(data);		
	}
	else
	{
		std::cout << "Unknown format: " << formatStr << std::endl;
	}
	
}



void testOfSignature()
{
	const std::string compressed_key("0314fc03b8df87cd7b872996810db8458d61da8448e531569c8517b469a119d267");
	//ECPoint uncomp
	const PubKey pk = PubKey::fromHex(compressed_key);
		
	//std::cout << std::endl << std::endl;
		
	std::cout << "Comp: " << compressed_key << std::endl;
	std::cout << "Uncomp: " << pk.point /*uncomp*/ << std::endl;
		
	const BigInt<1024> xShouldBe(BigInt<1024>::fromHex("14fc03b8df87cd7b872996810db8458d61da8448e531569c8517b469a119d267"));
	const BigInt<1024> yShouldBe(BigInt<1024>::fromHex("be5645686309c6e6736dbd93940707cc9143d3cf29f1b877ff340e2cb2d259cf"));
		
	if(pk.point.x != xShouldBe)
	{
		throw std::string("Error in x val");
	}
		
	if(pk.point.y != yShouldBe)
	{
		throw std::string("Error in y val");
	}


	const std::string uncompStr = hexString(pk.point.getRaw(false)); //getNonCompressedHex(); //getRawNonCompressed();
	if(uncompStr != "0414FC03B8DF87CD7B872996810DB8458D61DA8448E531569C8517B469A119D267BE5645686309C6E6736DBD93940707CC9143D3CF29F1B877FF340E2CB2D259CF")
	{
		std::cout << "Got: " << uncompStr << std::endl;
		throw std::string("Error in uncompressed string");			
	}

		
	//return;
	const BigInt<1024> myVal(9);
	BitcoinEllipticCurve ec;
	
	const BigInt<1024> sqMyVal = ec.modNsqrt(myVal);
	std::cout << "sqrt(" << myVal << ")=" << sqMyVal << std::endl;

	BigInt<1024> N(BitcoinEllipticCurve::Nval());

	const BigInt<1024> squared = (sqMyVal * sqMyVal) % N;

	std::cout << "Square: " << squared << std::endl;
		
	std::cout << "Performing software tests" << std::endl;
	
	const BigInt<1024> a(6);
	const BigInt<1024> b(30);
	//const BigInt<1024> rrrr = a.powerLarge(b);
	
	//std::cout << a << "^" << b << "=" << rrrr << std::endl;
		
	const BigInt<1024> x(2);
	const BigInt<1024> y(1);
		
	const BigInt<1024> expRes = ec.pow_mod(x, y);
		
	std::cout << x << "^" << y << " = " << expRes << std::endl;

	//const BigInt<1024> N(BitcoinEllipticCurve::N());

	BigInt<1024> myResult(x);
	const int loopEnd = y.intVal();
		
	for(int i = 1 ; i < loopEnd ; i++)
	{			
		myResult = (myResult * x) % N;
	}
		
	std::cout << x << "^" << y << " = " << myResult << std::endl;
		
		
	std::cout << "sign tests start" << std::endl;
		
	//asdasd
	
	const std::string sigStart("Bitcoin Signed Message:\n");
	const std::string fullMsg(sigStart +"This is an example of a signed message.");
	const std::string elecHashRaw = sha256(sha256(fullMsg));
	const BigInt<256> elecHash(elecHashRaw, 'c');
		
	//const BigInt<1024> elecX(BigInt<1024>::fromHex("a34b99f22c790c4e36b2b3c2c35a36db06226e41c692fc82b8b56ac1c540c5bd"));
	//const BigInt<1024> elecY(BigInt<1024>::fromHex("5b8dec5235a0fa8722476c7709c02559e3aa73aa03918ba2d492eea75abea235"));
	//const ECPoint elecPubkey(elecX, elecY);
		
	//const BigInt<1024> elecR(BigInt<1024>::fromHex("356cc2cf2dec4176f69052970d0f2a084adab9d74d40f36f2c4379286a7a806c"));
	//const BigInt<1024> elecS(BigInt<1024>::fromHex("8098dbbee6f123e4e345fe4917b5e155de213edc74188b451f91c204eedf8f8c"));
	//const ECPoint elecSign(elecR, elecS);
	//const SigFileContent sigParam(true, true, false, elecHash, elecPubkey, elecSign);

/*		
	if(checkSigLowS(elecHash, elecPubkey, elecSign))
	{
		std::cout << "Verify OK for armory" << std::endl;
	}
	else
	{
		std::cout << "Error in sign!" << std::endl;
	}
*/	
	//stSign();
		
	const BigInt<256> privKey1(BigInt<256>::fromHex("66473289ABCEF738374837847392392838330093939938848818191111FFAF31"));
	const std::string privVal1("5JbL8znGx6YAKyhMTC2mBgJiZp9vofMud2adz4wEo7e5VwG3ymx");
	const std::string pubVal1("1CvVWcQccwYbQSy2xoFR4fwAqCQ9BK6x5D");		
	testGeneration(privKey1, privVal1, pubVal1);

	const BigInt<256> privKey2(BigInt<256>::fromHex("6466373783738872372873282787AAAA7A7A77B77B7B777C77C7777D876FE123"));
	const std::string privVal2("5JaW9Sdn6aSzbeZ9JpRzmNK23x4J9UtwdM8hDQWRDEYrLi639Vh");
	const std::string pubVal2("1HwxQQ4KBNLF8C5NqFSCw6K8dTJHMK3BvH");
	testGeneration(privKey2, privVal2, pubVal2);

	const BigInt<256> privKey3(BigInt<256>::fromHex("748A88B8CC8EE8F8DD8E8110101239832299928388348374888F888F788888A8"));
	const std::string privVal3("5JhcU967vqV5F5xgaif5BEeh4Ef44Sv1MvBSvteQfZ48ZUfSGxM");
	const std::string pubVal3("1T6Bq5fVEShn4o429CCk1LBV96Qsg1wuJ");		
	testGeneration(privKey3, privVal3, pubVal3);

	const BigInt<256> privKey4(BigInt<256>::fromHex("5B27BE3AA5DEC37AB03C3A370790019966C642FDCB5B800F6DAD11F9990A9229"));
	const std::string privVal4("5JWS27KLkfoNkwrZyJBDURY349ANV8zvo3UZptJpf78z2xeXca4");
	const std::string pubVal4("1JfAc11XPLJoLXN5GMTLzoaeZDWeCGLGmM");		
	testGeneration(privKey4, privVal4, pubVal4);

	const BigInt<256> privKey5(BigInt<256>::fromHex("D8CB36B9628E1BBABA0F47A0E8A16189E03FA379BF2FB41EA8FFD53549D05B77"));
	const std::string privVal5("5KTmHfxNNNJ6KY1YTR1MsMWhAGwPiBubWGDufQBwemjAuAcM1pY");
	const std::string pubVal5("16pNo3yfxz4KzG1aCmso88MSLQb2Cb2nRd");		
	testGeneration(privKey5, privVal5, pubVal5);

	const BigInt<256> privKey6(BigInt<256>::fromHex("EFDD50ADCF5447B5C86DAFE49BCBE8CB4FE836C89321B8CBFEE8C3917BD68249"));
	const std::string privVal6("5KdvbfknT5CsdRc5xjiZuaaVcs2uUbc2WM1uYBM4RwoReskn7rr");
	const std::string pubVal6("12WqGoYkk9u1oQWCWMZZAvL93PBBACahjz");		
	testGeneration(privKey6, privVal6, pubVal6);

	const BigInt<256> privKey7(BigInt<256>::fromHex("2B6675725A9D247BC3C4C963521282B6F3DF2C40E9B5DEED588AFB6C4238C30E"));
	const std::string privVal7("5J9QBWvRTTpGhqMYA1Gf7u44BNLgNUtvC6HfVnbt9GsgEPe4ArQ");
	const std::string pubVal7("1KnUoYwPuLD5jPkrjGh1JMQeBbEZ98f8GT");		
	testGeneration(privKey7, privVal7, pubVal7);


	testSigning(Wif(privVal1));
	testSigning(Wif(privVal2));
	testSigning(Wif(privVal3));
	testSigning(Wif(privVal4));
	testSigning(Wif(privVal5));
	testSigning(Wif(privVal6));
	testSigning(Wif(privVal7));


	std::cout << std::endl;
	std::cout << "All tests performed successfully" << std::endl;
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
		std::cout << "For help, use \"bitsig help\"" << std::endl;
		std::cout << std::endl;
		
		
		if(BitsigConfig::numOwnKeypairs() == 0)
		{
			std::cout << "You do not currently have any own private keys" << std::endl;
			std::cout << "To be able to sign, a private key can be created: \"bitsig random\"" << std::endl;
			std::cout << std::endl;
		}

		if(BitsigConfig::numPublicKeys() == 0)
		{
			std::cout << "You do not currently have any imported public keys" << std::endl;
			std::cout << "For signature verification, a public key can be imported: \"bitsig import\"" << std::endl;
			
		}		
		
		throw std::string("Missing argument");
		//return;
	}


	const std::string configDir = BitsigConfig::getOrCreateConfigDir();




	const std::string firstArg(argv[1]);

	if(firstArg == "version" || firstArg == "-v")
	{
		std::cout << "bitsig version " << versionNumber << std::endl;
		std::cout << "http://www.bitcoin-gen.org/" << std::endl;		
	}
	else if(firstArg == "help")
	{
		printSigUsage();
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
	else if(firstArg == "verifyMultibit")
	{
		verifyMultibitMessage(argc, argv);
	}
	else if(firstArg == "signMultibit")
	{
		signMultibitMessage(args);
	}		
	else if(firstArg == "signArmory")
	{
		signArmoryMessage(args);
	}	
	else if(firstArg == "verifyArmory")
	{
		verifyArmoryMessage(argc, argv);		
	}
	else if(firstArg == "signAb64")
	{	
		signArmoryBase64(args);
	}
	else if(firstArg == "verifyAb64")
	{
		verifyArmoryBase64(argc, argv);
	}
	else if(firstArg == "signAclear")
	{	
		signArmoryClear(args);
	}	
	else if(firstArg == "verifyAclear")
	{
		verifyArmoryClear(argc, argv);
	}
	else if(firstArg == "verifyInputsIo")
	{
		verifyInputsIo(argc, argv);
	}
	else if(firstArg == "verifyMessage")
	{
		verifyAnyMessage(args);
	}	
	else if(firstArg == "verifyRaw")
	{
		verifyRaw(argc, argv);
	}
	else if(firstArg == "signRaw")
	{
		signRaw(args);
	}		
	else if(firstArg == "signInputsIo")
	{
		signInputsIoMessage(args);
	}			
	else if(firstArg == "test")
	{
		testOfSignature();
	}
	else if(firstArg == "sign")
	{
		signMessage(args);
	}
	else if(firstArg == "verify")
	{
		verifyMessage(args);
	}
	else if(firstArg == "signFile")
	{
		signFile(args);
	}
	else if(firstArg == "verifyFile")
	{
		verifyFile(args);
	}	
	else if(firstArg == "certify")
	{
		//Todo
		//certifyAddress(args);
	}	
	else if(firstArg == "importPrivate")
	{	
		importPrivateKey(argc, argv);
	}
	else if(firstArg == "import")
	{
		importPublicKey(argc, argv);		
	}
	else if(firstArg == "listPublic")
	{
		listPublicAddresses(args);		
	}	
	else if(firstArg == "listPublicKey")
	{
		listThePublicKeys(args);		
	}		
	else if(firstArg == "listOwn")
	{
		listOwnKeys(args);		
	}
	//else if(firstArg == "listPrivate")
	//{
	//	listPrivateKeys();		
	//}
	else if(firstArg == "list")
	{
		listKeys(argc, argv);	
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
		std::cerr << "Unknown bitsign command: " << argv[1] << std::endl;
		return;
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


	
