

#include "transactiondecoder.h"
#include "transactionSignatureChecker.h"
#include "randomUtils.h"
#include "FileUtil.h"




//Creates transaction SIGNED TRANSACTION
//(input scripts unchanged)
BinaryTransaction TransAndInScriptPos::replaceScriptInTransaction(const int numToCheck, const BinaryScript& script) const
{
	std::string ret;
	
	int i(0);
	int currentPos(0);
	
	for(std::list<SigPos>::const_iterator it = inputScriptPositions.begin() ;
		it != inputScriptPositions.end() ;
		++it)
	{
		const SigPos& sigPos = (*it);
		//std::cout << "Adding part: " << hexString(part) << std::endl;

		
		const bool changeThis = (i == numToCheck);
		if(changeThis)
		{
			const int len = sigPos.sigStartPos - currentPos;
			const std::string part = /*transWithPos.*/transaction.substr(currentPos, len);
			currentPos += (len + sigPos.sigLen + 1);
			
			ret += part;
			
			const std::string middle = Transaction::sizeIndicationAndScript(script, changeThis);
			ret += middle;	
		}
		else
		{
			const int len = sigPos.sigStartPos - currentPos + sigPos.sigLen;
			const std::string fullPart = /*transWithPos.*/transaction.substr(currentPos, len);
			currentPos += (len + 1);
			
			ret += fullPart;
			
		}
		
		//std::cout << "Adding middle: " << hexString(middle) << std::endl;		
		
		i++;	
	}
	
	
	const std::string end = /*transWithPos.*/transaction.substr(currentPos);
	
	//std::cout << "Adding end: " << hexString(end) << std::endl;			
	ret += end;	
	//std::cout << "Start: " << hexString(start) << std::endl;
	//std::cout << "End: " << hexString(end) << std::endl;
	
	//const std::string hashcodeType = Transaction::hashCodeType();
	//ret += hashcodeType;
	
	//const std::string total = start + middle + end + hashcodeType;
	return BinaryTransaction(ret);
}




//Creates transaction TO SIGN
//(input scripts set to empty string if they are not signed)
BinaryTransaction TransAndInScriptPos::getUnsignedTransactionForIndex(const int numToCheck, const BinaryScript& redeemScript) const
{
	std::string ret;
	
	int i(0);
	int currentPos(0);
	
	for(std::list<SigPos>::const_iterator it = /*transWithPos.*/inputScriptPositions.begin() ;
		it != /*transWithPos.*/inputScriptPositions.end() ;
		++it)
	{
		const SigPos& sigPos = (*it);
		const int len = sigPos.sigStartPos - currentPos;
		const std::string part = /*transWithPos.*/transaction.substr(currentPos, len);
		currentPos += (len + sigPos.sigLen + 1);
		//std::cout << "Adding part: " << hexString(part) << std::endl;
		ret += part;
		
		const bool includeThis = (i == numToCheck);

		const std::string middle = Transaction::sizeIndicationAndScript(BinaryScript(redeemScript), includeThis);
		
		//std::cout << "Adding middle: " << hexString(middle) << std::endl;		
		ret += middle;	
		
		i++;	
	}
	
	
	const std::string end = transaction.substr(currentPos);
	
	//std::cout << "Adding end: " << hexString(end) << std::endl;			
	ret += end;	
	//std::cout << "Start: " << hexString(start) << std::endl;
	//std::cout << "End: " << hexString(end) << std::endl;
	
	const std::string hashcodeType = Transaction::hashCodeType();
	ret += hashcodeType;
	
	//const std::string total = start + middle + end + hashcodeType;
	return BinaryTransaction(ret);
}






