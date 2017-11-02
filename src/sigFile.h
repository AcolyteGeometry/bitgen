#ifndef SIG_SIG_FILE_H
#define SIG_SIG_FILE_H

/**
 * sigFile.h - Bitcoin address generator
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


#include "bigint.h"
#include "bitcoinEC.h"
#include "pubAddress.h"
#include "signature.h"
#include "RSVal.h"

#include <list>

class SigFileContent
{
public:
	SigFileContent(const bool inFirstX, const bool inOddY, const bool inCompressed, const RSVal& inRs) :
		firstX(inFirstX), oddY(inOddY), compressed(inCompressed), rs(inRs)
	{
		
	}



	//Todo: Use IndexAndParity
	const bool firstX;
	const bool oddY;
	const bool compressed;

	const RSVal rs;
	//const ECPoint pubkey;	//Todo: Remove
};



class ArmoryFileContent
{
public:
	ArmoryFileContent(const PubAddress& inAddress, const BigInt<256>& inHash, const ECPoint& inPubkey, const RSVal& inRs) :
		address(inAddress), hash(inHash), pubkey(inPubkey), rs(inRs)
	{
		
	}

	const PubAddress address;
	const BigInt<256> hash;
	const RSVal rs;
	const ECPoint pubkey;
};






class InputsIoData
{
public:	
	InputsIoData(const std::string& inMessage, const std::string& inAddress, const std::string& inSignature) :
		message(inMessage), address(inAddress), signature(inSignature) {}



	const std::string message;
	const std::string address;
	const std::string signature;
};


std::string encodeRs(const RSVal& rs, const IndexAndParity& iap, const bool compressed);


void validateSigatureByte(const unsigned char signatureByte);


std::string createSignatureContent(const BigInt<256>& hashNum, const BigInt<256>& r, const BigInt<256>& s, 
									const bool indexZero, const bool oddY, const bool compressed);

std::string createFileSignatureContent(const BigInt<256>& hashNum, const BigInt<256>& r, const BigInt<256>& s, 
	const bool indexZero, const bool oddY, const bool compressed, const std::string& content, const bool rawContent);


SigFileContent parseSignedWithIncludedFile(const std::string& signFile, std::string& fileContent, const bool rawContent);
SigFileContent parseSignFile(const std::string& signFile);

InputsIoData parseMultibit(const std::string& str);
InputsIoData parseInputsIo(const std::string& str);
std::string parseArmoryBase64(const std::string& str);
std::string parseArmoryClear(const std::string& str, std::string& message);


RSVal decodeBase64Signature(const std::string& signature, bool& firstX, bool& oddVal, bool& compressed);
RSVal decode65SignatureBytes(const std::string& decoded, bool& firstX, bool& oddVal, bool& compressed);

//IndexAndParity getIndexAndParity(const RSVal& rs, const ECPoint& pubkey, const BigInt<256>& hashNum);

std::string generateArmoryClearFileFormat(const std::string& message, const RSVal& rs, 
			const IndexAndParity& iap, const bool compressed);


std::string generateArmoryFilFormat(const ECPoint& pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message);

std::string generateMultibitFileFormat(const ECPoint& pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message, const IndexAndParity& iap, const bool compressed);

std::string generateInputsIoFileFormat(const ECPoint& not_used_pubkey, const RSVal& rs, const PubAddress& bitcoinAddress,
	const std::string& message, const IndexAndParity& iap, const bool compressed);

std::string generateArmoryA46FileFormat(const std::string& message, const RSVal& rs, 
			const IndexAndParity iap, const bool compressed);


#endif
