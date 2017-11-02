
/**
 * hash.cpp - Bitcoin address generator
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
 
#include "hash.h"



Sha256::Sha256()
{
	SHA256_Init(&ctx256);	
}


void Sha256::add(const std::string& str)
{
	SHA256_Update(&ctx256, (unsigned char*)str.c_str(), str.size());	
}

std::string Sha256::getHash()
{
	unsigned char buf[512];
	
	SHA256_Final(buf, &ctx256);
	const std::string ret((const char*)buf, 32);
	return ret;
}



std::string sha256(const std::string& str)
{
	SHA256_CTX	ctx256;
	unsigned char buf[512];
	
	SHA256_Init(&ctx256);
	SHA256_Update(&ctx256, (unsigned char*)str.c_str(), str.size());
	SHA256_Final(buf, &ctx256);
	
	const std::string ret((const char*)buf, 32);
	return ret;	
}


std::string doubleSha256(const std::string& str)
{
	return sha256(sha256(str));
}


std::string sha512(const std::string& str)
{
	SHA512_CTX	ctx512;
	
	SHA512_Init(&ctx512);
	SHA512_Update(&ctx512, (unsigned char*)str.c_str(), str.size());

	unsigned char buf[1024];
	SHA512_Final(buf, &ctx512);
	
	const std::string ret((const char*)buf, 64);
	return ret;	
}



std::string ripemd160(const std::string& str)
{
	char hash[160];
	ripemd160(str.c_str(), str.size(), hash);

	std::string result(hash, 20);
	return result;
}

