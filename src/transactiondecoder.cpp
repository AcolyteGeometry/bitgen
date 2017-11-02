

#include "transactiondecoder.h"
#include "util.h"
#include "FileUtil.h"
#include "transaction.h"





BigInt<64> TransactionDecoder::reversedIntStr(const std::string& str)
{
	//std::cout << "In reversedIntStr" << std::endl;
	std::string unreversed;
	for(int i = str.size() - 1 ; i >= 0 ; i--)
	{
		const unsigned char c = str[i];
		unreversed += c;
	}

	//std::cout << "Unreversed size: " << unreversed.size() << std::endl;
	const BigInt<64> val(BigInt<64>::fromBytes(unreversed));
	return val;
}





BigInt<64> TransactionDecoder::decodeVersion(ReadStream& stream)
{
	//std::cout << "Will decode version" << std::endl;
	const std::string str = stream.getStr(4);

	const BigInt<64> version = reversedIntStr(str);
	
	//std::cout << "Version: " << version.strDec() << std::endl;
	return version;
}



int TransactionDecoder::decodeOutputIndex(ReadStream& stream)
{
	const std::string str = stream.getStr(4);
	const BigInt<64> index = reversedIntStr(str);
	//std::cout << "Index: " << index << std::endl;
	return index.intVal();
}



BigInt<64> TransactionDecoder::decodeFourByteInteger(ReadStream& stream)
{
	const std::string str = stream.getStr(4);
	const BigInt<64> num = reversedIntStr(str);
	return num;
}



BigInt<64> TransactionDecoder::decodeEightByteInteger(ReadStream& stream)
{
	const std::string str = stream.getStr(8);
	const BigInt<64> num = reversedIntStr(str);
	return num;
}


BigInt<64> TransactionDecoder::decodeSequence(ReadStream& stream)
{
	const std::string str = stream.getStr(4);
	const BigInt<64> sequence = reversedIntStr(str);
	//std::cout << "Sequence: " << sequence << std::endl;
	return sequence;
}


//Todo: Move to separate class
int CryptoDecoder::decodeByteInteger(ReadStream& stream)
{
	const std::string str = stream.getStr(1);	
	const unsigned char count = str[0];
	const int numIn(count);
	//std::cout << "Input count: " << numIn << std::endl;	
	return numIn;
}


//Todo: Move to util
std::string TransactionDecoder::reverseBytes(const std::string& str)
{
	
	std::string reversed;
	for(int i = str.size() - 1 ; i >= 0 ; i--)
	{
		const unsigned char c = str[i];
		reversed += c;
	}
	
	return reversed;
}


TxHash TransactionDecoder::decodeTxHash(ReadStream& stream)
{
	const std::string str = stream.getStr(32);	
	const std::string hash = reverseBytes(str);
	const TxHash txHash(hash, false);	
	//std::cout << "Tx Hash    : " << hexString(hash) << std::endl;	
	return txHash;
}











int TransactionDecoder::decodeVariableSizeInteger(ReadStream& stream, int& sizeOfSize)
{
	const int first = CryptoDecoder::decodeByteInteger(stream);
	if(first < 0xfd)
	{
		sizeOfSize = 1;
		return first;
	}
	else if(first == 0xfd)
	{
		const int low = CryptoDecoder::decodeByteInteger(stream);
		const int high = CryptoDecoder::decodeByteInteger(stream);
		
		const int result = high * 256 + low;
		sizeOfSize = 3;
		return result;
	}
	else if(first == 0xfe)
	{
		const int b1 = CryptoDecoder::decodeByteInteger(stream);
		const int b2 = CryptoDecoder::decodeByteInteger(stream);
		const int b3 = CryptoDecoder::decodeByteInteger(stream);
		const int b4 = CryptoDecoder::decodeByteInteger(stream);

		const int result = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;
		
		sizeOfSize = 5;		
		return result;		
	}
	else
	{
		throw std::string("Varint error");
	}
}




