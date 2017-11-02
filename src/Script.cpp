/**
 * Script.cpp - Bitcoin poscript generator
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


#include "Script.h"

#include "readStream.h"
#include "FileUtil.h"
#include "scriptdecoder.h"
#include "transaction.h"
#include "signature.h"


//Todo: Rename to hashRipe160Sha256
std::string hashRipe160Sha256(const std::string& script)
{
	const std::string theSha = sha256(script);
			
	const std::string theRipe = ripemd160(theSha);
	return theRipe;
}


std::string Script::scriptTypeString() const
{	
	if(isRSInputScript())
	{
		return "RSInput";
	}	
	else if(isMultisigInputScript())
	{
		return "P2SH input signed";		
	}
	else if(isUnsignedMultisigOutputScript())
	{
		return "P2SH input unsigned";
	}	
	else if(isSigAndPubkeyInputScript())
	{
		return "P2PKH input";
	}
	else if(isNormalOutputScript())
	{
		return "P2PKH output";
	}
	else if(isHash160OutputScript())
	{
		return "P2SH output";
	}
	else if(isOpReturnOutputScript())
	{
		return "opreturn output";
	}	
	else if(isPayToPubkeyScript())
	{
		return "P2PK output";
	}	
	
	
	return "Unknown";
}



std::string Script::toAddressString() const
{
	if(isRSInputScript())
	{
		return "RSInput";
	}	
	else if(isMultisigInputScript())
	{
		const PubAddress pubAddress = getMultisigInputScriptAddress();
		return pubAddress.toString();
	}
	else if(isSigAndPubkeyInputScript())
	{
		const PubAddress pubAddress = getSigAndPubkeyInputScriptAddress();
		return pubAddress.toString();
	}
	else if(isNormalOutputScript())
	{
		const PubAddress pubAddress = getNormalOutputScriptAddress();
		return pubAddress.toString();			
	}
	else if(isHash160OutputScript())
	{
		const PubAddress pubAddress = getHash160OutputScriptAddress();
		return pubAddress.toString();			
	}
	else if(isOpReturnOutputScript())
	{
		const std::string data = getOpReturnString();
		if(startsWith(data, "DOCPROOF"))
		{
			return "DOCPROOF";
		}
		else if(startsWith(data, "EW "))
		{
			const std::string readableString = "EW " + ScriptDecoder::printable(data.substr(0, 10));			
			return readableString;
		}

		const std::string readableString = "+" + ScriptDecoder::printable(data.substr(0, 10)) + "+";
		return readableString;
	}
	else if(isPayToPubkeyScript())
	{
		const PubAddress pubAddress = getPayToPubkeyScriptAddress();
		return pubAddress.toString();			
	}
	else if(isUnsignedMultisigRedeem())
	{
		const PubAddress pubAddress = getUnsignedMultisigRedeemAddress();
		return pubAddress.toString();			
	}
	//else if(isOpReturnOutputScript())
	//{
	//	//const PubAddress pubAddress = getUnsignedMultisigRedeemAddress();
	//	return "OP_RETURN";
	//}

		
	//std::cout << "Unknown address for script: " << toString() << std::endl;
	std::string errStr;
	errStr += "UnknownAddress: ";
	errStr += toString();
	
	return errStr; //"UnknownAddress";	
}




Script Script::parse(const BinaryScript& theScript)
{
	//std::cout << "Will parse script: " << hexString(script) << std::endl;
	const std::string script = theScript.raw();
	
	Script scr;
	for(int i = 0 ; i < script.size() ; i++)
	{
		const unsigned char c = script[i];
		if(c == 0xA9)
		{
			ScriptItem item(ScriptItem::OP_HASH160);
			scr.items.push_back(item);
		}
		else if(c == 0x00)
		{
			ScriptItem item(ScriptItem::OP_0);
			scr.items.push_back(item);
		}
		//else if(c == 0x51)
		//{
		//	ScriptItem item(ScriptItem::OP_TRUE);
		//	scr.items.push_back(item);
		//}				
		else if(c == 0x88)
		{
			ScriptItem item(ScriptItem::OP_EQUALVERIFY);
			scr.items.push_back(item);
		}
		else if(c == 0x76)
		{
			ScriptItem item(ScriptItem::OP_DUP);
			scr.items.push_back(item);
		}		
		else if(c == 0xAC)
		{
			ScriptItem item(ScriptItem::OP_CHECKSIG);
			scr.items.push_back(item);
		}
		else if(c == 0xAA)
		{
			ScriptItem item(ScriptItem::OP_HASH256);
			scr.items.push_back(item);
		}
		else if(c == 0x87)
		{
			ScriptItem item(ScriptItem::OP_EQUAL);
			scr.items.push_back(item);
		}
		else if(c == 0xAE)
		{
			ScriptItem item(ScriptItem::OP_CHECKMULTISIG);
			scr.items.push_back(item);
		}
		else if(c == 0x61)
		{
			ScriptItem item(ScriptItem::OP_NOP);
			scr.items.push_back(item);
		}
		else if(c == 0x6A)	//OP_RETURN
		{
			ScriptItem item(ScriptItem::OP_RETURN);
			scr.items.push_back(item);
			
			/*
			const std::string objectSize = script.substr(i + 1, 1);
			const unsigned char num = objectSize[0];
			const unsigned int itemSize = int(num);
			//std::cout << "Object size: " << itemSize << std::endl;
			const std::string object = script.substr(i + 2, itemSize);
			
			ScriptItem item(ScriptItem::OP_RETURN, object);
			//std::cout << "Item: " << item.toString() << std::endl;
			scr.items.push_back(item);
			
			i += 2;
			i += itemSize;	
			*/
		}		
		else if(c == 0x4C)
		{
			const std::string objectSize = script.substr(i + 1, 1);
			const unsigned char num = objectSize[0];
			const unsigned int itemSize = int(num);
			//std::cout << "Object size: " << itemSize << std::endl;
			const std::string object = script.substr(i + 2, itemSize);
			
			ScriptItem item(ScriptItem::DATA, object);
			//std::cout << "Item: " << item.toString() << std::endl;
			scr.items.push_back(item);
			
			i += 2;
			i += itemSize;	
		}
		else if(c == 0x4D)
		{
			const std::string objectSizeLow = script.substr(i + 1, 1);
			const std::string objectSizeHigh = script.substr(i + 2, 1);
			
			const unsigned char numLow = objectSizeLow[0];
			const unsigned char numHigh = objectSizeHigh[0];
			
			const unsigned int num = numLow + 256 * numHigh;
			
			const unsigned int itemSize = int(num);
			//std::cout << "Object size: " << itemSize << std::endl;
			const std::string object = script.substr(i + 3, itemSize);
			
			ScriptItem item(ScriptItem::DATA, object);
			//std::cout << "Item: " << item.toString() << std::endl;
			scr.items.push_back(item);
			
			i += 3;
			i += itemSize;	
		}
		else if(c == 0xB1)	//OP_CHECKLOCKTIMEVERIFY
		{
			ScriptItem item(ScriptItem::OP_CHECKLOCKTIMEVERIFY);
			scr.items.push_back(item);			
		}		
		else if(c == 0x75)	//OP_DROP
		{
			ScriptItem item(ScriptItem::OP_DROP);
			scr.items.push_back(item);
		}
		/*
		else if(c == 0x4A)
		{
			const std::string objectSize = script.substr(i + 2, 1);
			const unsigned char num = objectSize[0];
			const unsigned int itemSize = int(num);
			//std::cout << "Object size: " << itemSize << std::endl;
			const std::string object = script.substr(i + 3, itemSize);
			
			ScriptItem item(ScriptItem::DATA, object);
			//std::cout << "Item: " << item.toString() << std::endl;
			scr.items.push_back(item);
			
			i += 3;
			i += itemSize;	
		}*/	
		else if(c >= 0x51 && c < 0x60)
		{
			const unsigned char num = c - 0x52 + 2;
			const std::string str(1, num);
			
			ScriptItem item(ScriptItem::NUMBER, str);
			scr.items.push_back(item);
		}	
		else if(c > 0x01 && c < 0x4b)
		{
			//Item size
			const int itemSize = c;
			//std::cout << "Item size: " << itemSize << std::endl;
			const std::string object = script.substr(i + 1, itemSize);
			if(object.size() != itemSize)
			{
				std::cout << "Parsing DATA, got: " << object.size() << " should have got: " << itemSize << std::endl;
				std::cout << "Script this far: " << scr.toString() << std::endl;
				throw std::string("Parse error, did not get specified number bytes");
			}
			//std::cout << "Found object: " << hexString(object) << std::endl;
			ScriptItem item(ScriptItem::DATA, object);
			scr.items.push_back(item);
			i += itemSize;
		}
		else
		{
			std::cout << "Could not parse byte: " << int(c) << " hex:" << charToHex(c) << "" << std::endl;
			std::cout << "Previously parsed: " << scr.toString() << std::endl;
			std::cout << "Raw script: " << hexString(script) << std::endl;
			throw std::string("Script parse error");
		}
	}
	
	//std::cout << "Returning result" << std::endl;
	return scr;
}



