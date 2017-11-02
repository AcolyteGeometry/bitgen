/**
 * bitcoinKeyPair.cpp - Bitcoin generator
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

#include "bitcoinKeyPair.h"
#include "privKey.h"


BitcoinKeyPair BitcoinKeyPair::calcKeys(const PrivKey& privKey)
{
	const Wif wif = privKey.getWif();
	
	BitcoinEllipticCurve ec;
	
	const PubKey pubKey = privKey.getPubkey(ec);
	const PubAddress pubAddr = pubKey.getAddress();
	
	const BitcoinKeyPair keys(pubAddr, wif);
	return keys;
}


//Todo: Remove, not used
/*
BitcoinKeyPair BitcoinKeyPair::calcDisplayKeys(const ECPoint& publicKeyPoint, const PrivKey& privKey)
{	
	const PubAddress pubkey = PubAddress::bitcoinAddresFromPubkey(publicKeyPoint, privKey.compressed);

	const Wif wif = privKey.getWif();
	
	const BitcoinKeyPair keys(pubkey, wif);
	return keys;
}
*/








bool operator<(const BitcoinKeyPair& a, const BitcoinKeyPair& b)
{
	return (a.pubaddr.toString() /* address */ < b.pubaddr.toString() /* address */);
}