BinaryScript TransactionDecoder::decodeRawInputScript(ReadStream& stream, int& sigStartPos, int& sigLen)
{
	//std::cout << "In decodeRawInputScript" << std::endl;
	
	sigStartPos = stream.currentPos();

	int sizeOfSize(0);
	const int size = decodeVariableSizeInteger(stream, sizeOfSize);
	//const int size = decodeByteInteger(stream);	//Todo: Not single byte!!!!!!!!!!!!!
	//std::cout << "Script size (varint): " << size << std::endl;
		
	sigLen = size + (sizeOfSize - 1);	//Todo: WHY MINUS ONE???????????

	const std::string script = stream.getStr(size);
	//std::cout << "Input script: " << hexString(script) << std::endl;
	if(script.empty())
	{
		throw std::string("Input script error");
	}
	
	return BinaryScript(script);
}



//std::string
BinaryScript TransactionDecoder::decodeScript(ReadStream& stream, const int size)
{
	const std::string script = stream.getStr(size);	
	//std::cout << "Script: " << hexString(script) << std::endl;
	//std::cout << "Size Script: " << script.size() << std::endl;
	return BinaryScript(script);
}



//std::string
BinaryScript TransactionDecoder::decodeScriptTotal(ReadStream& stream)
{
	const int size = CryptoDecoder::decodeByteInteger(stream);
	const BinaryScript script = decodeScript(stream, size);
	//const BinaryScript binScript(script);
	return script;	
}



//std::string
BinaryTransaction CryptoTransaction::serialize() const
{
	std::string str;
	
	const std::string versionStr = Transaction::getVersion(version);
	
	const std::string inputCountStr = Transaction::inputCount(inputs.size());

	
	str += versionStr;
	str += inputCountStr;

	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		
		const std::string prevTxHashStr = Transaction::prevTxHash(ti.txHash);
		const std::string outputIndexStr = Transaction::outputIndex(ti.index);
		
		str += prevTxHashStr;
		str += outputIndexStr;
		
		const std::string scriptWithSize = Transaction::sizeIndicationAndScript(ti.script, true);			
		str += scriptWithSize;
		
		const BigInt<32> mySeq(ti.sequence);
		const std::string sequenceStr = Transaction::fourByteInt(mySeq);
		str += sequenceStr;
	}
	
	
	const std::string numberOutputsStr = Transaction::numberOutputs(outputs.size());		
	str += numberOutputsStr;
	
	
	for(std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
		it != outputs.end() ;
		++it)
	{
		const TransactionOutput& to = (*it);
		
		const std::string amountStr = Transaction::encodeAmount(to.amount.amount);		
		str += amountStr;

		const std::string script = to.script.raw();
		str += Transaction::intByte(script.size());		//Todo: Varint!!!!!!!!!!!!!!!!
		str += script;
		
	}

	const std::string locktimeStr = Transaction::locktime(locktime.intVal());
	str += locktimeStr;
	
	//const std::string hashCodeTypeStr = hashCodeType();	
	//str += hashCodeTypeStr;


	return BinaryTransaction(str);
}





CryptoTransaction CryptoTransaction::parse(const BinaryTransaction& trans,
	//const std::string& trans, 
	const bool printInfo)
{
	std::list<SigPos> signaturePositions;
	const CryptoTransaction cryptoTrans = CryptoTransaction::parseBinaryTransaction(trans, signaturePositions, printInfo);
	return cryptoTrans;
}



//Todo: Rename "signaturePositions" to inputScriptPositions