bool Script::isUnsignedMultisigRedeem() const
{
	try
	{
		getUnsignedMultisigRedeemAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}



//NUMBER(req) DATA(pubkey1) DATA(pubkey2) DATA(pubkeyN) NUMBER(total) OP_CHECKMULTISIG 
PubAddress Script::getUnsignedMultisigRedeemAddress() const
{
	
	if(items.size() < 3)
	{
		throw std::string("Not unsigned multisig redeem");
	}
	
	
	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& required = (*it);
	if(required.object != ScriptItem::NUMBER)
	{
		throw std::string("Not unsigned multisig redeem");
	}
	
	++it;

	for(;;)
	{
		const ScriptItem& pubkey = (*it);
		
		if(pubkey.object != ScriptItem::DATA)
		{
			break;
		}
		++it;
	
	}

	const ScriptItem& total = (*it);
	if(total.object != ScriptItem::NUMBER)
	{
		throw std::string("Not unsigned multisig redeem");
	}
	
	++it;

	const ScriptItem& multisig = (*it);
	if(multisig.object != ScriptItem::OP_CHECKMULTISIG)
	{
		throw std::string("Not unsigned multisig redeem");
	}
	
	++it;

	if(it != items.end())
	{
		throw std::string("Not unsigned multisig redeem");		
	}
	
	const BinaryScript rawScript = serialize();
	
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(rawScript);
	return pubAddress;
}


PubAddress Script::getSigAndPubkeyInputScriptAddress() const
{
	if(items.size() != 2)
	{
		throw std::string("Not sig and pubkey in script");
	}

	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::DATA)
	{
		throw std::string("Not sig and pubkey in script");
	}
	
	++it;

	const ScriptItem& pubkeyItem = (*it);
	if(pubkeyItem.object != ScriptItem::DATA)
	{
		throw std::string("Not sig and pubkey in script");
	}
	
	const std::string pubkey = pubkeyItem.data;	
	
	bool compressed(false);
	const ECPoint point = ECPoint::parsePointBin(pubkey, compressed);
	const PubKey pubKey(point, compressed);
		
	return pubKey.getAddress();	
}



