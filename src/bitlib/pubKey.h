#ifndef PUBKEY_H
#define PUBKEY_H

/**
 * pubKey.h - Bitcoin poscript generator
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
#include "bigintutil.h"


class PubKey
{
public:
	PubKey(const ECPoint& inP, const bool inCompressed) : 
		point(inP), compressed(inCompressed), onlyIndicatedUsage(false), signVerify(false) {}

	PubKey(const ECPoint& inP, const bool inCompressed, const bool inOnlyIndicatedUsage, const bool inSignVerify) :
		point(inP), compressed(inCompressed), onlyIndicatedUsage(inOnlyIndicatedUsage), signVerify(inSignVerify) {}


	static PubKey fromBin(const std::string& pubkey)
	{
		//const std::string pubkey = stream.getStr(pubkeySize);	
		//std::cout << "Pubkey: " << hexString(pubkey) << std::endl;
	
		bool compressed(false);
		const ECPoint point = ECPoint::parsePointBin(pubkey, compressed);
		const PubKey pubKey(point, compressed);
		return pubKey;
	}


	static PubKey fromHex(const std::string& pubKeyStr)
	{
		bool compressed(false);
		const ECPoint point = ECPoint::parsePointHex(pubKeyStr, compressed);
		const PubKey pubKey(point, compressed);
		return pubKey;
	}


	static PubKey fromPkif(const std::string& str)
	{
		bool compressed(false);
		bool onlyIndicatedUsage(false);
		bool signVerify(false);
		const ECPoint point = Pkif::parsePkif(str, compressed, onlyIndicatedUsage, signVerify);		
		const PubKey pubkey(point, compressed, onlyIndicatedUsage, signVerify);
		return pubkey;
	}

	static bool isPubkey(const std::string& pubKeyStr)
	{
		bool compressed(false);
		try
		{
			const ECPoint point = ECPoint::parsePointHex(pubKeyStr, compressed);
		}
		catch(const std::string& err)
		{
			return false;
		}
		
		return true;
	}
	
	
	std::string toString() const
	{
		return createPublicKeyImportFormat();
	}

	
	std::string toStringHex() const
	{
		return hexString(point.getRaw(compressed));
	}
	
	std::string getRaw() const
	{
		return point.getRaw(compressed);
	}	

	Pkif getPkif() const
	{
		const Pkif pkif = createPublicKeyImportFormat();
		return pkif;
	}

	PubAddress getAddress() const
	{
		const PubAddress pubAddress = PubAddress::bitcoinAddresFromPubkey(point, compressed);
		return pubAddress;
	}

	//Todo: Return val?
	std::string getChecksum() const;


//Todo: Make private?
	const ECPoint point;
	const bool compressed;
	const bool onlyIndicatedUsage;
	const bool signVerify;

private:	
	std::string createPublicKeyImportFormat() const;
	std::string getRawPKIFWithoutChecksum() const;

	static ECPoint parseVerbosePubKey(const BigInt<1024>& x);	
};

bool operator<(const PubKey& a, const PubKey& b);


#endif