CryptoTransaction CryptoTransaction::parseBinaryTransaction(
		const BinaryTransaction& transaction,
		std::list<SigPos>& signaturePositions, const bool printInfo)
{
	std::list<TransactionInput> inputs;
	std::list<TransactionOutput> outputs;		
	//std::cout << "The parameter: " << transaction.asHex() << std::endl;
	
	ReadStream stream(transaction.raw());
	//std::cout << "Will get version" << std::endl;
	
	const BigInt<64> version = TransactionDecoder::decodeVersion(stream);
	//std::cout << "Got version" << std::endl;
	
	if(printInfo)
	{
		std::cout << "Version: " << version << std::endl;
	}
	
	int sizeOfSize(0);
	const int numInputs = TransactionDecoder::decodeVariableSizeInteger(stream, sizeOfSize);
	//const int numInputs = CryptoDecoder::decodeByteInteger(stream);

	if(printInfo)
	{
		std::cout << "Num inputs: " << numInputs << std::endl;
	}

	
	for(int i = 0 ; i < numInputs ; i++)
	{
		const TxHash txHash = TransactionDecoder::decodeTxHash(stream);	
		if(printInfo)
		{
			std::cout << "TxHash: " << txHash.toString() << std::endl;
		}
			
		const int index = TransactionDecoder::decodeOutputIndex(stream);
		if(printInfo)
		{
			std::cout << "index: " << index << std::endl;
		}

		
		int sigStartPos(0);
		int sigLen(0);
		const BinaryScript inputScript = TransactionDecoder::decodeRawInputScript(stream, sigStartPos, sigLen);		
		//const BinaryScript binScript(inputScript);
		const SigPos sigPos(sigStartPos, sigLen, inputScript);
		signaturePositions.push_back(sigPos);
		
		if(printInfo)
		{
			std::cout << "inputScript with size " << inputScript.raw().size() << ": " << hexString(inputScript.raw()) << std::endl;
		}
		
		//std::cout << "Will decode input script seqence" << std::endl;
		const BigInt<64> sequence = TransactionDecoder::decodeSequence(stream);
		//std::cout << "Has decoded input script seqence" << std::endl;
		
		if(printInfo)
		{
			std::cout << "sequence: " << sequence << std::endl;
		}

				
		const TransactionInput transactionInput(txHash, index, inputScript, sequence);
		inputs.push_back(transactionInput);		
	}

	//std::cout << "==================== Ready parsing all inputs ====================" << std::endl;

	int sizeOfOutputSize(0);
	const int numOutputs = TransactionDecoder::decodeVariableSizeInteger(stream, sizeOfOutputSize);
	
	//const int numOutputs = CryptoDecoder::decodeByteInteger(stream);
	//std::cout << "Num output: " << numOutputs << std::endl;
	
	for(int i = 0 ; i < numOutputs ; i++)
	{
		const BigInt<128> amount(TransactionDecoder::decodeEightByteInteger(stream));
		const BinaryScript script = TransactionDecoder::decodeScriptTotal(stream);
				
		TransactionOutput transactionOutput(script, amount);
		outputs.push_back(transactionOutput);
		//const BigInt<64> hashCode = decodeFourByteInteger(stream);
		//std::cout << "HashCode: " << hashCode.strHex() << std::endl;		
	}
	
	const BigInt<64> locktime = TransactionDecoder::decodeFourByteInteger(stream);

	const CryptoTransaction cryptoTransaction(version, inputs, outputs, locktime);
	return cryptoTransaction;
}



/*
bool hasOnlyTrue(const Script& stack)
{
	return true;
	
}
*/








void CryptoTransaction::printSimple() const
{

	std::cout << "Inputs: " << inputs.size() << " Outputs: " << outputs.size() << std::endl;
	std::cout << std::endl;
	
	if(isCoinbase())
	{
		std::cout << "Coinbase transaction" << std::endl;
	}
	
	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		
		const TransactionInput& input = (*it);
		
		const bool txIsZero = input.txHash.zero();
		
		if(txIsZero)
		{
			std::cout << "Zero hash, coinbase transaction" << std::endl;
		}
		else
		{
			std::cout << "" << minSize(/*ScriptDecoder::scriptToAddress(*/input.script.scriptToAddress(), 34);
			std::cout << " " + input.txHash.toString() << " at " << input.index << std::endl;			
		}
	}

	std::cout << std::endl;

	int outputNum(0);
	for(std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
		it != outputs.end() ;
		++it)
	{
		const TransactionOutput& output = (*it);
		
		const std::string theAddress = output.script.scriptToAddress();
		std::cout << "" << minSize(theAddress, 34);
		
		const Amount outAmount(output.amount);
		std::cout << " " << minSize(output.amount.amount.strDec(), 15, true) << " satoshi = " << minSize(outAmount.amountString(), 15) << " " <<
			getCurrencyShortName() << std::endl;
		
	}

	
	std::cout << std::endl;
}