PubAddress Script::getMultisigInputScriptAddress() const
{
	if(items.size() < 3)
	{
		throw std::string("Not multisig in script");
	}

	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::OP_0)
	{
		throw std::string("Not 160 script out");
	}
	
	++it;

	for(; it != items.end(); ++it)
	{
		const ScriptItem& item = (*it);
		if(item.object != ScriptItem::DATA)
		{
			throw std::string("Not 160 script out");
		}		
	}

	const ScriptItem last = items.back();
	const BinaryScript myScript(last.data);
	const PubAddress pubAddress = PubAddress::multisigToBitcoinAddress(myScript); //last.data);
	return pubAddress;
}


//Pay to script hash
//Todo: Rename to getP2SHOutputScriptAddress
PubAddress Script::getHash160OutputScriptAddress() const
{
	if(items.size() != 3)
	{
		throw std::string("Not 160 script out");
	}
	
	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::OP_HASH160)
	{
		throw std::string("Not 160 script out");
	}
	
	++it;
	
	const ScriptItem& third = (*it);
	if(third.object != ScriptItem::DATA)
	{
		throw std::string("Not 160 script out");
	}
	
	++it;

	const ScriptItem& fourth = (*it);
	if(fourth.object != ScriptItem::OP_EQUAL)
	{
		throw std::string("Not 160 script out");
	}
	
	
	const std::string ripe = third.data;		
	const char netVersion = getMultisigVersion();
	const PubAddress pubAddress(PubAddress::fromBin(ripe, netVersion));
		
	return pubAddress;
}


bool Script::isRSInputScript() const
{
	if(items.size() != 1)
	{
		return false;
	}

	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::DATA)
	{
		return false;		
	}
	
	return true;
}




bool Script::isOpReturnOutputScript() const
{
	try
	{
		getOpReturnString();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}

std::string Script::getOpReturnString() const
{
	if(items.size() != 2)
	{
		throw std::string("Not op_return out script");
	}
	
	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::OP_RETURN)
	{
		throw std::string("Not op_return out script");
	}
	
	++it;
	
	const ScriptItem& second = (*it);
	if(second.object != ScriptItem::DATA)
	{
		throw std::string("Not op_return out script");
	}
	


	const std::string data = second.data;
	return data;
}





