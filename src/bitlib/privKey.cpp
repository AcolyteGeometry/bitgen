/**
 * privKey.cpp- Bitcoin poscript generator
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
 
 
#include "privKey.h"


std::ostream& operator<<(std::ostream& str, const PrivKey& k)
{
	//const BigInt<257> pk(BigInt<257>::asPositive(k.key));	//Make sure the number is not negative
	str << hexString(k.key.getBinaryFull());
	
	//str << pk;
	return str;
}



ECPoint PrivKey::point(const BitcoinEllipticCurve& ec) const
{		
	const BigInt<1024> privKeyRoot(BigInt<1024>::asPositive(key));	//Using 1024 bits during conversion
			
	const ECPoint PublicKey = ec.multiplyGenPoint(privKeyRoot);
	
	return PublicKey;
}

