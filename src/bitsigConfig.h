#ifndef BITSIG_CONFIG_H
#define BITSIG_CONFIG_H


/**
 * bitgenConfig.h - Bitcoin poscript generator
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

#include "pubAddress.h"
#include "pkif.h"
#include "bitsigFileFormat.h"
#include "privKey.h"

#include "bitcoinKeyPair.h"

#include <list>
#include <set>


BigInt<256> getPrivKeyForSigning(bool& compressed);

Pkif getPublicKeyForName(const std::string& alias);


class BitsigConfig
{
public:	
	static std::string getConfigDir();

	static void createConfigDir(const std::string& configDir);
	static std::string getOrCreateConfigDir();

	static void addPublicAddress(const PubAddress& addr, const std::string& name);
	static void addPublicKey(const Pkif& pkif, const std::string& name);
	
	static void storePrivateKey(const BitcoinKeyPair& keypair);
	
	static PubAddress getPubForDefaultPriv();
	static BigInt<256> getDefaultPrivkey(bool& compressed);
	static bool checkIfDefaultExist();
	static void setDefaultKey(const PubAddress& addr);		//Sets default private

	static bool hasPrivateForPublic(const PubAddress& myPubAddr);


	static bool findStoredPubAddr(const PubAddress& bitaddress, std::string& alias);
	static bool findStoredPubAddrList(std::list<ECPoint>& possiblePoints, std::string& alias, ECPoint& foundPubkey);
	
	
	static std::set<PubKeyAlias> getAllStoredPub();
	static bool hasPublic(const PubAddress& myPubAddr);
	
	
	static std::set<BitcoinKeyPair> getOwnKeypairs(std::string& defPrivContent);
	static std::set<Wif> getOwnPrivate(std::string& defPrivContent);


	static int numOwnKeypairs();
	static int numPublicKeys();

	static void listPrivateAddresses();
	static void listPublicAddresses(const bool listOnlyAddresses = false);
	static void listPublicKeys();	
	static void listOwnPublicAddresses();
	static void listOwnPublicKeys();
	
	
	static void deletePrivate(const PubAddress& myPubAddr);
	static void deletePublic(const PubAddress& myPubAddr);


	static void deleteDefault();	
};



#endif

