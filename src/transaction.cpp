/**
 * transaction.cpp - Bitcoin poscript generator
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


#include "transaction.h"
#include "randomUtils.h"
#include "scriptdecoder.h"
#include "FileUtil.h"

void Transaction::saveTranscation(const BinaryTransaction& trans, const bool fullySigned)
{
	const std::string hashSigned = doubleSha256(trans.raw());		

	const std::string filename = transactionFilename(hashSigned, fullySigned);
		
	const std::string content = hexString(trans.raw()) + "\n";
	fileWrite(filename, content);
	std::cout << std::endl;
	std::cout << "Saved file: " << filename << std::endl;
}



std::string Transaction::fourByteInt(BigInt<32> val)
{
	const std::string versionStr = getReversed(val);
	return versionStr;		
}


std::string Transaction::getVersion(BigInt<32> versionInt)
{
	const std::string versionStr = getReversed(versionInt);
	return versionStr;	
}


std::string Transaction::getVersion()
{
	BigInt<32> versionInt(1);
	const std::string versionStr = getReversed(versionInt);
	return versionStr;	
}



std::string Transaction::inputCount(const int num)
{
	std::string str;
	str += Transaction::varIntSize(num);	
	return str;
}




std::string Transaction::prevTxHash(const TxHash& txHash)
{
	std::string str;
	
	str += txHash.getReversed();
		
	return str;
}



std::string Transaction::outputIndex(const int index)
{
	BigInt<32> indexInt(index);
	const std::string indexStr = getReversed(indexInt);
	
	return indexStr;	
}





std::string Transaction::getReversedStr(const std::string& bytes)
{
	std::string ret;
	for(int i = (bytes.size() - 1) ; i >= 0 ; i--)
	{
		const unsigned char c = bytes[i];
		ret += c;
	}
	return ret;
}






unsigned char Transaction::intByte(const int val)
{
	if(val > 255)
	{
		std::cout << "Can not encode as byte, number out of range: " << val << std::endl;
		throw std::string("Number out of range");
	}
	
	const unsigned char num = val;
	return num;
}





RSVal Transaction::getSignature(const PrivKey& privKey, const BigInt<256>& tempKey, const /*std::string*/BinaryTransaction& raw)
{
	const std::string hash = doubleSha256(raw.raw());		
	//std::cout << "Hash to sign: " << hexString(hash) << std::endl;
	
	const BigInt<256> hashNum(hash, 'c');
	
	const RSVal rs = SignatureImplementation::sign(privKey.key, tempKey, hashNum);
	return rs;
}





std::string Transaction::transactionFilename(const std::string& hash, const bool fullySigned)
{
	const std::string filename = getCurrencyShortName() + "_" + (fullySigned ? "" : "nonsigned_") + hexString(Transaction::getReversedStr(hash)) + ".trans";
	return filename;
}



std::string Transaction::sequence(const bool enableLocktime)
{
	std::string str;
	
	if(!enableLocktime)
	{
		str += '\xff';
		str += '\xff';
		str += '\xff';
		str += '\xff';
	}
	else
	{
		str += '\x0';
		str += '\x0';
		str += '\x0';
		str += '\x0';
	}
		
	return str;	
}




std::string Transaction::numberOutputs(const int num)
{
	std::string str;
	
	str += Transaction::varIntSize(num);	
	
	return str;	
}


//Todo: Rename to "encodeAmount()"
std::string Transaction::encodeAmount(const Amount& amount)
{
	BigInt<64> amountInt(amount.amount);
	//std::cout << "Amount: " << amountInt << std::endl;
	const std::string amountReversed = getReversed(amountInt);	
	//std::cout << "amountReversed: " << hexString(amountReversed) << std::endl;
	return amountReversed;
}



std::string Transaction::locktime(const int locktimeVal)
{
	std::string str;

	BigInt<32> locktimeInt(locktimeVal);
	const std::string locktimeStr = getReversed(locktimeInt);	
	return locktimeStr;	
}


std::string Transaction::hashCodeType()
{
	std::string str;
	str += '\x01';
	str += '\x00';
	str += '\x00';
	str += '\x00';
		
	return str;		
}












