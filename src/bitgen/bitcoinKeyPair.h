#ifndef BITCOIN_KEY_PAIR_H
#define BITCOIN_KEY_PAIR_H

/**
 * bitcoinKeyPair.h - Bitcoin poscript generator
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
#include "wif.h"


class BitcoinKeyPair
{
public:
	BitcoinKeyPair(const PubAddress& inPubaddr, const Wif& inPrivaddr) : 
		pubaddr(inPubaddr), privaddr(inPrivaddr)
		{}

	static BitcoinKeyPair calcKeys(const PrivKey& privKey);
	
	
	//Todo: Remove, not used
	//static BitcoinKeyPair calcDisplayKeys(const ECPoint& publicKeyPoint, const PrivKey& privKey);

		
	const PubAddress pubaddr;
	const Wif privaddr;	
};



bool operator<(const BitcoinKeyPair& a, const BitcoinKeyPair& b);


#endif
