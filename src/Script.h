#ifndef SCRIPT_CLASS_H
#define SCRIPT_CLASS_H

/**
 * Script.h - Bitcoin poscript generator
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
#include <iostream>
#include <list>

#include "readStream.h"

#include "ScriptItem.h"
#include "sigPos.h"
#include "pubKey.h"
#include "RSVal.h"
#include "privKey.h"
#include "BinaryScript.h"
#include "binaryTransaction.h"

std::string hashRipe160Sha256(const std::string& script);




class Script
{
public:
	Script() {}
	Script(const std::list<ScriptItem>& inItems) : items(inItems) {}

	Script(const Script& a, const Script& b)
	{		
		items.insert(items.end(), a.items.begin(), a.items.end());
		items.insert(items.end(), b.items.begin(), b.items.end());	
	}

	static Script parse(const BinaryScript& script);


	Script run(const BinaryTransaction& transaction, const std::list<SigPos>& signaturePositions, const bool printOutput) const;

	Script copyScriptAndChangeItemWithNum(const int pos, const ScriptItem& newItem) const;
	Script copyScriptAndAddItemAfterPos(const int pos, const ScriptItem& newItem) const;


	std::list<PubKey> getPubkeysFromMultisigRedeem() const;

	int lastDataItemPos() const;
	int firstDataItemPos() const;

	ScriptItem getFirstDataItem() const;			
	ScriptItem getItemNum(const int pos) const;
	
	std::string toString() const;
	BinaryScript serialize() const;

	int size() const
	{
		return items.size();
	}

	std::string scriptTypeString() const;


	int getRequiredNum() const;	//Todo: Rename to getRequiredSignaturesFromMultisigScrip

	int getNumSignaturesFromMultisigScript() const;

	std::list<PubKey> getPubkeysFromRedeemScript() const;

	static Script createUnsignedMultsigInputScript(const Script& not_needed_script, const BinaryScript& myscript);


	


	static void saveTheScript(const BinaryScript& script);
	static std::string scriptFilename(const std::string& hash);
	static void saveScriptWithFilename(const BinaryScript& script, const std::string& filename);


	//static std::string getSignatureOfString(const RSVal& rs);
	static std::string getDerEncodedSignature(const RSVal& rs);
	
	static std::string encodeSignature(const RSVal& rs);

	static BinaryScript getscriptPartSignedTot(const RSVal& rs, const PrivKey& privKey, const bool payToPubkey);



private:	

	static std::string derNumber(const BigInt<1024>& val);


	static bool checkAllMultisigSignatures(const std::list<RSVal>& signatures, const std::list<PubKey>& pubkeys, 
		const std::string& unsignedTransaction, const int numNeeded);
public:

	static bool getSuccessStatus(const Script& stack);




	std::string toAddressString() const;

	bool isMultiSigUnlockScript() const;
	bool isMultiSigRedeem() const;

	bool isPayToPubkeyScript() const;
	bool isNormalOutputScript() const;
	bool isHash160OutputScript() const;
	bool isMultisigInputScript() const;
	bool isSigAndPubkeyInputScript() const;
	bool isRSInputScript() const;
	bool isOpReturnOutputScript() const;
	bool isUnsignedMultisigOutputScript() const;
	bool isUnsignedMultisigRedeem() const;
	bool isUnsignedTimelockOutputScript() const;

	PubAddress getUnsignedMultisigRedeemAddress() const;
	PubAddress getPayToPubkeyScriptAddress() const;
	PubAddress getNormalOutputScriptAddress() const;
	PubAddress getHash160OutputScriptAddress() const;
	PubAddress getMultisigInputScriptAddress() const;
	PubAddress getSigAndPubkeyInputScriptAddress() const;
	std::string getOpReturnString() const;


	std::list<ScriptItem> items;
};



class ScriptCreator
{
public:
	static BinaryScript createOpReturnScript(const std::string& nextAddrStr);
	static BinaryScript scriptPubkeyWithHash(const std::string& scriptHashStr);
	static BinaryScript scriptPubAddress(const PubAddress& address);
	static BinaryScript createOldPubkeyFundscript(const PubKey& pubkey);
	static BinaryScript createP2SHFundScript(const std::string redeemScript);
	static BinaryScript createMultisigFundScript(const PubAddress& multisigAddr);
	static BinaryScript createAnyoneCanSpendScript();
	static BinaryScript createAnyoneCanSpendFundscript();
	static BinaryScript createMultiSigScript(const int numReq, const std::list<PubKey>& mypkifSet);

	static Script createSignedMultsigInputScript(const BinaryScript& redeemscript, const std::list<RSVal>& signatures);
	
};


/*
class ScriptIdentifier
{
public:

};
*/


#endif