//Todo: Needed?
BinaryScript TransactionSigner::signedAndReturnSignedScript(const Script& scriptWithSignatures, 
	const TransAndInScriptPos& transAndPos,
	const int num, const BinaryScript& myRedeemScript, const PrivKey& givenPriv)
{
	//Sign, and replace script with signed script
	

	const BinaryTransaction transToSign = transAndPos.getUnsignedTransactionForIndex(num, myRedeemScript);


	//std::cout << "THIS TRANSACTION WILL BE SIGNED: " << hexString(transToSign) << std::endl;

	//Sign transaction				

	const std::string random = getFromDevRandom(32);				
	const BigInt<256> tempKey(random, 'c');

	//todo: NO size  signatureWithSizeAndEndOne
	const RSVal rs = Transaction::getSignature(givenPriv, tempKey, BinaryTransaction(transToSign));
	const std::string signature = Script::encodeSignature(rs);
	
	//saveSignatureFile(signature, givenPriv);
	//std::cout << "signature: " << hexString(signature) << std::endl;

	const ScriptItem newScriptItem(ScriptItem::DATA, signature);
			
	//if(scriptWithSignatures.isUnsignedMultisigOutputScript())	//Todo: Not needed, remove test
	//{
	//	std::cout << "INTERNAL ERROR" << std::endl;
	//	throw std::string("Internal sign error");			//Todo: Not needed, remove test			
	//}
	//else
	//{
	const int posNextToLast = (scriptWithSignatures.size() - 2);
	
	const Script partlySignedInputScript = scriptWithSignatures.copyScriptAndAddItemAfterPos(posNextToLast, newScriptItem);			
	//std::cout << "Script with signature added: " << partlySignedInputScript.toString() << std::endl;
	std::cout << std::endl;
	const BinaryScript serializedScript = partlySignedInputScript.serialize();
	//std::cout << "Size of serialized script: " << serializedScript.size() << std::endl;
	//std::cout << "Serialized script: " << hexString(serializedScript) << std::endl;
	return serializedScript;
}



//Return signed script
//std::string
BinaryScript TransactionSigner::findPointForPubkey(const Script& scriptWithSignatures, const Script& redeemScript, const PrivKey& givenPriv,
					const TransAndInScriptPos& transAndPos,
					const BinaryScript& myRedeemScript,
						const int num)
{

	const BitcoinEllipticCurve ec;
	const PubKey givenPk = givenPriv.getPubkey(ec);
	std::cout << "Pubkey for given privkey: " << givenPk.toString() << std::endl;	
	
	
	//Redeem: Pubkey1 Pubkey2 Pubkey3
	//Signatures: Sig1 
	
	const std::list<PubKey> pubkeyList = redeemScript.getPubkeysFromRedeemScript(); //redeemScript);
		
	if(!verifyThatPubkeyIsInList(givenPk, pubkeyList))
	{
		std::cout << "Error, privkey does not correspond to any pubkey in multisig" << std::endl;
		return BinaryScript("");
		//throw std::string("Privkey does not correspond to any pubkey in multisig");
	}
	


	return signedAndReturnSignedScript(scriptWithSignatures, transAndPos,
						num, myRedeemScript, givenPriv);

	
	//return "";	//Not this script, no change should be done
}








//If this is a multsig script with the correct public key, it is signed and returned
//Returns signed script or "" (empty string) if no change should be done
//std::string
BinaryScript TransactionSigner::signAndReplaceScript(const Script& scriptWithSignatures, const PrivKey& givenPriv, 
	const TransAndInScriptPos& transAndPos,
	const int num, const BinaryScript& myRedeemScript)
{	
	//std::cout << "===Script to add signatures: " << scriptWithSignatures.toString() << std::endl;
	
	const Script parsedRedeemScript = Script::parse(BinaryScript(myRedeemScript));	

	//std::cout << "===Redeem script: " << parsedRedeemScript.toString() << std::endl;


	return findPointForPubkey(scriptWithSignatures, parsedRedeemScript, givenPriv, transAndPos, myRedeemScript, num);
}




//Todo: Needed??????
BinaryTransaction TransactionSigner::getSignedMultisigTransaction(const Script& scriptThatWillHaveSignatures, const PrivKey& givenPriv, 
	const TransAndInScriptPos& transAndPos, const int num, const BinaryScript& myRedeemScript)
{			
			
	const BinaryScript newScript = signAndReplaceScript(scriptThatWillHaveSignatures, givenPriv, transAndPos, num, BinaryScript(myRedeemScript));
			
	if(newScript.raw().empty())
	{
		std::cout << "Could not get signed script" << std::endl;
		return BinaryTransaction("");
	}
	
	
	const BinaryTransaction newTrans = transAndPos.replaceScriptInTransaction(num, newScript);
	std::cout << "Signed transaction: " << hexString(newTrans.raw()) << std::endl;
				
	return newTrans;			
}





