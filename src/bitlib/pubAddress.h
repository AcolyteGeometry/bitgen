#ifndef PUB_ADDRESS_H
#define PUB_ADDRESS_H
/**
 * pubAddress.h - Bitcoin poscript generator
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
#include "hash.h"
#include "base58.h"
#include "currencySpecific.h"
#include "BinaryScript.h"


class PubAddress
{
public:
	explicit PubAddress(const std::string& a) : address(a) 
	{
		validatePubAddress(address);
	}

	static bool isAddress(const std::string& address)
	{
		try
		{
			validatePubAddress(address);
		}
		catch(const std::string& err)
		{
			return false;
		}
		
		return true;
	}
	
	
	static PubAddress fromBin(const std::string& theRipe, const char netVersion);
	static PubAddress fromHex(const std::string& hex);
	
	std::string getBin() const;
	
	BigInt<512> getNumeric() const
	{
		return Base58Decode<512>(address);
	}
	
	BigInt<32> getChecksum() const
	{
		return validatePubAddress(address);
	}

	std::string toString() const
	{
		return address;
	}

	bool isMultisig() const;

	//friend bool operator<(const BitcoinKeyPair& a, const BitcoinKeyPair& b);
	static PubAddress pubkeyToBitcoinAddress(const std::string& addressStringRepresentation);
	static PubAddress multisigToBitcoinAddress(const BinaryScript& addressStringRepresentation);


	friend bool operator!=(const PubAddress& a, const PubAddress& b);
	friend bool operator==(const PubAddress& a, const PubAddress& b);
	friend std::ostream& operator<<(std::ostream& stream, const PubAddress& addr);
	friend bool operator<(const PubAddress& a, const PubAddress& b);
	friend class PubKey;	//Todo: Remove!
private:
	static PubAddress stringToBitcoinAddress(const std::string& addressStringRepresentation, const char netVersion);


//Todo: Make private
	const std::string address;


	static PubAddress bitcoinAddresFromPubkey(const ECPoint& pubk, const bool compressed);


	//Todo: Returns???
	static BigInt<32> validatePubAddress(const std::string& a);

	//Todo: Remove this method? No compressed argument present!
	static PubAddress pubPointToBitcoinAddress(const ECPoint& pubkey);

};


bool operator<(const PubAddress& a, const PubAddress& b);
std::ostream& operator<<(std::ostream& stream, const PubAddress& addr);
bool operator==(const PubAddress& a, const PubAddress& b);
bool operator!=(const PubAddress& a, const PubAddress& b);

#endif
