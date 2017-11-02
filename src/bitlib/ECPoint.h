#ifndef EC_POINT_H
#define EC_POINT_H

/**
 * ECPoint.h - Bitcoin poscript generator
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


class ECPoint
{
	friend class BitcoinEllipticCurve;
	friend class PubKey;

private:	
	ECPoint(const BigInt<1024>& inX, const BigInt<1024>& inY) : x(inX), y(inY) {}

public:

	static ECPoint parsePointHex(const std::string& pubKey, bool& compressed);
	static ECPoint parsePointBin(const std::string& raw, bool& compressed);
		
		
	std::string getRaw(const bool compressed) const;

	BigInt<1024> x;
	BigInt<1024> y;	


private:
	std::string getRawNonCompressed() const;
	std::string getRawCompressed() const;	
};

bool operator==(const ECPoint& a, const ECPoint& b);
bool operator!=(const ECPoint& a, const ECPoint& b);


std::ostream& operator<<(std::ostream& s, const ECPoint& point);



#endif