std::string Transaction::varIntSize(const int num)
{
	std::string ret;
	
	if(num < 0xfd)
	{
		ret += intByte(num);
		return ret;
	}
	else if(num < 0xffff)
	{
		const int low = num % 256;
		const int high = num / 256;

		ret += intByte(0xfd);
		ret += intByte(low);
		ret += intByte(high);
		
		//const int result = high * 256 + low;
		return ret;
	}
	else if(num < 0xffffffff)
	{
		const int b1 = (num >> 0) & 0xff;
		const int b2 = (num >> 8) & 0xff;
		const int b3 = (num >> 16) & 0xff;
		const int b4 = (num >> 24) & 0xff;
		
		
		ret += intByte(0xfe);
		ret += intByte(b1);
		ret += intByte(b2);
		ret += intByte(b3);
		ret += intByte(b4);
		
		return ret;
		
		//const int result = (b4 << 24) + (b3 << 16) + (b2 << 8) + b1;
		//return result;		
	}
	else
	{
		throw std::string("Varint error");
	}
	
}


std::string Transaction::encodeScript(const BinaryScript& totalScript)
{	
	std::string str;
	
	str += Transaction::varIntSize(totalScript.raw().size());
	str += totalScript.raw();
	
	return str;
}


std::string Transaction::sizeIndicationAndScript(const BinaryScript& script, const bool includeThis)
{
	std::string ret;

	if(includeThis)
	{
		ret += encodeScript(script);
	}
	else
	{
		ret += varIntSize(0);		
	}
	
	return ret;	
}






std::string TxData::createTransactionInputs(const bool signTrans, const TxInput& thisInput,  const std::list<BigInt<256> >& tempKeys) const
{
	const bool enableLocktime = (locktime != 0);
	
	const std::string sequenceStr = Transaction::sequence(enableLocktime);
	
	
	std::string str;
	
	
	const std::string inputCountStr = Transaction::inputCount(inputs.size());
	str += inputCountStr;

	
	std::list<BigInt<256> >::const_iterator xt = tempKeys.begin();

	for(std::list<TxInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TxInput txInput = (*it);

		const std::string prevTxHashStr = Transaction::prevTxHash(txInput.input.txHash);
		const std::string outputIndexStr = Transaction::outputIndex(txInput.input.index);
		
		str += prevTxHashStr;
		str += outputIndexStr;
		
		
		if(!signTrans)
		{
			const bool includeThis = (thisInput.input.txHash.hash == txInput.input.txHash.hash && thisInput.input.index == txInput.input.index);

			//Unsigned version, just take the given input script as it is			
			const std::string scriptSigTotalStr = Transaction::sizeIndicationAndScript(txInput.input.script, includeThis);
			str += scriptSigTotalStr;				
		}
		else
		{
			//std::cout << "Adding signed part: " <<  txInput.txHash.hash << std::endl;
			//Signing
			
			if(txInput.scriptIsSigned())
			{
				//The provided script has already have been correctly signed (eg multisig)

				//std::cout << "Will add script" << std::endl;
				const std::string scriptSigTotalStr = Transaction::encodeScript(txInput.input.script);
				str += scriptSigTotalStr;
				
			}
			else
			{
				
				if(xt == tempKeys.end())
				{
					throw std::string("Error, no more temp keys");
				}

				const BigInt<256>& tempKey = (*xt);
				++xt;

				
				
				//Recursive call to create transaction to sign
				
				const std::list<BigInt<256> > emptyKeys;
				
				const BinaryTransaction toSignStr = createTransactionInternal(false, txInput, emptyKeys);
				//std::cout << "USING TEMPKEY: " << tempKey << std::endl;
				
				const bool payToPubkey = (txInput.extra.txInputType == InputExtraInfo::PAYTO_PUBKEY);
				
				const RSVal rs = Transaction::getSignature(txInput.extra.privkey, tempKey, toSignStr);

				//Get a signed script
				const BinaryScript myTotalScript = Script::getscriptPartSignedTot(rs, txInput.extra.privkey, payToPubkey);
				//const Script signedScript = Script::parse(myTotalScript);
				//std::cout << "Signed script: " << signedScript.toString() << std::endl;


				const std::string scriptSigTotalStr = Transaction::encodeScript(myTotalScript);
				str += scriptSigTotalStr;	
				
								
			}
						
		}

		
		str += sequenceStr;	
		
	}
		
	return str;
}



//Todo: Member in class TxData
std::string TxData::createTransactionOutputs() const //const std::list<TransactionOutput>& recipients)
{
	std::string str;
		
	const std::string numberOutputsStr = Transaction::numberOutputs(recipients.size());
		
	str += numberOutputsStr;
	for(std::list<TransactionOutput>::const_iterator it = recipients.begin() ;
		it != recipients.end() ; 
		++it)
	{
		const TransactionOutput& addressAndAmount = (*it);
		
		const std::string amountStr = Transaction::encodeAmount(addressAndAmount.amount.amount);
		str += amountStr;

		const BinaryScript script = addressAndAmount.script;		
		str += Transaction::encodeScript(script);
	}
	
	return str;
}






