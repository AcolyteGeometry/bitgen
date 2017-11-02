#ifndef TRANSACTION_SIGNATURE_CHECKER_H
#define TRANSACTION_SIGNATURE_CHECKER_H

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

#include "bigint.h"
#include "transaction.h"
#include "scriptdecoder.h"
#include "readStream.h"
#include "sigPos.h"


class TransactionSigner
{
public:
	static bool verifyThatPubkeyIsInList(const PubKey& givenPk, const std::list<PubKey>& pubkeyList);


	static BinaryScript signedAndReturnSignedScript(const Script& scriptWithSignatures, 
		const TransAndInScriptPos& transAndPos, const int num, const BinaryScript& myRedeemScript, const PrivKey& givenPriv);


	static BinaryScript findPointForPubkey(const Script& scriptWithSignatures, const Script& redeemScript, const PrivKey& givenPriv,
					const TransAndInScriptPos& transAndPos, const BinaryScript& myRedeemScript, const int num);


	static RSVal getTheSignature(const TransAndInScriptPos& transAndPos, const PrivKey& givenPriv, 
			const BinaryScript& redeemScript, const int num, const BigInt<256>& tempKey);

	static RSVal getSignatureForTransaction(const TransAndInScriptPos transAndPos, const PrivKey& givenPriv, const int num, 
		const BinaryScript& myRedeemScript, const BigInt<256>& tempKey);

	static BinaryScript signAndReplaceScript(const Script& scriptWithSignatures, const PrivKey& givenPriv, 
		const TransAndInScriptPos& transAndPos, const int num, const BinaryScript& myRedeemScript);


	static BinaryTransaction insertSignaturesInNormalTransaction(const BinaryTransaction& binaryTransaction, const std::list<RSVal>& signatures);																
	static BinaryTransaction insertSignaturesInMultisigTransaction(const BinaryTransaction& transaction, const std::list<RSVal>& signatures);
		
	static BinaryTransaction signUnsignedNormalInputScript(const TransAndInScriptPos& transAndPos, const std::list<RSVal>& signatures, 
		const BinaryScript& redeemScriptStr, const int num);		
	static BinaryTransaction signUnsignedMultisigInputScript(const TransAndInScriptPos& transAndPos, const std::list<RSVal>& signatures, 
		const BinaryScript& redeemScriptStr, const int num);
	
	static std::list<RSVal> sortSignatures(const BinaryTransaction signedTrans, const std::list<RSVal>& signatures, const std::list<PubKey>& pubkeys);

	static RSVal partlySignMultisigTransaction(const BinaryTransaction& transaction,
		const PrivKey& givenPriv, const BigInt<256>& tempKey);

	
	static void signMultisigTransaction(const BinaryTransaction& transaction,
		const PrivKey& givenPriv);
	
	static bool signGivenInputScript(const TransAndInScriptPos& transAndPos, const PrivKey& givenPriv, const TransactionInput& ti, const int num);
	
	static BinaryTransaction getSignedMultisigTransaction(const Script& scriptThatWillHaveSignatures, const PrivKey& givenPriv, 
		const TransAndInScriptPos& transAndPos, const int num, const BinaryScript& myRedeemScript);
	
};



//Todo: Remove
class TransactionSignatureChecker
{
public:	
		
	//static std::string replaceScriptInTransaction(const TransAndInScriptPos& transWithPos,
	//	const int numToCheck, const std::string& script);		
		
	//static std::string getUnsignedTransactionForIndex(const TransAndInScriptPos& transWithPos,
	//			const int numToCheck, const std::string& script);			
private:

	//Todo: Use TransAndInScriptPos
	//static std::string not_used_getUnsignedTransactionToVerify(const TransAndInScriptPos& transWithPos,
	//			const int numToCheck, const PubKey& pubkey);

};





#endif