bool Script::isPayToPubkeyScript() const
{
	try
	{
		getPayToPubkeyScriptAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}



PubAddress Script::getNormalOutputScriptAddress() const
{
	if(items.size() != 5)
	{
		throw std::string("Not normal script out");
	}
	
	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& first = (*it);
	if(first.object != ScriptItem::OP_DUP)
	{
		throw std::string("Not normal script out");
	}
	
	++it;
	
	const ScriptItem& second = (*it);
	if(second.object != ScriptItem::OP_HASH160)
	{
		throw std::string("Not normal script out");
	}
	
	++it;

	const ScriptItem& third = (*it);
	if(third.object != ScriptItem::DATA)
	{
		throw std::string("Not normal script out");
	}
	
	++it;

	const ScriptItem& fourth = (*it);
	if(fourth.object != ScriptItem::OP_EQUALVERIFY)
	{
		throw std::string("Not normal script out");
	}
	
	++it;

	const ScriptItem& fifth = (*it);
	if(fifth.object != ScriptItem::OP_CHECKSIG)
	{
		throw std::string("Not normal script out");
	}
	
	//++it;
	
	//OP_DUP, OP_HASH160, DATA, OP_EQUALVERIFY, OP_CHECKSIG
	
	const std::string ripe = third.data;	
	//std::cout << "Ripe     : " << hexString(ripe) << std::endl;
	
	const char netVersion = getNetVersion();
	const PubAddress pubAddress(PubAddress::fromBin(ripe, netVersion));
	
	
	return pubAddress;
}







//DATA OP_CHECKSIG
PubAddress Script::getPayToPubkeyScriptAddress() const
{
	if(items.size() != 2)
	{
		throw std::string("Not P2P script");
	}

	std::list<ScriptItem>::const_iterator it = items.begin();
	
	const ScriptItem& sig = (*it);
	if(sig.object != ScriptItem::DATA)
	{
		throw std::string("Not P2P script");
	}
	
	++it;

	const ScriptItem& checksig = (*it);
	if(checksig.object != ScriptItem::OP_CHECKSIG)
	{
		throw std::string("Not P2P script");
	}
	
	const std::string pubkeyStr = sig.data;	
	
	const PubKey pubkey = PubKey::fromBin(pubkeyStr);
	return pubkey.getAddress();
}




bool Script::isNormalOutputScript() const
{
	try
	{
		getNormalOutputScriptAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}




bool Script::isHash160OutputScript() const
{
	try
	{
		getHash160OutputScriptAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}


bool Script::isMultisigInputScript() const
{
	try
	{
		getMultisigInputScriptAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}


bool Script::isSigAndPubkeyInputScript() const
{
	try
	{
		getSigAndPubkeyInputScriptAddress();
	}
	catch(const std::string& err)
	{
		return false;
	}
	
	return true;
}


bool Script::getSuccessStatus(const Script& stack)
{
	if(stack.items.size() != 1)
	{
		return false;
	}
		
	const ScriptItem& si = (*(stack.items.begin()));
	if(si.object == ScriptItem::BOOL && si.data == "1")
	{
		//std::cout << "Unlock succeeded" << std::endl;
		return true;
	}
		
	return false;
}


//std::string
BinaryScript Script::serialize() const
{
	std::string ret;
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& item = (*it);
		ret += item.serialize();
	}
	
	return BinaryScript(ret);
}


bool Script::isUnsignedTimelockOutputScript() const
{
	if(items.size() < 5)
	{
		std::cout << "Must have at least 5 items in timelock script" << std::endl;
		return false;
	}

	std::list<ScriptItem>::const_iterator it = items.begin();

	const ScriptItem& dataItem = (*it);
	if(dataItem.object != ScriptItem::DATA)
	{
		std::cout << "Item 1 must be DATA" << std::endl;		
		return false;		
	}

	++it;

	const ScriptItem& checkClockItem = (*it);
	if(checkClockItem.object != ScriptItem::OP_CHECKLOCKTIMEVERIFY)
	{
		std::cout << "Item 2 must be OP_CHECKLOCKTIMEVERIFY" << std::endl;		
		return false;		
	}

	++it;

	const ScriptItem& dropItem = (*it);
	if(dropItem.object != ScriptItem::OP_DROP)
	{
		std::cout << "Item 3 must be OP_DROP" << std::endl;		
		return false;		
	}


	++it;

	const ScriptItem& data2Item = (*it);
	if(data2Item.object != ScriptItem::DATA)
	{
		std::cout << "Item 4 must be DATA" << std::endl;		
		return false;		
	}

	++it;

	const ScriptItem& checksigItem = (*it);
	if(checksigItem.object != ScriptItem::OP_CHECKSIG)
	{
		std::cout << "Item 5 must be OP_CHECKSIG" << std::endl;		
		return false;		
	}
	
	return true;
}


//NUMBER DATA1 DATA2 ... DATAn NUMBER OP_CHECKMULTISIG
bool Script::isUnsignedMultisigOutputScript() const
{
	if(items.size() < 4)
	{
		//throw std::string("Not unsigned multisig out script");
		return false;
	}
	
	std::list<ScriptItem>::const_iterator it = items.begin();

	const ScriptItem& numReq = (*it);
	if(numReq.object != ScriptItem::NUMBER) // && numReq.object != ScriptItem::OP_TRUE)
	{
		return false;		
	}
	
	++it;


	for(int i = 0 ; i < (items.size() - 3) ; i++)
	{
		const ScriptItem& dataItem = (*it);
		if(dataItem.object != ScriptItem::DATA)
		{
			return false;		
		}
	
		++it;
	}


	const ScriptItem& numTot = (*it);
	if(numTot.object != ScriptItem::NUMBER)
	{
		return false;		
	}
	
	++it;

	const ScriptItem& opmultisig = (*it);
	if(opmultisig.object != ScriptItem::OP_CHECKMULTISIG)
	{
		return false;		
	}
	
	
	return true;
}



Script Script::run(const BinaryTransaction& unsignedTransaction, 
	const std::list<SigPos>& signaturePositions, const bool printOutput) const
{
	//std::cout << "RUNSCRIPT: " << (printOutput ? "PRINT" : "silent") << std::endl;
	
	Script stack;
	
	if(printOutput)
	{
		std::cout << std::endl;
		std::cout << "================ Running script ================" << std::endl;
	}
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& item = (*it);
		if(printOutput)
		{
			std::cout << "Operation: " << item.toString() << std::endl;
		}
		
		if(item.object == ScriptItem::DATA)
		{
			stack.items.push_back(item);
		}
		else if(item.object == ScriptItem::OP_DUP)
		{
			const ScriptItem topItem = stack.items.back();
			stack.items.push_back(topItem);
		}
		else if(item.object == ScriptItem::OP_0)		//Todo: Use NUMBER(0) instead?
		{
			const ScriptItem topItem(ScriptItem::OP_0);
			stack.items.push_back(topItem);
		}
		else if(item.object == ScriptItem::NUMBER)
		{
			const ScriptItem topItem(ScriptItem::NUMBER, item.data);
			stack.items.push_back(topItem);
		}
		/*
		else if(item.object == ScriptItem::OP_TRUE)
		{
			const unsigned char one(1);
			std::string oneStr(one, 1);
			const ScriptItem topItem(ScriptItem::NUMBER, oneStr);	//NUMBER(1) instead of OP_TRUE
			stack.items.push_back(topItem);
		}
		*/
		else if(item.object == ScriptItem::OP_HASH160)
		{
			const ScriptItem topItem = stack.items.back();
			stack.items.pop_back();
			
			const std::string origVal = topItem.data;
			const std::string hashVal = hashRipe160Sha256(origVal);
			
			const ScriptItem hashItem(ScriptItem::DATA, hashVal);
			stack.items.push_back(hashItem);
		}
		else if(item.object == ScriptItem::OP_HASH256)
		{
			const ScriptItem topItem = stack.items.back();
			stack.items.pop_back();
			
			const std::string origVal = topItem.data;
			
			const std::string hashVal = doubleSha256(origVal);
			
			const ScriptItem hashItem(ScriptItem::DATA, hashVal);
			stack.items.push_back(hashItem);
		}		
		else if(item.object == ScriptItem::OP_EQUAL)
		{
			const ScriptItem firstItem = stack.items.back();
			stack.items.pop_back();

			const ScriptItem secondItem = stack.items.back();
			stack.items.pop_back();
			
			const bool result = (firstItem.data == secondItem.data);

			const ScriptItem boolItem(ScriptItem::BOOL, result ? std::string("1") : std::string("0"));
			stack.items.push_back(boolItem);

		}		
		else if(item.object == ScriptItem::OP_EQUALVERIFY)
		{
			const ScriptItem firstItem = stack.items.back();
			stack.items.pop_back();

			const ScriptItem secondItem = stack.items.back();
			stack.items.pop_back();
			
			const bool result = (firstItem.data == secondItem.data);
			
			//Todo: Put result on stack!!!!!!!!!!
			
			if(!result)
			{
				std::cout << "Script failed" << std::endl;
				return stack;
			}
			
			//const ScriptItem boolItem(ScriptItem::BOOL, result ? std::string("1") : std::string("0"));
			//stack.items.push_back(boolItem);
		}
		else if(item.object == ScriptItem::OP_CHECKSIG)
		{
			const ScriptItem pubkeyItem = stack.items.back();
			stack.items.pop_back();
			const std::string pubkeyStr = pubkeyItem.data;
			const PubKey pubKey = PubKey::fromBin(pubkeyStr);
			
			if(printOutput)
			{
				std::cout << "Pubkey: " << pubKey.toStringHex() << std::endl;
			}
			const ScriptItem sigantureItem = stack.items.back();
			stack.items.pop_back();
			
			//todo
			ReadStream stream(sigantureItem.data);
			const RSVal rs = ScriptDecoder::decodeSignatureRS(stream);
			//std::cout << "R: " << rs.r.strHex() << std::endl;
			//std::cout << "S: " << rs.s.strHex() << std::endl;
			
			
			//Todo: Check signature
			//const int numToCheck = 0;
			//const bool result =  TransactionSignatureChecker::checkTransactionSignature(unsignedTransaction, pubKey, rs);
			
			const std::string hash = doubleSha256(unsignedTransaction.raw());
			const BigInt<256> hashNum(hash, 'c');
			
			const ECPoint& pubPoint = pubKey.point;	
			
			const bool result = SignatureImplementation::verifySign(rs, pubPoint, hashNum);

			
			
			
			//const bool result = false;	//Todo
			
			const ScriptItem boolItem(ScriptItem::BOOL, result ? std::string("1") : std::string("0"));
			stack.items.push_back(boolItem);
		}
		else if(item.object == ScriptItem::OP_CHECKMULTISIG)
		{
			const ScriptItem totnumItem = stack.items.back();
			stack.items.pop_back();
			
			if(totnumItem.object != ScriptItem::NUMBER)
			{
				std::cout << "Check multisig failed" << std::endl;
				return stack;
			}
			
			const int numTot = int(totnumItem.data[0]);
			//std::cout << "Num tot: " << numTot << std::endl;
			
			std::list<PubKey> pubkeys;
			for(int i = 0 ; i < numTot ; i++)
			{
				const ScriptItem pubkeyItem = stack.items.back();
				stack.items.pop_back();
				
				if(pubkeyItem.object != ScriptItem::DATA)
				{
					std::cout << "Check multisig failed, not pubkey" << std::endl;
					return stack;
				}
				
				const PubKey pubkey(PubKey::fromBin(pubkeyItem.data));
				pubkeys.push_back(pubkey);
				if(printOutput)
				{
					std::cout << "Pubkey: " << pubkey.toString() << std::endl;
				}
			}
			
			const ScriptItem totsigItem = stack.items.back();
			stack.items.pop_back();
			
			if(totsigItem.object != ScriptItem::NUMBER)
			{
				std::cout << "Check multisig failed" << std::endl;
				return stack;
			}
			
			const int reqSig = int(totsigItem.data[0]);
			std::cout << "Multisig: " << reqSig << "/" << numTot <<std::endl;
			
			std::list<RSVal> signatures;
			for(int i = 0 ; i < reqSig ; i++)
			{
				const ScriptItem sigItem = stack.items.back();
				stack.items.pop_back();
				
				if(sigItem.object != ScriptItem::DATA)
				{
					std::cout << "Check multisig failed, not sig" << std::endl;
					return stack;
				}
				
				//std::cout << "Will decode signature RS with: " << hexString(sigItem.data) << std::endl;
				ReadStream stream(sigItem.data);
				const RSVal rs = ScriptDecoder::decodeSignatureRS(stream);
				//std::cout << "R: " << rs.r << std::endl;
				//std::cout << "S: " << rs.s << std::endl;
				
				signatures.push_back(rs);
			}
			
			if(!checkAllMultisigSignatures(signatures, pubkeys, unsignedTransaction.raw(), reqSig))
			{
				std::cout << "Multsig signature failed" << std::endl;
				return stack;				
			}			
			
			stack.items.pop_back(); //Remove OP_0
			
			//Add true to stack
			const ScriptItem boolItem(ScriptItem::BOOL, std::string("1"));
			stack.items.push_back(boolItem);
		}
		else
		{
			std::cout << "Operation not implemented: " << item.toString() << std::endl;
			throw std::string("Operation not implemented");
		}
		
		if(printOutput)
		{
			std::cout << "Stack after operation: " << stack.toString() << std::endl;
			std::cout << std::endl;
		}
				
	}
	
	if(printOutput)
	{	
		std::cout << "FINAL STACK: " << stack.toString() << std::endl;
	}
	
	if(stack.items.size() == 1)
	{
		return stack;	
	}
	
	
	const ScriptItem& lastItem = ((stack.items.back()));
	if(lastItem.object == ScriptItem::BOOL && lastItem.data == "1")
	{
		//std::cout << "This part of the script script succeeded" << std::endl;
		//Remove last item
		stack.items.pop_back();
		
		
		
		//Convert the last item to script
		const ScriptItem scriptItem = stack.items.back();
		stack.items.pop_back();

		if(scriptItem.object != ScriptItem::DATA)
		{
			std::cout << "Could not convert last item to script" << std::endl;
			return stack;
		}
		
		const BinaryScript rawScript = BinaryScript(scriptItem.data);
		const Script multisigScript = Script::parse(rawScript);
		const Script finalScript(stack, multisigScript);
		
		if(printOutput)
		{
			std::cout << "FINAL SCRIPT: " << finalScript.toString() << std::endl;
		}
		
		const std::list<SigPos> signaturePositions;
		const Script theStack = finalScript.run( unsignedTransaction, signaturePositions, printOutput);
		if(getSuccessStatus(theStack))
		{
			return theStack;
		}
	}
	
	
	return stack;
}







bool Script::checkAllMultisigSignatures(const std::list<RSVal>& signatures, const std::list<PubKey>& pubkeys, 
	const std::string& unsignedTransaction, const int numNeeded)
{
	int numSuccess(0);
	
	const std::string hash = doubleSha256(unsignedTransaction);
	const BigInt<256> hashNum(hash, 'c');
	
	//std::cout << "Hash of unsigned transaction for signature check: " << hexString(hash) << std::endl;
	
	//Now check the signatures
	std::list<RSVal>::const_iterator sit = signatures.begin();
	//std::list<PubKey>::const_iterator pit = pubkeys.begin();

	for(std::list<PubKey>::const_iterator pit = pubkeys.begin();
		pit != pubkeys.end();
		++pit)
	{
	//for(std::list<RSVal>::const_iterator sit = signatures.begin();
	//	sit != signatures.end();
	//	++sit)
	//{
				
		const RSVal& rs = (*sit); 
		//std::cout << "Trying R: " << rs.r << std::endl;
		//std::cout << "Trying S: " << rs.s << std::endl;
		
		
		const PubKey& pubkey = (*pit);
		//std::cout << "Trying pubkey: " << pubkey.toString() << " "; 
								
				
		//Check signature
			
		const ECPoint& pubPoint = pubkey.point;	
			
		const bool result = SignatureImplementation::verifySign(rs, pubPoint, hashNum);
		if(result)
		{
			numSuccess++;
			
			std::cout << pubkey.toString() << " Signature OK, got: " << numSuccess << std::endl;
			
			if(numSuccess >= numNeeded)
			{
				std::cout << "Found " << numSuccess << " signatures" << std::endl;
				return true;
			}

			++sit;
			if(sit == signatures.end())
			{
				break;
			}
			
			//break;	//Try the next pubkey, do not use this pubkey again 
		}
		else
		{
			//std::cout << "Signature Error" << std::endl;
		}
		
	//}
	}
	
	return (numSuccess >= numNeeded);
}



ScriptItem Script::getFirstDataItem() const
{
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& si = (*it);
		if(si.object == ScriptItem::DATA)
		{
			return si;
		}
	}
		
	throw std::string("No DATA item exist");
}

	
int Script::lastDataItemPos() const
{
	int num(0);
	int lastFoundPos(-1);
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& si = (*it);
		if(si.object == ScriptItem::DATA)
		{
			lastFoundPos = num;
		}
			
		num++;
	}

	if(lastFoundPos == -1)
	{
		throw std::string("No DATA item exist");
	}
	
	return lastFoundPos;
}



