#ifndef BITSIG_FILE_FORMAT_H
#define BITSIG_FILE_FORMAT_H

/**
 * bitgenFileFormat.h - Bitcoin poscript generator
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


#include "pkif.h"
#include "pubKey.h"
#include "bitcoinKeyPair.h"
#include "base58.h"



class PubKeyAlias
{
public:	
	PubKeyAlias(const std::string& inData, const std::string& inAlias, const bool inIsAddress) : 
		isAddress(inIsAddress), data(inData), alias(inAlias) {}


	PubKeyAlias(const PubAddress& inPubkey, const std::string& inAlias) : 
		isAddress(true), data(inPubkey.toString() /*address*/), alias(inAlias) {}

	PubKeyAlias(const Pkif& inPkif, const std::string& inAlias) : 
		isAddress(false), data(inPkif.toString()), alias(inAlias) {}


	Pkif getPkif() const
	{
		Pkif pkif(data); // = Pkif::parsePkif(data);
		return pkif;
	}

	PubAddress getPubAddr() const
	{
		if(isAddress)
		{
			const PubAddress pubAddress(data);
			return pubAddress;
		}
		else
		{
			const Pkif pkif(data); // = Pkif::parsePkif(data);
			const PubKey pubKey = pkif.getPubKey();		
			const PubAddress pubAddress = pubKey.getAddress();
			return pubAddress;
		}
	}


	bool isAddr() const
	{
		return isAddress;
	}

	//const Pkif pkif;
	//const PubAddress pubkey;
	bool isAddress;
	std::string data;
	const std::string alias;
};


bool operator<(const PubKeyAlias& a, const PubKeyAlias& b);

PubKeyAlias parsePubAddr(const std::string& str);
PubKeyAlias parsePubKey(const std::string& str);
PubKeyAlias parseImportedPublic(const std::string& pubfile);
std::string generateKeyStoreFile(const BitcoinKeyPair& keypair);
std::string parseWifFromPrivFile(const std::string& str);


#endif
