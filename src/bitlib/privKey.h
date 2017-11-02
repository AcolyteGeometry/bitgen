#ifndef BITGEN_PRIV_KEY_H
#define BITGEN_PRIV_KEY_H

/**
 * privKey.cpp - Bitcoin poscript generator
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

#include "wif.h"

#include "pubKey.h"

class PrivKey
{
public:

	//Todo: Test this
	//PrivKey(const std::string& binVal, const bool inCompressed) : 
	//	key(stringToInt(val)), compressed(inCompressed) {}


	PrivKey(const BigInt<256>& val, const bool inCompressed) : 
		key(val), compressed(inCompressed) {}
	


	//Prefer this method, reduces ec initialization
	ECPoint point(const BitcoinEllipticCurve& ec) const;
	
	
	//Avoid this method, use point(ec) instead
	/*
	ECPoint pointWithEc() const
	{		
		const BigInt<1024> privKeyRoot(BigInt<1024>::asPositive(key));	//Using 1024 bits during conversion
			
		BitcoinEllipticCurve ec;			
		const ECPoint PublicKey = ec.multiplyGenPoint(privKeyRoot);
	
		return PublicKey;
	}
	*/
	
	//Todo: Rename to getPub() !!!!!!!!!
	PubKey getPubkey(const BitcoinEllipticCurve& ec, const bool onlyIndicatedUsage = false, const bool signVerify = false) const
	{
		const ECPoint pubpoint = point(ec);
		const PubKey pubkey(pubpoint, compressed, onlyIndicatedUsage, signVerify);
		return pubkey;
	}
	
	PubAddress getAddress(const BitcoinEllipticCurve& ec) const
	{
		const PubKey pubKey = getPubkey(ec);
		return pubKey.getAddress();
	}

	Wif getWif() const
	{
		const Wif wif = Wif::privkeyToWIF(key, compressed);
		return wif;
	}
	
	std::string toString() const
	{
		Wif wif = getWif();
		return wif.toString();
	}
	
	BigInt<257> getKey() const
	{
		return BigInt<257>::asPositive(key);
	}

	const BigInt<256> key;
	const bool compressed;
	
private:
	BigInt<256> stringToInt(const std::string& val)
	{
		if(val.size() != 32)
		{
			throw std::string("Error, exactly 64 hex digits needed");
		}
		const BigInt<256> priv(val.c_str(), 'c');
		return priv;
	}
	
};


std::ostream& operator<<(std::ostream& str, const PrivKey& k);



#endif