BinaryTransaction TxData::createTransactionInternal(const bool signTrans, const TxInput& thisInput, const std::list<BigInt<256> >& tempKeys) const
{
	std::string str;

	const std::string versionStr = Transaction::getVersion();	
	str += versionStr;
	

	str += createTransactionInputs(signTrans, thisInput, tempKeys);
	
	
	str += createTransactionOutputs();
	

	const std::string locktimeStr = Transaction::locktime(locktime);
	str += locktimeStr;
	
	if(!signTrans)
	{
		const std::string hashCodeTypeStr = Transaction::hashCodeType();	
		str += hashCodeTypeStr;
	}


	return BinaryTransaction(str);
}





	

BinaryTransaction TxData::createSignedTransaction(const std::list<BigInt<256> >& tempKeys) const
{	
	TxInput dummyTxInput;
	
	const BinaryTransaction signedTransaction = createTransactionInternal(true, dummyTxInput, tempKeys);

	return signedTransaction;
}



int TxData::getNumTempkeysNeeded() const
{
	int numNeeded(0);
	
	for(std::list<TxInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TxInput txInput = (*it);
		const BigInt<256> one(1);
		
		if(txInput.extra.privkey.key == one)
		{
			//Nothing
		}
		else
		{		
			numNeeded++;
		}
	}
	
	return numNeeded;
}



Amount TxData::getMinerFee() const
{
	BigInt<128> minerFee;

	for(std::list<TxInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TxInput txInput = (*it);
		
		minerFee += txInput.extra.amount.amount;
	}
	
	for(std::list<TransactionOutput>::const_iterator it = recipients.begin() ;
		it != recipients.end() ;
		++it)
	{
		TransactionOutput scriptAndAmount = (*it);
		
		minerFee -= scriptAndAmount.amount.amount;
	}
	
	const Amount minerFeeVal(minerFee);

	return minerFeeVal;
}


//Todo: Return string instead of printing directly
void TxData::printTransactionInformation() const
{
	const BitcoinEllipticCurve ec;
	//BigInt<128> minerFee;
	
	
	std::cout << "============= Num inputs: " << inputs.size() << std::endl;
	//std::cout << std::endl;		


	for(std::list<TxInput>::const_iterator it = inputs.begin() ;
		it != inputs.end() ;
		++it)
	{
		const TxInput txInput = (*it);
		const Script script(Script::parse(txInput.input.script));

		std::cout << "Tx  : " << hexString(txInput.input.txHash.hash.getBinaryLong());
		std::cout << " at " << txInput.input.index << " (" << script.scriptTypeString() << ")" << std::endl;
		//std::cout << "Script type: " << script.scriptTypeString() << std::endl;
		
		std::cout << "Addr: " << minSize(txInput.extra.privkey.getAddress(ec).toString(), 34);
		std::cout << " Amount : " << txInput.extra.amount.amountString() << " " << getCurrencyShortName() << std::endl;
		std::cout << std::endl;		
	}



	std::cout << "============= Num outputs: " << recipients.size() << std::endl;

	for(std::list<TransactionOutput>::const_iterator it = recipients.begin() ;
		it != recipients.end() ;
		++it)
	{
		TransactionOutput scriptAndAmount = (*it);;
		std::cout << "Addr: " << minSize(/*ScriptDecoder::scriptToAddress(*/scriptAndAmount.script.scriptToAddress(), 34) << " ";
		std::cout << "Amount : " << scriptAndAmount.amount.amountString() << " " << getCurrencyShortName() << std::endl;
		//std::cout << std::endl;
	}


	const Amount minerFeeVal = getMinerFee();
	
	std::cout << std::endl;	
	std::cout << "                                      Miner fee : " << 
				minerFeeVal.amountString() << " " << getCurrencyShortName() << std::endl;
	std::cout << std::endl;

	if(minerFeeVal.amount.isNegative())
	{
		std::cout << "Warning: Negative miner fee" << std::endl;
		std::cout << std::endl;
	}

	if(locktime != 0)
	{
		std::cout << "Locktime: " << locktime; 	
		
		if(locktime < 500000000)
		{
			std::cout << " (block height)" << std::endl;
		}
		else
		{
			std::cout << " (Unix epoch time)" << std::endl;	 		
		}
		
		std::cout << std::endl;
	}

}