int Script::firstDataItemPos() const
{
	int num(0);
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& si = (*it);
		if(si.object == ScriptItem::DATA)
		{
			return num;
		}
			
		num++;
	}
	
	throw std::string("No DATA item exist");
}
	
	
	
	
ScriptItem Script::getItemNum(const int pos) const
{
	if(pos >= items.size())
	{
		throw std::string("Script item num out of bounds");
	}
	
	int num(0);
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		if(pos == num)
		{
			const ScriptItem& si = (*it);
			return si;
		}
		
		num++;
	}
		
	throw std::string("Internal error");		
}
	

std::string Script::toString() const
{
	std::string ret;
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& si = (*it);
		ret += si.toString();
		ret += " ";
	}
		
	return ret;
}



Script Script::copyScriptAndAddItemAfterPos(const int pos, const ScriptItem& newItem) const
{
	std::list<ScriptItem> newItems;
	
	int num(0);
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem& item = (*it);
		newItems.push_back(item);
		
		if(num == pos)
		{
			newItems.push_back(newItem);
		}
		
		num++;
	}
	
	const Script newScript(newItems);
	return newScript;
	
}



Script Script::copyScriptAndChangeItemWithNum(const int pos, const ScriptItem& newItem) const
{
	std::list<ScriptItem> newItems;
	
	int num(0);
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		if(num == pos)
		{
			newItems.push_back(newItem);
		}
		else
		{
			const ScriptItem& item = (*it);
			newItems.push_back(item);
		}
		
		num++;
	}
	
	const Script newScript(newItems);
	return newScript;
}