//Todo: Needed??????
bool TransactionSigner::signGivenInputScript(const TransAndInScriptPos& transAndPos, const PrivKey& givenPriv, const TransactionInput& ti, const int num)
{
	//Parse the script
	const Script origScript = Script::parse(ti.script);		
	//std::cout << "The script: " << origScript.toString() << std::endl;
		
		
	if(origScript.isUnsignedMultisigOutputScript())		//Completely unsigned
	{
		//std::cout << "This is a multisig script" << std::endl;
		//std::cout << "This is an multisig output script" << std::endl;
		const Script scriptThatWillHaveSignatures = Script::createUnsignedMultsigInputScript(origScript, ti.script);
		//std::cout << "Script that will have signatures: " << scriptThatWillHaveSignatures.toString() << std::endl;
		
		const BinaryTransaction newTrans = getSignedMultisigTransaction(scriptThatWillHaveSignatures, givenPriv, transAndPos, num, ti.script);
		//const BinaryTransaction newBinaryTransaction(newTrans);
		if(newTrans.raw().empty())
		{
			std::cout << "Not this one, skipping" << std::endl;
			return false;
		}
			
		Transaction::saveTranscation(newTrans); //newTrans);	
		return true;
							
	}
	else if(origScript.isMultisigInputScript())		//Partly signed
	{
		//Todo: NEEDED?????????????? Not supported?
		std::cout << "Is partly signed" << std::endl;
			
		const int lastDataItemPos = origScript.lastDataItemPos();
		const ScriptItem scriptItem = origScript.getItemNum(lastDataItemPos);
		const BinaryScript redeemScript = BinaryScript(scriptItem.data);
			
		//Todo: Print the current number of signatures
		const int numSig = origScript.getNumSignaturesFromMultisigScript(); //origScript);
		std::cout << "Previous number of signatures: " << numSig << std::endl;
			
		if(false)	//Verbose
		{
			const Script parsedRedeemScript = Script::parse(BinaryScript(redeemScript));
			std::cout << "===Redeem script: " << parsedRedeemScript.toString() << std::endl;
		}
			
		const BinaryTransaction newTrans = getSignedMultisigTransaction(origScript, givenPriv, transAndPos,
			num, redeemScript);
		//const BinaryTransaction newBinaryTransaction(newTrans);
							
		if(newTrans.raw().empty())
		{
			std::cout << "Not this one, skipping" << std::endl;
			return false;
		}
			
		Transaction::saveTranscation(newTrans); //newTrans);	
		return true;			
	}
	else
	{
		std::cout << "Not multisig: " << origScript.toString() << std::endl;
	}
		
	return false;
}







RSVal TransactionSigner::getSignatureForTransaction(const TransAndInScriptPos transAndPos, const PrivKey& givenPriv, const int num, 
	const BinaryScript& myRedeemScript, const BigInt<256>& tempKey)
{
	const BinaryTransaction transToSign = transAndPos.getUnsignedTransactionForIndex(num, myRedeemScript);

	//const std::string random = getFromDevRandom(32);				
	//const BigInt<256> tempKey(random, 'c');

	const RSVal rs = Transaction::getSignature(givenPriv, tempKey, transToSign);
	return rs;

//	const std::string signature = Script::encodeSignature(rs);
	
//	saveSignatureFile(signature, givenPriv);
}







//Todo: Needed????
void TransactionSigner::signMultisigTransaction(const BinaryTransaction& transaction,
	//const std::string& transaction, 
	const PrivKey& givenPriv)
{
	std::list<SigPos> intputScriptPositions;
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(transaction, intputScriptPositions);	
	//std::cout << "Size of intputScriptPositions: " << intputScriptPositions.size() << std::endl;

	const TransAndInScriptPos transAndPos(transaction, intputScriptPositions);

	int num(0);

	//Find the P2SH input(s)
	for(std::list<TransactionInput>::const_iterator it = trans.inputs.begin() ;
		it != trans.inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		if(signGivenInputScript(transAndPos, givenPriv, ti, num))
		{
			return;
		}
				
		num++;				
	}
	
	std::cout << "Error. Could not find the multisig transaction to sign" << std::endl;
}



