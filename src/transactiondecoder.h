#ifndef TRANSACTION_DECODER_H
#define TRANSACTION_DECODER_H

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
#include <vector>
#include <map>

#include "bigint.h"
#include "scriptdecoder.h"
#include "readStream.h"
#include "cryptoDecoder.h"
#include "sigPos.h"
#include "txHash.h"
#include "amount.h"
#include "binaryTransaction.h"



std::string convertFromHex(const std::string hexString);



class TransAndInScriptPos
{
public:
	TransAndInScriptPos(const BinaryTransaction& inTransaction, const std::list<SigPos> inInputScriptPositions) :
		transaction(inTransaction.raw()), inputScriptPositions(inInputScriptPositions) {}
	
	//Creates transaction TO SIGN (other input scripts empty)
	BinaryTransaction getUnsignedTransactionForIndex(const int numToCheck, const BinaryScript& redeemScript) const;
	
	//Creates SIGNED transaction (other input scripts unchanged)
	BinaryTransaction replaceScriptInTransaction(const int numToCheck, const BinaryScript& script) const;
	
	RSVal partlySignMultisig(const PrivKey& givenPriv, const BigInt<256>& tempKey) const;
	
	
	const std::string transaction;
	const std::list<SigPos> inputScriptPositions;
};












class TransactionInput
{
public:	
	TransactionInput(const TxHash& inTxHash, const int inIndex, const BinaryScript& inScript, const BigInt<64> inSequence) : 
		txHash(inTxHash), index(inIndex), script(inScript), sequence(inSequence) {}

	//TransactionInput(const TxHash& inTxHash, const int inIndex, const std::string& inScript, const BigInt<64> inSequence) : 
	//	txHash(inTxHash), index(inIndex), script(inScript), sequence(inSequence) {}
	
	const TxHash txHash;
	const int index;
	const BinaryScript script;
	const BigInt<64> sequence;
};



class TransactionOutput
{	
public:
	TransactionOutput(const BinaryScript& inScript, const Amount& inAmount) :
		script(inScript), amount(inAmount) {}
		
	const BinaryScript script;
	const Amount amount;	
};




class TransactionSet
{
	std::map<std::string, std::string> hashToTransaction;
};


//Todo: Relation to TxData???
class CryptoTransaction
{
public:	
	CryptoTransaction(const BigInt<64>& inVersion, const std::list<TransactionInput>& inInputs,
		const std::list<TransactionOutput>& inOutputs, const BigInt<64>& inLocktime) : 
		version(inVersion), inputs(inInputs), outputs(inOutputs), locktime(inLocktime) {}

	//Todo: Rename to "parseWithPositions()"
	static CryptoTransaction parseBinaryTransaction(const BinaryTransaction& transaction,
		std::list<SigPos>& signaturePositions, const bool printInfo = false);


	static CryptoTransaction parse(const BinaryTransaction& binaryTransaction,
				const bool printInfo = false);

	void printTransactionInfo(const bool verbose) const;
	void printSimple() const;
	void saveScripts() const;
	std::list<TxHash> getInputHashes() const;

	BinaryTransaction serialize() const;

	std::string locktimeInfoStr() const;
	std::string getHash() const;

	void saveScript(const BinaryScript& script, const std::string& transHash, const int index, const std::string& extra) const;

	bool run(const TransAndInScriptPos& transWithPos, Amount& totalIn, const bool verbose) const;
	bool run(const TransactionSet& transSet, const TransAndInScriptPos& transWithPos, Amount& totalIn, const bool verbose) const;


	bool isCoinbase() const
	{
		if(inputs.size() == 1)
		{
			const TransactionInput& input = *inputs.begin();
			return input.txHash.zero();
		}
		
		return false;
	}


	TransactionOutput getOutput(const int index) const
	{
		if(index >= outputs.size())
		{
			throw std::string("Index out of bounds");
		}
		
		std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
		for(int i = 0 ; i < index ; i++)
		{
			++it;
		}
		
		return (*it);
	}

	Amount getTotalOutAmount() const
	{
		Amount total;
		
		for(std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
			it !=  outputs.end() ;
			++it)
		{
			const TransactionOutput& to = (*it);
			total += to.amount;
		}
	
		return total;
	}


	const BigInt<32> version;	//Todo: BigInt32 ????
	std::list<TransactionInput> inputs;			//Todo: vector	//Todo: const
	std::list<TransactionOutput> outputs;		//Todo: vector	//Todo: const
	const BigInt<64> locktime;
};





class TransactionDecoder
{
public:

	static BigInt<64> reversedIntStr(const std::string& str);	
	static BigInt<64> decodeVersion(ReadStream& stream);
	static int decodeOutputIndex(ReadStream& stream);
	static BigInt<64> decodeFourByteInteger(ReadStream& stream);
	static BigInt<64> decodeEightByteInteger(ReadStream& stream);
	static BigInt<64> decodeSequence(ReadStream& stream);
	static std::string reverseBytes(const std::string& str);
	static TxHash decodeTxHash(ReadStream& stream);
	static BinaryScript decodeScript(ReadStream& stream, const int size);
	static BinaryScript decodeScriptTotal(ReadStream& stream);

	static BinaryScript decodeRawInputScript(ReadStream& stream, int& sigStartPos, int& sigLen);
	
	static BinaryTransaction tryGetTransactionFromFile(const std::string& filename);

	static BinaryTransaction getTransactionFromFile(const std::string& filename);
	static BinaryScript getScriptFromFile(const std::string& filename);
	static std::string getSignatureFromFile(const std::string& filename);
	
	
	static int decodeVariableSizeInteger(ReadStream& stream, int& sizeOfSize);


};




#endif
