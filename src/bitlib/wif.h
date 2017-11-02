#ifndef WIF_ADDRESS_H
#define WIF_ADDRESS_H

/**
 * wif .h - Bitcoin poscript generator
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
 
 
#include <string>
#include "bigint.h" 
#include "pkif.h" 
 
class PrivKey;


 
class Wif
{
public:
	explicit Wif(const std::string& inWif) : m_wif(inWif) 
	{
		bool compressed(false);
		wifToPrivKey(compressed); //Make sure the wif is valid
	}
	
	PrivKey getPriv() const;
	
	static Wif privkeyToWIF(const BigInt<256>& privkey, bool compressed);

	std::string toString() const
	{
		return m_wif;
	}


	friend bool operator<(const Wif& a, const Wif& b);
	friend std::ostream& operator<<(std::ostream& stream, const Wif& addr);
private:
	const BigInt<256> wifToPrivKey(bool& compressed) const;

	const BigInt<256> getPrivKey(bool& compressed) const
	{
		return wifToPrivKey(compressed);
	}

	const BigInt<256> toPrivKey() const
	{
		bool compressed(false);
		return wifToPrivKey(compressed);
	}


	//Todo: Make private
	const std::string m_wif;
};



std::ostream& operator<<(std::ostream& stream, const Wif& addr);

bool operator<(const Wif& a, const Wif& b);


#endif
