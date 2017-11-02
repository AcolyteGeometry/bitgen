#ifndef VANITY_SEARCHER_H
#define VANITY_SEARCHER_H

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
 

#include "privKey.h"
#include "RangeSearcher.h"


#include <string>


class VanitySearcher
{
public:
	//VanitySearcher() {}
	
	
	static void searchForVanityAddress(const std::string& searchPatternRaw, const bool usePool,
		const std::string& pkifStr, const std::string& thetime, const std::string& accountName);
		
		
	static void generateVanityAddress(const PrivKey& startPointPriv, const RangeSearcher& rangeSearcher,
		const bool compressed, const std::string& searchPatternRaw, const std::string& thetime,
		const std::string& accountName, const BigInt<256> estimatedTriesBig);
		
		
	static void generateVanityAddressPool(const Pkif& givenPkif, const PrivKey& priv, const RangeSearcher& rangeSearcher,
		const std::string& searchPatternRaw, const std::string& thetime,
		const std::string& accountName, const BigInt<256>& estimatedNumTries);
		
	static bool tryAddPatternRange(RangeSearcher& rangeSearcher, 
		const std::string& startAdr, const std::string& endAdr);
		
		
private:
	static const std::vector<std::string> addFirstChar(const std::string first, const std::vector<std::string>& permutations);
	static std::string repeat(const char c, const int num);
};


#endif

