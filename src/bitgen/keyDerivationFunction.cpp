/**
 * keyDerivationFunction.cpp - Bitcoin address generator
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

#include "argon2d.h"

std::string bitgenArgon2Kdf(const std::string& inval, const std::string& usersalt)
{		
	const std::string salt = usersalt + "12345678901234567890";
	
	const unsigned t_cost = 5;
	const unsigned m_cost = 100000; 
	const unsigned thr = 8;
	
	const unsigned char outlen = 32;
	unsigned char out[outlen];	


	Argon2dRef(out, outlen, (const uint8_t*)inval.c_str(), inval.size(), 
		(const uint8_t*)salt.c_str(), salt.size(), NULL, 0, NULL, 0, t_cost, m_cost, thr);


	const std::string result((char*)out, outlen);
	return result;
}