Script Script::createUnsignedMultsigInputScript(const Script& not_used_origScript, const BinaryScript& myscript)
{
	//Script OP_0 DATA(script)
	
	std::list<ScriptItem> items;
	
	ScriptItem op0(ScriptItem::OP_0);
	items.push_back(op0);
	
	ScriptItem data(ScriptItem::DATA, myscript.raw());
	items.push_back(data);
	
	const Script script(items);
	return script;
}



std::list<PubKey> Script::getPubkeysFromRedeemScript() const
{
	std::list<PubKey> ret;
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		
		const ScriptItem item = (*it);
		if(item.object != ScriptItem::DATA)
		{
			continue;
		}
		
		//std::cout << "Getting raw pubkey: " << hexString(item.data) << std::endl;
		const PubKey pk = PubKey::fromBin(item.data);
		//std::cout << "Pubkey: " << pk.toString() << std::endl;	
		ret.push_back(pk);
	}
	
	return ret;
}






//OP_0 DATA(sig1) DATA(sig2) DATA(redeem)
int Script::getNumSignaturesFromMultisigScript() const
{
	//Todo: Improve, and make check for OP_0 and DATA(redeem)
	return (items.size() - 2);
}





int Script::getRequiredNum() const
{
	std::list<ScriptItem>::const_iterator it = items.begin() ;
	
	if(it == items.end())
	{
		throw std::string("Num required not found");
	}
	
	const ScriptItem& item = (*it);
	
	if(item.object != ScriptItem::NUMBER)
	{
		throw std::string("Number required not found");		
	}
	
	if(item.data.size() != 1)
	{
		throw std::string("Number indication for required not found");				
	}
	
	const unsigned char num = item.data[0];
	return num;
}


