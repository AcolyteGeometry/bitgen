#ifndef TRANSACTION_H
#define TRANSACTION_H

/**
 * transaction.h - Bitcoin poscript generator
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
#include <map>

#include "bigint.h"
#include "RSVal.h"
#include "privKey.h"
#include "signature.h"
#include "amount.h"
#include "transactiondecoder.h"
#include "txHash.h"






template<int NUM_BITS>
std::string getReversed(BigInt<NUM_BITS>& hash)
{
	std::string ret;
	const std::string bytes = hash.getBinaryLong();
	for(int i = (bytes.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char c = bytes[i];
		ret += c;
	}
	return ret;
}



class InputExtraInfo
{
public:
	enum TxInputType { PAYTO_PUBKEY, PAYTO_PUBKEYHASH, PAYTO_SCRIPTHASH, PAYTO_ANYONECANSPEND };

	InputExtraInfo(const PrivKey& inPrivkey, const Amount& inAmount, const TxInputType inTxInputType) : 
		privkey(inPrivkey), amount(inAmount), txInputType(inTxInputType) {}

	InputExtraInfo(const Amount& inAmount, const TxInputType inTxInputType) : 
		privkey(BigInt<256>(1), true), amount(inAmount), txInputType(inTxInputType) {}


	InputExtraInfo() :  privkey(BigInt<256>(1), true), amount(0), txInputType(PAYTO_SCRIPTHASH) {}
	
	
	const Amount amount;	
	const PrivKey privkey;	
	const TxInputType txInputType;	
};



//Todo: Inherit from TransactionInput?
class TxInput
{
public:


	TxInput(const TransactionInput& inInput, const PrivKey& inPrivkey, const Amount& inAmount, const InputExtraInfo::TxInputType inTxInputType) : 
		input(inInput),
		extra(inPrivkey, inAmount, inTxInputType)
		{}

	TxInput(const TransactionInput& inInput, const Amount& inAmount, 
			const InputExtraInfo::TxInputType inTxInputType) : 
		input(inInput),
		extra(inAmount, inTxInputType)
		{}


	TxInput() : input(TxHash("1"), 1, BinaryScript(""), BigInt<64>(0))  {}
	
	bool scriptIsSigned() const
	{
		const BigInt<256> one(1);
		return (extra.privkey.key == one);
	}
	
//Todo: Make private	
	const TransactionInput input;	
	const InputExtraInfo extra;
};




class TxData
{
public:
	TxData(const std::list<TxInput>& inInputs, const std::list<TransactionOutput>& inRecipients, int inLocktime) : 
		inputs(inInputs), recipients(inRecipients), locktime(inLocktime) {}

	void printTransactionInformation() const;

	BinaryTransaction createSignedTransaction(const std::list<BigInt<256> >& tempKeys) const; 

	BinaryTransaction createTransactionInternal(const bool toSign, const TxInput& thisInput, const std::list<BigInt<256> >& tempKeys) const;


	int getNumTempkeysNeeded() const;

	int numInputs() const { return inputs.size(); }


	Amount getMinerFee() const;


private:
	std::string createTransactionInputs(const bool toSign, const TxInput& thisInput, const std::list<BigInt<256> >& tempKeys) const ;
	std::string createTransactionOutputs() const;

public:
	const std::list<TxInput> inputs;
	const std::list<TransactionOutput> recipients;	//Todo: Rename to "outputs"
	const int locktime;
};







class Transaction
{
public:
static void saveTranscation(const BinaryTransaction& trans, const bool fullySigned = true);
static std::string transactionFilename(const std::string& hash, const bool fullySigned = true);


static std::string varIntSize(const int num);

static std::string fourByteInt(BigInt<32> val);
static std::string getVersion(BigInt<32> versionInt);
static std::string getVersion();

static std::string inputCount(const int num);
static std::string prevTxHash(const TxHash& txHash);
static std::string outputIndex(const int index);
static std::string getReversedStr(const std::string& bytes);
static unsigned char intByte(const int val);

static RSVal getSignature(const PrivKey& privKey, const BigInt<256>& tempKey, const BinaryTransaction& raw);


static std::string sequence(const bool enableLocktime);
static std::string numberOutputs(const int num);
static std::string encodeAmount(const Amount& amount);
static std::string locktime(const int locktimeVal);
static std::string hashCodeType();

static std::string encodeScript(const BinaryScript& totalScript);

static std::string sizeIndicationAndScript(const BinaryScript& script, const bool includeThis);



static std::string scriptPayToPubkeyHashUnsignedTotal(const PubAddress& address);	//New




};





#endif