std::string CryptoTransaction::getHash() const
{
	const BinaryTransaction serialized = serialize();
	const std::string hash = serialized.hash(); //doubleSha256(serialized.raw());		
	
	return hash;
}

void CryptoTransaction::saveScript(const BinaryScript& script, const std::string& transHash, const int index, const std::string& extra) const
{
	const std::string indexStr = intToString(index);
	const std::string filename = getCurrencyShortName() + "_script_" + extra + "_" + 
		hexString(Transaction::getReversedStr(transHash)) + "_" + indexStr + ".script";
	
	Script::saveScriptWithFilename(BinaryScript(script), filename);	
}


void CryptoTransaction::saveScripts() const
{
	const std::string transHash = getHash();
	
	std::cout << "Inputs" << std::endl;
	int index(0);
	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TransactionInput& input = (*it);
		saveScript(input.script, transHash, index, "input");
		//Script::saveTheScript(input.script);
		index++;
	}

	std::cout << std::endl;
	std::cout << "Outputs" << std::endl;

	int outIndex(0);
	for(std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
		it != outputs.end() ;
		++it)
	{
		const TransactionOutput& output = (*it);
		saveScript(output.script, transHash, outIndex, "output");		
		//Script::saveTheScript(output.script);
		outIndex++;
	}

}





std::string CryptoTransaction::locktimeInfoStr() const
{	
	if(locktime.intVal() < 500000000)
	{
		return "(block height)";
	}
	else
	{
		return "(Unix epoch time)";
	}	
}



void CryptoTransaction::printTransactionInfo(const bool verbose) const
{
	if(verbose)
	{	
		std::cout << "Transaction data----------------------" << std::endl;		
		std::cout << "Version   : " << version << std::endl;
		
		//Todo: If locktime is unix epoc time, print the time string
		std::cout << "Locktime  : " << locktime.strDec() << " " << locktimeInfoStr() << std::endl;
	}

	std::cout << "Inputs: " << inputs.size() << " Outputs: " << outputs.size() << std::endl;
	std::cout << std::endl;
	
	if(isCoinbase())
	{
		std::cout << "Coinbase transaction" << std::endl;
	}
	
	int inputNum(0);
	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		if(verbose)
		{	
			std::cout << "--Input index " << inputNum << std::endl;				
		}
		
		const TransactionInput& input = (*it);
		std::cout << "IHash  : " + input.txHash.toString() << " at " << input.index << std::endl;
		
		const bool txIsZero = input.txHash.zero();
		
		if(txIsZero)
		{
			std::cout << "Zero hash, coinbase transaction" << std::endl;
		}
		else
		{
			std::cout << "Address: " << /*ScriptDecoder::scriptToAddress(*/input.script.scriptToAddress() << std::endl;
			if(verbose)
			{	
				std::cout << "Type   : " << /*ScriptDecoder::scriptTypeString(*/input.script.scriptTypeString() << std::endl;				
				std::cout << "Seq    : " << input.sequence << std::endl;
			}
		}
		
		if(verbose)
		{		
			std::cout << "IScript: " << hexString(input.script.raw()) << std::endl;
			if(!txIsZero)
			{
				const Script parsedScript = Script::parse(input.script);
				std::cout << "Decoded script: " << parsedScript.toString() << std::endl;
			}
			
		}
		inputNum++;		
	}

	std::cout << std::endl;
	//std::cout << "Num output: " << outputs.size() << std::endl;

	int outputNum(0);
	for(std::list<TransactionOutput>::const_iterator it = outputs.begin() ;
		it != outputs.end() ;
		++it)
	{
		const TransactionOutput& output = (*it);
		if(verbose)
		{
			std::cout << "--Output index " << outputNum++ << std::endl;			
			std::cout << "OScript: " << hexString(output.script.raw()) << std::endl;
			const Script parsedScript = Script::parse(output.script);
			std::cout << "Decoded script: " << parsedScript.toString() << std::endl;
			std::cout << "Script type: " << /*ScriptDecoder::scriptTypeString(*/output.script.scriptTypeString() << std::endl;
			
		}
		

		const std::string theAddress = /*ScriptDecoder::scriptToAddress(*/output.script.scriptToAddress();
		std::cout << " " << minSize(theAddress, 34);
		

		const Amount outAmount(output.amount);
		std::cout << " " << minSize(output.amount.amount.strDec(), 15, true) << " satoshi = " << minSize(outAmount.amountString(), 15) << " " <<
			getCurrencyShortName() << std::endl;
		

	}

	
	std::cout << std::endl;
}



		/*
		std::cout << "Will check signatures" << std::endl;
		const bool incorrectSignature = TransactionSignatureChecker::checkSignatures(transaction, signaturePositions, verbose);
	
		if(incorrectSignature)
		{
			std::cout << std::endl;
			std::cout << "Error, incorrect signature" << std::endl;
			std::cout << std::endl;		
		}
		else
		{
			std::cout << "Found no incorrect signatures" << std::endl;
		}
		*/



