#ifndef BITCOIN_HASH_H
#define BITCOIN_HASH_H

/**
 * hash.h - Bitcoin poscript generator
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

#include "sha2.h"
#include "ripemd160.h"

#include <string>

std::string sha256(const std::string& str);
std::string sha512(const std::string& str);

std::string doubleSha256(const std::string& str);


std::string ripemd160(const std::string& str);

class Sha256
{
public:	
	Sha256();
	void add(const std::string& str);
	std::string getHash();

private:
	SHA256_CTX	ctx256;
};



#endif
