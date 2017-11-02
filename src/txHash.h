#ifndef TXHASH_H
#define TXHASH_H

/**
 * transaction.h - Bitcoin poscript generator
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


class TxHash
{
public:
	TxHash(const std::string& str) : hash(BigInt<256>::fromHex(str))
	{
		
	}

	TxHash(const std::string& str, bool dummy) : hash(BigInt<256>::fromBytes(str))
	{
		
	}

	std::string getReversed() const
	{
		std::string ret;
		const std::string bytes = hash.getBinaryLong();
		for(int i = (bytes.size() - 1) ; i >= 0 ; i--)
		{
			const unsigned char c = bytes[i];
			ret += c;
		}
		return ret;
	}


	bool zero() const
	{
		BigInt<256> zeroNum;
		return (hash == zeroNum);
	}

	//std::string toBin() const
	//{
	//	return hash.getBinaryLong();
	//}


	std::string toString() const
	{
		//return hash.strHex();
		return hexString(hash.getBinaryLong());
	}


	//Todo: Store as string instead?
	BigInt<256> hash;
};


#endif

