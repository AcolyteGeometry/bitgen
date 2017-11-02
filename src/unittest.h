#ifndef BITGEN_UNIT_TEST_H
#define BITGEN_UNIT_TEST_H

/**
 * unittest.h - Bitcoin poscript generator
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
  
void unitTestsBitcoin();

void testAddressGeneration(const std::string& wifStr, const std::string& address);

void testMultisigGeneric(const std::list<std::string>& pkifStrList,  
					const int numReq, const std::string& pubAddrStr);

void testMultisigPubKeyGeneric(const std::list<std::string>& pubkeyStrList,  
					const int numReq, const std::string& pubAddrStr);

#endif