std::string TransactionDecoder::getSignatureFromFile(const std::string& filename)
{
	const std::string content = readFile(filename);
	const std::string hexOnly = getHexString(content);
		//std::cout << "The transaction from file: \"" << hexOnly << "\"" << std::endl;
	const std::string script = convertFromHex(hexOnly);	
	return script;
}



BinaryScript TransactionDecoder::getScriptFromFile(const std::string& filename)
{
	const std::string content = readFile(filename);
	const std::string hexOnly = getHexString(content);
		//std::cout << "The transaction from file: \"" << hexOnly << "\"" << std::endl;
	const std::string script = convertFromHex(hexOnly);	
	return BinaryScript(script);
}

BinaryTransaction TransactionDecoder::getTransactionFromFile(const std::string& filename)
{
	const std::string content = readFile(filename);
	const std::string hexOnly = getHexString(content);
		//std::cout << "The transaction from file: \"" << hexOnly << "\"" << std::endl;
	const std::string script = convertFromHex(hexOnly);	
	return BinaryTransaction(script);
}




BinaryTransaction TransactionDecoder::tryGetTransactionFromFile(const std::string& filename)
{
	try
	{
		return BinaryTransaction(getTransactionFromFile(filename));
	}
	catch(const std::string& err)
	{
		return BinaryTransaction("");
	}
}



std::list<TxHash> CryptoTransaction::getInputHashes() const
{
	std::list<TxHash> ret;
	
	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		const TxHash& hash = ti.txHash;
		ret.push_back(hash);
	}

	return ret;
}


