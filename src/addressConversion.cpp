/**
 * addressConversion.cpp - Bitcoin poscript generator
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
 
 
#include "addressConversion.h"




std::string compressedVal(const BigInt<256> & pubX, const BigInt<256>& pubY)
{
	if(pubY.odd())
	{
		return "03" + pubX.strHex();
	}
	else
	{
		return "02" + pubX.strHex();
	}
}













/*
ECPoint getPublicPointFromPrivateBinary(const BigInt<256>& myPrivKey)
 {			 
	const BigInt<1024> privKey(BigInt<1024>::asPositive(myPrivKey));	//Using 1024 bits during conversion
	  
	BitcoinEllipticCurve ec;
					
	const ECPoint PublicKey = ec.multiplyGenPoint(privKey);
	return PublicKey;
}
*/	
	
	
	