std::list<PubKey> Script::getPubkeysFromMultisigRedeem() const
{
	std::list<PubKey> ret;
	//std::cout << "Will get pubkeys from: " << redeemScript.toString() << std::endl;
	
	for(std::list<ScriptItem>::const_iterator it = items.begin() ;
		it != items.end() ;
		++it)
	{
		const ScriptItem item = (*it);
		if(item.object == ScriptItem::DATA)
		{
			const std::string pubkeyStr = item.data;
			//std::cout << "Will decode: " << hexString(pubkeyStr) << std::endl;
			const PubKey pubkey = PubKey::fromBin(pubkeyStr);
			//std::cout << "PubKey: " << pubkey.toString() << std::endl;
			ret.push_back(pubkey);
		}
	}
	
	return ret;
}



//Todo: Use RSVal for the signatures??
Script ScriptCreator::createSignedMultsigInputScript(const BinaryScript& redeemscript, const std::list<RSVal>& signatures)
{
	//Script OP_0 DATA(script)
	
	std::list<ScriptItem> items;
	
	const ScriptItem op0(ScriptItem::OP_0);
	items.push_back(op0);
	
	for(std::list<RSVal>::const_iterator it = signatures.begin() ;
		it != signatures.end() ;
		++it)
	{
		const RSVal& rs = (*it);
		const std::string signature = Script::encodeSignature(rs);		
		ScriptItem data(ScriptItem::DATA, signature);
		items.push_back(data);
	}
	
	const ScriptItem data(ScriptItem::DATA, redeemscript.raw());
	items.push_back(data);
	
	const Script script(items);
	return script;
}



bool Script::isMultiSigRedeem() const
{
	//std::cout << "The redeem script: " << script.toString() << std::endl;
	
	const ScriptItem lastItem = items.back();
	if(lastItem.object == ScriptItem::OP_CHECKMULTISIG)
	{
		return true;
	}
	
	return false;
}



bool Script::isMultiSigUnlockScript() const
{
	if(!items.empty() && items.front().object == ScriptItem::OP_0)
	{
		return true;
	}
	
	return false;
}




//std::string
BinaryScript ScriptCreator::createMultiSigScript(const int numReq, const std::list<PubKey>& mypkifSet)
{
	//std::cout << "Creating multisig fund script: " << numReq << "/" << mypkifSet.size() << std::endl;
	
	if(numReq < 1)
	{
		throw std::string("At least one signature required");
	}
	
	if(numReq > 16)
	{
		throw std::string("At most 16 signatures permitted");
	}
	
	if(mypkifSet.empty())
	{
		throw std::string("At least one pubkey required");		
	}
	
	if(mypkifSet.size() > 16)
	{
		throw std::string("At most 16 pubkeys required");
	}
	
	if(numReq > mypkifSet.size())
	{
		throw std::string("Can not require more signatures than pubkeys");		
	}


//-----------------------

	std::list<ScriptItem> items;
	
	ScriptItem opReturn(ScriptItem::NUMBER, numReq);
	items.push_back(opReturn);
	
	for(std::list<PubKey>::const_iterator it = mypkifSet.begin() ;
		it != mypkifSet.end() ;
		++it)
	{
		const PubKey& pubkey = (*it);
				
		const std::string raw = pubkey.getRaw();

		ScriptItem data(ScriptItem::DATA, raw);
		items.push_back(data);		
	}
	
	ScriptItem num(ScriptItem::NUMBER, mypkifSet.size());
	items.push_back(num);
	
	ScriptItem opCheckMultisig(ScriptItem::OP_CHECKMULTISIG);
	items.push_back(opCheckMultisig);

	const Script script(items);
	return BinaryScript(script.serialize());
}


std::string Script::scriptFilename(const std::string& hash)
{
	const std::string filename = getCurrencyShortName() + "_script_" + hexString(Transaction::getReversedStr(hash)) + ".script";
	return filename;
}



void Script::saveScriptWithFilename(const BinaryScript& script, const std::string& filename)
{
	const std::string content = hexString(script.raw()) + "\n";
	fileWrite(filename, content);
	std::cout << "Saved script file: " << filename << std::endl;	//Todo: Do this elsewhere
}


void Script::saveTheScript(const BinaryScript& script)
{
	const std::string hash = doubleSha256(script.raw());		

	const std::string filename = scriptFilename(hash);
	
	saveScriptWithFilename(script, filename);
	//std::cout << "Saved script file: " << filename << std::endl;
}




std::string Script::encodeSignature(const RSVal& rs)
{
	const std::string signatureOnly = Script::getDerEncodedSignature(rs);
	
	const std::string signatureEndOne = signatureOnly + '\x01';

	return signatureEndOne;
}


/*
std::string Script::getSignatureOfString(const RSVal& rs)
{
	std::string str;
		
	const std::string derEncoded = Script::getDerEncodedSignature(rs);
	
	str += derEncoded;
	
	return str;
}
*/



std::string Script::getDerEncodedSignature(const RSVal& rs)
{
	std::string rest;
	rest += '\x02'; //int
	const std::string rDerNumber = derNumber(rs.r);
	//std::cout << "R: " << hexString(rDerNumber) << std::endl;
	
	rest += Transaction::intByte(rDerNumber.size());
	rest += rDerNumber;
	rest += '\x02'; //int
	
	const std::string sDerNumber = derNumber(rs.s);
	//std::cout << "S: " << hexString(sDerNumber) << std::endl;
	
	rest += Transaction::intByte(sDerNumber.size());
	rest += sDerNumber;
	
	std::string str;	
	str += '\x30';
	str += Transaction::intByte(rest.size());
	str += rest;
	
	return str;
}