bool CryptoTransaction::run(const TransAndInScriptPos& transWithPos,  
	Amount& totalIn, const bool verbose) const
{
	//std::cout << "RUNTRANSACTIONINPUTS: " << (verbose ? "VERBOSE" : "silent") << std::endl;
	
	bool transactionSuccess(true);
	
	int numToCheck = 0;
	//Run input script
	for(std::list<TransactionInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TransactionInput& ti = (*it);
		const TxHash& hash = ti.txHash;
		const int index = ti.index;
		std::cout << "InputHash: " << hash.toString() << " Index: " << index << std::endl;
		
		const bool txIsZero = hash.zero();
		
		if(txIsZero)
		{
			std::cout << "Coinbase input, skipping" << std::endl;
			continue;
		}
		
		const BinaryScript unlockScript = ti.script;
		//std::cout << "Will parse unlock script: " << hexString(unlockScript) << std::endl;
		
		
		Script unlockScr = Script::parse(unlockScript);
		
		if(verbose)
		{
			std::cout << "The Unlock script: " << unlockScr.toString() << std::endl;
		}
		
		//If first item is OP_0, this is a multisig, and the last item should be duplicated
		if(unlockScr.isMultiSigUnlockScript())
		{
			//Is multisig unlock script, duplication last item
			unlockScr.items.push_back(unlockScr.items.back());
		}
		//std::cout << "Ready parsing unlock script" << std::endl;



		
		const std::string lockingFilename = Transaction::transactionFilename(hash.getReversed());
		
		
		//Todo: Get transaction from TransactionSet instead of from file
		const BinaryTransaction lockingTransRaw = TransactionDecoder::tryGetTransactionFromFile(lockingFilename);
		if(lockingTransRaw.raw().empty())
		{
			std::cout << "Could not find locking transaction file, skipping verification for this input" << std::endl;
			std::cout << std::endl;
			
			transactionSuccess = false;
			continue;
		}
		
		std::cout << "Got script from file" << std::endl;
		//std::cout << "TODO: GET AMOUNT FROM LOCKING SCRIPT" << std::endl;
		
		std::list<SigPos> lockSignaturePositions;
		//const BinaryTransaction binaryTransaction(lockingTransRaw);		
		const CryptoTransaction lockingTrans = CryptoTransaction::parseBinaryTransaction(lockingTransRaw, lockSignaturePositions);
		
	
				
		const TransactionOutput to = lockingTrans.getOutput(index);
		const BinaryScript lockingScript = to.script;
		const Script lockScr = Script::parse(lockingScript);
		
		if(verbose)
		{
			std::cout << "Locking script: " << lockScr.toString() << std::endl;
		}
		
		const Amount amount(to.amount);
		std::cout << "InAmount: " << amount.amountString() << std::endl;
		totalIn += amount;

		std::string unsignedScript;
		
		if(unlockScr.isMultiSigUnlockScript())
		{
			//Todo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			const ScriptItem lastUnlockItem = unlockScr.items.back();
			if(lastUnlockItem.object != ScriptItem::DATA)
			{
				std::cout << "Unlock Script: " << unlockScr.toString() << std::endl;
				throw std::string("Multisig err");
			}
		
		
			const BinaryScript redeemScript = BinaryScript(lastUnlockItem.data);
		
			const Script parsedRedeemScript = Script::parse(redeemScript);
			if(verbose)
			{	
				std::cout << "============================Redeem script: " << parsedRedeemScript.toString() << std::endl;
			}
		
		
			if(!parsedRedeemScript.isMultiSigRedeem())
			{
				std::cout << "Multisig error" << std::endl;
				throw std::string("Multisig redeem error");
			}
			
			unsignedScript = redeemScript.raw();
		}
		else
		{
			unsignedScript = lockingScript.raw();
		}
		
		//std::cout << "Will create unsigned transaction" << std::endl;
		
		const BinaryTransaction unsignedTransaction = transWithPos.getUnsignedTransactionForIndex(numToCheck, BinaryScript(unsignedScript));
		
		if(verbose)
		{	
			std::cout << "Raw unsigned transaction for index: " << numToCheck << ": " << hexString(unsignedTransaction.raw()) << std::endl;
		}
		

		const Script totScript(unlockScr, lockScr);
		
		if(verbose)
		{
			std::cout << "Lock script: " << lockScr.toString() << std::endl;
			std::cout << "Unlock script: " << unlockScr.toString() << std::endl;	
			std::cout << std::endl;
			std::cout << "Total script: " << totScript.toString() << std::endl;
		}
		const Script stack = totScript.run(unsignedTransaction, transWithPos.inputScriptPositions, verbose);
		
		const bool success = Script::getSuccessStatus(stack);
		if(success)
		{
			std::cout << "Unlock succeeded for script" << std::endl;
		}
		else
		{
			std::cout << "Unlock FAILED" << std::endl;
			std::cout << "The stack after operation: " << stack.toString() << std::endl;
			transactionSuccess = false;
		}
		
		std::cout << std::endl;
		
		numToCheck++;
	}
	
	return transactionSuccess;
}