RSVal TransactionSigner::getTheSignature(const TransAndInScriptPos& transAndPos, const PrivKey& givenPriv, 
	const BinaryScript& redeemScript, const int num, const BigInt<256>& tempKey)
{
	//Parse the script
	const Script origScript = Script::parse(BinaryScript(redeemScript));		
	//std::cout << "The script: " << origScript.toString() << std::endl;
		
		
	if(origScript.isUnsignedMultisigOutputScript())		//Completely unsigned
	{
		std::cout << "OK, found unsigned multisig output script" << std::endl;
	}
	else if(origScript.isUnsignedTimelockOutputScript())		//Completely unsigned
	{
		std::cout << "OK, found unsigned timelock output script" << std::endl;
	}
	else if(true)
	{
		std::cout << "TESTING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	}
	/*
	else if(origScript.isUnsignedAddrInputScript())
	{
		std::cout << "OK, found unsigned address input script" << std::endl;
	}
	*/
	else
	{
		std::cout << "Not multisig or timelock script: " << origScript.toString() << std::endl;
		RSVal rsError(0,0);	
		return rsError;		
	}

				
	const RSVal rs = getSignatureForTransaction(transAndPos, givenPriv, num, redeemScript, tempKey);
	return rs;	
}



RSVal TransAndInScriptPos::partlySignMultisig(const PrivKey& givenPriv, const BigInt<256>& tempKey) const
{
	std::list<SigPos> intputScriptPositions;
	const BinaryTransaction binaryTransaction(transaction);		
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(binaryTransaction, intputScriptPositions);	
	
	int num(0);

	//Find the P2SH input(s)
	for(std::list<TransactionInput>::const_iterator it = trans.inputs.begin() ;
		it != trans.inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		//Todo: Make member
		const RSVal rs = TransactionSigner::getTheSignature((*this), givenPriv, ti.script, num, tempKey);
		if(rs.r != 0)	
		{
			return rs;
		}			
				
		num++;				
	}
	
	std::cout << "Error. Could not find a multisig transaction input to sign" << std::endl;
	std::cout << "Number of transaction inputs: " << trans.inputs.size() << std::endl;
	RSVal rsError(0,0);
	return rsError;
}



//This method goes through a transaction, and signes the first found input that needs to be signed
//Todo: Make member in TransAndInScriptPos
RSVal TransactionSigner::partlySignMultisigTransaction(const BinaryTransaction& transaction,
													const PrivKey& givenPriv, const BigInt<256>& tempKey)
{
	std::list<SigPos> intputScriptPositions;
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(transaction, intputScriptPositions);	
	//std::cout << "Size of intputScriptPositions: " << intputScriptPositions.size() << std::endl;

	const TransAndInScriptPos transAndPos(transaction, intputScriptPositions);
	const RSVal rs = transAndPos.partlySignMultisig(givenPriv, tempKey);
	return rs;
}



std::list<RSVal> TransactionSigner::sortSignatures(const BinaryTransaction signedTrans,
	const std::list<RSVal>& signatures, const std::list<PubKey>& pubkeys)
{
	//std::cout << "Num signatures: " << signatures.size() << std::endl;
	//std::cout << "Num pubkeys: " << pubkeys.size() << std::endl;
	
	const std::string hash = doubleSha256(signedTrans.raw());		
	//std::cout << "Hash to sign: " << hexString(hash) << std::endl;
	
	const BigInt<256> hashNum(hash, 'c');
	
	
	std::list<RSVal> sortedSignatures;
	
	
	//Todo: N^2 tests not needed, make more efficient
	for(std::list<PubKey>::const_iterator it = pubkeys.begin() ;
		it != pubkeys.end() ;
		++it)
	{
		const PubKey& pk = (*it);
		for(std::list<RSVal>::const_iterator jt = signatures.begin() ;
			jt != signatures.end() ;
			++jt)
		{
			const RSVal& rs = (*jt);
			//const std::string& sig = (*jt);
			//const RSVal rs = ScriptDecoder::decodeSignatureRS(sig);
			//std::cout << "R: " << rs.r << std::endl;
			//std::cout << "S: " << rs.s << std::endl;
			
			//Is this signature for this pubkey?
			try
			{
			if(SignatureImplementation::checkSigLowS(hashNum, pk.point, rs))
			{
				//std::cout << "MATCH" << std::endl;
				sortedSignatures.push_back(rs);
			}
			}
			catch(const std::string& err)
			{
			}
		}
		
	}
	
	if(sortedSignatures.size() != signatures.size())
	{
		std::cout << std::endl;
		std::cout << "Num signatures: " << signatures.size() << std::endl;
		std::cout << "Num pubkeys: " << pubkeys.size() << std::endl;
		
		std::cout << "Signature mismatch. Sorted size: " << sortedSignatures.size() << " != sig size: " << signatures.size() << std::endl;
		throw std::string("Signature mismatch");
	}
		
	return sortedSignatures;
}




BinaryTransaction TransactionSigner::signUnsignedMultisigInputScript(const TransAndInScriptPos& transAndPos, 
	const std::list<RSVal>& signatures, 
	const BinaryScript& redeemScriptStr, const int num)
{
	std::cout << "Trying input script: " << num << std::endl;
	
	//Parse the script
	const Script redeemScript = Script::parse(redeemScriptStr);
				
	if(redeemScript.isUnsignedMultisigOutputScript())		//Completely unsigned
	{
		std::cout << "OK, found unsigned multisig redeem script" << std::endl;
	}
	else
	{
		std::cout << "Not redeem script: " << redeemScript.toString() << std::endl;		
		return BinaryTransaction("");
	}
	
	std::cout << "Using input: " << num << std::endl;


	//Get pubkeys in the given order
	const std::list<PubKey> pubkeys = redeemScript.getPubkeysFromMultisigRedeem();
	const int required = redeemScript.getRequiredNum();
	std::cout << "Num required: " << required << " total: " << pubkeys.size() << std::endl;

	//Sort signatures in the same order as pubkeys	
	const BinaryTransaction signedTrans = transAndPos.getUnsignedTransactionForIndex(num, redeemScriptStr);
	
	const std::list<RSVal> sortedSignatures = sortSignatures(signedTrans, signatures, pubkeys);
	if(sortedSignatures.size() < required)
	{
		std::cout << "Not enough signatures. Got: " << sortedSignatures.size() << ", required: " << required << 
			", max: " << pubkeys.size() << std::endl;
		throw std::string("Not enough signatures");
	}

	const Script scriptWithSignatures = ScriptCreator::createSignedMultsigInputScript(redeemScriptStr, sortedSignatures);
	
	//std::cout << "Script with signatures: " << scriptWithSignatures.toString() << std::endl;
	//std::cout << std::endl;

	const BinaryScript scriptWithSig = scriptWithSignatures.serialize();

	const BinaryTransaction newTrans = transAndPos.replaceScriptInTransaction(num, scriptWithSig);
				
	if(newTrans.raw().empty())
	{
		std::cout << "Could not sign transaction" << std::endl;
		return BinaryTransaction("");
	}

	//std::cout << "Signed transaction: " << hexString(newTrans) << std::endl;
	return newTrans;	
}










BinaryTransaction TransactionSigner::signUnsignedNormalInputScript(const TransAndInScriptPos& transAndPos, const std::list<RSVal>& signatures, 
	const BinaryScript& redeemScriptStr, const int num)
{
	std::cout << "Trying normal input script: " << num << std::endl;
	
	//Parse the script
	const Script redeemScript = Script::parse(BinaryScript(redeemScriptStr));
				
	//if(redeemScript.isUnsignedMultisigOutputScript())		//Completely unsigned
	//{
	//	std::cout << "OK, found unsigned multisig redeem script" << std::endl;
	//}
	if(true)
	{
		std::cout << "TESTING, REMOVE WHEN READY" << std::endl;
	}
	//else if(redeemScript.isUnsignedTimelockOutputScript())		//Completely unsigned
	//{
	//	std::cout << "OK, found unsigned timelock redeem script" << std::endl;
	//}
	else
	{
		std::cout << "Not redeem script: " << redeemScript.toString() << std::endl;		
		return BinaryTransaction("");		
	}
	
	std::cout << "Using input: " << num << std::endl;

	//const std::list<std::string> sortedSignatures;
	const Script scriptWithSignatures = ScriptCreator::createSignedMultsigInputScript(redeemScriptStr, signatures);

	const BinaryScript scriptWithSig = scriptWithSignatures.serialize();

	const BinaryTransaction newTrans = transAndPos.replaceScriptInTransaction(num, scriptWithSig);
				
	if(newTrans.raw().empty())
	{
		std::cout << "Could not sign transaction" << std::endl;
		return BinaryTransaction("");
	}

	std::cout << "Signed transaction: " << hexString(newTrans.raw()) << std::endl;
	return newTrans;

/*
	

	const Script scriptWithSignatures = ScriptCreator::createSignedMultsigInputScript(redeemScriptStr, sortedSignatures);
	
	//std::cout << "Script with signatures: " << scriptWithSignatures.toString() << std::endl;
	//std::cout << std::endl;

	const std::string scriptWithSig = scriptWithSignatures.serialize();

	const std::string newTrans = transAndPos.replaceScriptInTransaction(num, scriptWithSig);
				
	if(newTrans.empty())
	{
		std::cout << "Could not sign transaction" << std::endl;
		return "";
	}

	//std::cout << "Signed transaction: " << hexString(newTrans) << std::endl;
	return newTrans;
	*/

	std::cout << "Not yet implemented!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	std::cout << std::endl;
		
	return BinaryTransaction("");
}





BinaryTransaction TransactionSigner::insertSignaturesInNormalTransaction(const BinaryTransaction& binaryTransaction,
																const std::list<RSVal>& signatures)
{
	
	std::list<SigPos> intputScriptPositions;
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(binaryTransaction, intputScriptPositions);	

	const TransAndInScriptPos transAndPos(binaryTransaction, intputScriptPositions);

	int num(0);

	for(std::list<TransactionInput>::const_iterator it = trans.inputs.begin() ;
		it != trans.inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		const BinaryTransaction signedTransaction = signUnsignedNormalInputScript(transAndPos, signatures, ti.script, num);
		
		if(!signedTransaction.raw().empty())		
		{
			return signedTransaction;
		}		
				
		num++;				
	}
	
	std::cout << "Error. Could not find one multisig transaction to sign" << std::endl;
	return BinaryTransaction("");
}




BinaryTransaction TransactionSigner::insertSignaturesInMultisigTransaction(const BinaryTransaction& binaryTransaction,
																const std::list<RSVal>& signatures)
{
	
	std::list<SigPos> intputScriptPositions;
	const CryptoTransaction trans = CryptoTransaction::parseBinaryTransaction(binaryTransaction, intputScriptPositions);	
	//std::cout << "Size of intputScriptPositions: " << intputScriptPositions.size() << std::endl;

	const TransAndInScriptPos transAndPos(binaryTransaction, intputScriptPositions);

	int num(0);

	//Find the P2SH input(s)
	for(std::list<TransactionInput>::const_iterator it = trans.inputs.begin() ;
		it != trans.inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		const BinaryTransaction signedTransaction = signUnsignedMultisigInputScript(transAndPos, signatures, ti.script, num);
		
		if(!signedTransaction.raw().empty())		
		{
			return signedTransaction;
		}		
				
		num++;				
	}
	
	std::cout << "Error. Could not find one multisig transaction to sign" << std::endl;
	return BinaryTransaction("");
}



bool TransactionSigner::verifyThatPubkeyIsInList(const PubKey& givenPk, const std::list<PubKey>& pubkeyList)
{	
	int currentNum(0);
	
	int matchNum(-1);
	std::cout << "Will try " << pubkeyList.size() << " pubkeys" << std::endl;
	
	for(std::list<PubKey>::const_iterator it = pubkeyList.begin() ;
		it != pubkeyList.end() ;
		++it)
	{
		currentNum++;
		
		const PubKey& pk = (*it);
		std::cout << "Pubkey: " << pk.toString(); // << std::endl;	
	
		//Is this the pubkey that corresponds to the privkey?
		if(givenPk.point == pk.point)
		{
			//std::cout << std::endl;
			matchNum = currentNum;
			std::cout << " MATCH" ; // for: " << pk.toString() << std::endl;
			//return true;	
		}
		else
		{
			//std::cout << "No match for: " << pk.toString() << std::endl;		
		}
	
		std::cout << std::endl;
	}

	if(matchNum != -1)
	{
		std::cout << "Match for num " << matchNum << " of " << pubkeyList.size() << std::endl;
	}
	std::cout << std::endl;

	return (matchNum != -1);
	
	
	//return false;
}
