#ifndef AMOUNT_H
#define AMOUNT_H

/**
 * transaction.h - Bitcoin poscript generator
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
#include <iostream>

#include "bigint.h"
#include "RSVal.h"
#include "privKey.h"
#include "signature.h"






class Amount
{
public:
	Amount(const BigInt<128>& inAmount) : amount(inAmount) {}
	Amount() : amount(0) {}

	void operator+=(const Amount& other)
	{
		amount += other.amount;
	}



	static BigInt<128> parseAmountString(const std::string& str);

	std::string amountString() const;


	//const 
	BigInt<128> amount;
};


Amount operator-(const Amount& a, const Amount& b);



#endif
