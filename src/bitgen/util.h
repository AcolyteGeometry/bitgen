#ifndef BITGEN_UTIL_H
#define BITGEN_UTIL_H

/**
 * util.h - Bitcoin poscript generator
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
#include <list>
#include "bigint.h"


int min(const int a, const int b);

std::string convertFromHex(const std::string hexString);

std::string getHexString(const std::string& str);


BigInt<256> plainToBigInt(std::string str);


std::string repeat(const char c, const int num);

std::string doubleToString(const double& val);
std::string intToString(const int num);

std::string hexStrToBytes(const std::string& cStr);
std::string removeNewLine(const std::string& str);


std::string minSize(std::string str, const int minSize, const bool right = false);
//std::string minSize(std::string str, const int minSize);

bool endsWith(const std::string& str, const std::string ending);
bool startsWith(const std::string& str, const std::string& start);

std::string removePrefix(const std::string& str, const std::string& start);


std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);

std::string removeNewline(const std::string& str);


bool isHexDigit(const char c);
std::string charToHex(const unsigned char c);


std::list<std::string> splitLines(const std::string& str);

#endif
