#ifndef BITGEN_SIGNATURE_H
#define BITGEN_SIGNATURE_H

/**
 * signature.h - Bitcoin address generator
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
 

#include "bitcoinEC.h"
#include "pubAddress.h"
#include "RSVal.h"

#include <list>



class IndexAndParity
{
public:
	IndexAndParity(const bool inIndexZero, const bool inParityOdd) :
		indexZero(inIndexZero), parityOdd(inParityOdd) {}
	
	const bool indexZero;
	const bool parityOdd;
	
};


IndexAndParity getIndexAndParity(const RSVal& rs, const ECPoint& pubkey, const BigInt<256>& hashNum);


class SignatureImplementation
{
public:
	static RSVal sign(const BigInt<256>& myPrivKey, const BigInt<256>& myTempRand, const BigInt<256>& myhashToSign);
	static bool checkSigLowS(const BigInt<256>& hash, const ECPoint& pubkey, const RSVal& rs, const bool requireLowS = false);

	static void verifyPubaddrSpecifiedByUser(const std::string & pubadd, const PubAddress& bitaddress, std::string& alias);

	static std::string verifySignatureForFile(const std::string& signedFile,const std::string& pubadd, std::string& alias, 
									std::string& verifiedAddress, const bool rawContent);


	static std::string hashStringStart(const int size);

	static std::string signString(const std::string& content, const BigInt<256>& privKey, 
					const BigInt<256>& tempKey, const bool& compressed, const bool rawContent);


	static ECPoint recoverThePublicKey(const bool firstX, const bool parity, const RSVal& rs,const BigInt<256>& hash);

	static std::list<ECPoint> recoverPossiblePubKeys(const ECPoint& rs,const BigInt<256>& hash);


	static bool verifySign(const RSVal& rs, const ECPoint& pubkey, const BigInt<256>& myhashToSign, const bool requireLowS = false);

	static std::list<std::string> getPossibleAdrFromPossiblePoints(const std::list<ECPoint>& possiblePoints);
	static ECPoint getPointForGivenAddress(const std::list<ECPoint>& possiblePoints, const PubAddress& givenPub);

	static std::string possiblePointsStr(const std::list<ECPoint>& possiblePoints);

	static bool getParity(const ECPoint& pubkey, const bool indexZero, const RSVal& rs, const BigInt<256>& hashNum);

};







#endif
