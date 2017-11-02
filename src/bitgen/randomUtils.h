/**
 * randomUtils.h - Bitcoin address generator
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

#include "bitcoinKeyPair.h"

#include <list>

std::list<BigInt<256> > getTempKeys(const int num, const bool fixednounce = false);

BigInt<256> getRandomNumber(const bool longbuf = false);
BigInt<256> getBlockCalcRandomNumber();

unsigned char getNonblockRandomChar(bool& gotChar);
unsigned char getURANDNonblockRandomChar(bool& gotChar);


std::string getFromDevRandom(const int numVal);
void checkPassphraseEntropy(const std::string& passphrase);
bool verifyHashEntropy(const std::string& hashval, const int minChar = 50);
void checkHashEntropy(const std::string& hashval, const int minChar = 50);
BitcoinKeyPair generateRandomAddress(const BigInt<256>& privKey, std::list<std::string>& generationExtra, const bool compressed);
void testGeneration(const BigInt<256>& privKey, const std::string& privVal, const std::string& pubVal);