std::string Script::derNumber(const BigInt<1024>& val)
{
	std::string str = val.getBinaryShortest();
	const unsigned char c = str[0];
	if(c & 0x80)
	{
		const unsigned char zero(0);
		std::string zeroStr(1, zero);
		str = zeroStr + str;
	}

	return str;
}


//std::string
BinaryScript ScriptCreator::createOpReturnScript(const std::string& mydata)
{
	if(mydata.size() > 40)
	{
		std::cerr << "40 bytes permitted, got: " << mydata.size() << " bytes" << std::endl;
		throw std::string("OP_RETURN allows only 40 bytes");
	}
	
	std::list<ScriptItem> items;
	
	ScriptItem opReturn(ScriptItem::OP_RETURN);
	items.push_back(opReturn);
	
	ScriptItem data(ScriptItem::DATA, mydata);
	items.push_back(data);
	
	const Script script(items);
	return BinaryScript(script.serialize());
}







//Thid method created a "SIGNED" script, which means the script contains the RS value:
//Normal transaction:      DATA(rs) DATA(pubkey)
//Old-style pay-to-pubkey: DATA(rs)
//
//Todo: Rename to getScriptWithSignature()
BinaryScript Script::getscriptPartSignedTot(const RSVal& rs, const PrivKey& privKey, const bool payToPubkey)
{
	//std::cout << "CREATING SIGNED SCRIPT" << std::endl;
	std::list<ScriptItem> items;
	
	const std::string signatureEndOne = Script::encodeSignature(rs);
	
	const ScriptItem opData1(ScriptItem::DATA, signatureEndOne);
	items.push_back(opData1);

	if(!payToPubkey)
	{
		const BitcoinEllipticCurve ec;
		const PubKey pubkey = privKey.getPubkey(ec);
		const std::string pubkeyStr = pubkey.getRaw();

		const ScriptItem opData2(ScriptItem::DATA, pubkeyStr);
		items.push_back(opData2);		
	}

	
	const Script script(items);
	return script.serialize();
}




//std::string
BinaryScript ScriptCreator::scriptPubkeyWithHash(const std::string& scriptHashStr)
{	
	//std::cout << "CREATING REDEEM SCRIPT PUBKEY-HASH (Normal address)" << std::endl;

	std::list<ScriptItem> items;
		
	const ScriptItem opDup(ScriptItem::OP_DUP);
	items.push_back(opDup);
	
	const ScriptItem opHash160(ScriptItem::OP_HASH160);
	items.push_back(opHash160);

	const ScriptItem opData(ScriptItem::DATA, scriptHashStr);
	items.push_back(opData);

	const ScriptItem opEqualVerify(ScriptItem::OP_EQUALVERIFY);
	items.push_back(opEqualVerify);
	
	const ScriptItem opChecksig(ScriptItem::OP_CHECKSIG);
	items.push_back(opChecksig);

	const Script script(items);
	return BinaryScript(script.serialize());
}



//std::string
BinaryScript ScriptCreator::scriptPubAddress(const PubAddress& address)
{
	const std::string scriptHashStr = address.getBin();
		
	return BinaryScript(ScriptCreator::scriptPubkeyWithHash(scriptHashStr));
}



//Todo: Rename to createP2PKFundscript??
BinaryScript ScriptCreator::createOldPubkeyFundscript(const PubKey& pubkey)
{	
	//std::cout << "Creating fundscript for P2PK" << std::endl;

	std::list<ScriptItem> items;
		
	const std::string pubkeyStr = pubkey.getRaw();
	const ScriptItem opData(ScriptItem::DATA, pubkeyStr);
	items.push_back(opData);
	
	const ScriptItem opChecksig(ScriptItem::OP_CHECKSIG);
	items.push_back(opChecksig);
	
	const Script script(items);
	return BinaryScript(script.serialize());
}



//std::string
BinaryScript ScriptCreator::createP2SHFundScript(const std::string redeemScript)
{
	std::cout << "CREATING MULTISIG FUND SCRIPT" << std::endl;
	std::list<ScriptItem> items;
	
	const ScriptItem opReturn(ScriptItem::OP_HASH160);
	items.push_back(opReturn);

	const std::string scriptHash = hashRipe160Sha256(redeemScript);
	
	const ScriptItem data(ScriptItem::DATA, scriptHash);
	items.push_back(data);

	const ScriptItem opEqual(ScriptItem::OP_EQUAL);
	items.push_back(opEqual);

	
	const Script script(items);
	return BinaryScript(script.serialize());
}


//std::string
BinaryScript ScriptCreator::createMultisigFundScript(const PubAddress& multisigAddr)
{
	//std::cout << "Creating multisig fund script" << std::endl;
	
	std::list<ScriptItem> items;
		
	const ScriptItem opHash160(ScriptItem::OP_HASH160);
	items.push_back(opHash160);
	
	const std::string scriptHash = multisigAddr.getBin();	
	const ScriptItem opData(ScriptItem::DATA, scriptHash);
	items.push_back(opData);

	const ScriptItem opEqual(ScriptItem::OP_EQUAL);
	items.push_back(opEqual);
	
	const Script script(items);
	return BinaryScript(script.serialize());
}


//Output script
BinaryScript ScriptCreator::createAnyoneCanSpendScript()
{	
	//std::cout << "Creating anyone can spend script" << std::endl;
	
	std::list<ScriptItem> items;
		
	const ScriptItem opNum(ScriptItem::NUMBER, 1);
	items.push_back(opNum);
	
	const Script script(items);
	return BinaryScript(script.serialize());
}


//Input script
BinaryScript ScriptCreator::createAnyoneCanSpendFundscript()
{	
	std::cout << "Creating anyone can spend fund script" << std::endl;
	
	std::string str;
	BinaryScript script(str);
	//Empty

	return script;
}


