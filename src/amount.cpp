/**
 * transaction.cpp - Bitcoin poscript generator
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


#include "transaction.h"
#include "randomUtils.h"
#include "scriptdecoder.h"


Amount operator-(const Amount& a, const Amount& b)
{
	const Amount tot = a.amount - b.amount;
	return tot;
}





//No "."   => Integer representing the number of satoshi
//With "." => Fraction representing the number of btc etc
// (100 000 000 satoshi)
BigInt<128> Amount::parseAmountString(const std::string& str)
{
	const int dotPos = str.find(".");
	
	if(dotPos == std::string::npos)
	{
		//std::cout << "No dot found" << std::endl;
		const BigInt<128> amount(BigInt<128>::fromDec(str));
		return amount;
	}
	else
	{
		//std::cout << "Dot found" << std::endl;
		const std::string wholeStr = str.substr(0, dotPos);
		const BigInt<128> whole(BigInt<128>::fromDec(wholeStr));
		
		const std::string fractionStr = str.substr(dotPos + 1);
		if(fractionStr.size() > 8)
		{
			throw std::string("Integer fraction too large");
		}
		
		//Add zeroes
		std::string zeroes((8 - fractionStr.size()), '0');
		const std::string totalFractionStr = fractionStr + zeroes;
		//std::cout << "Total fractionStr: " << totalFractionStr << std::endl;
		
		const BigInt<128> fraction(BigInt<128>::fromDec(totalFractionStr));
		//.1  -> 100000000
		//.10 -> 100000000
		//.100-> 100000000
		
		const BigInt<128> hundredMillion(100000000);
		
		const BigInt<128> total = whole * hundredMillion + fraction;
		//std::cout << whole << "." << fraction << std::endl;
		
		return total;		
	}
	
	//BigInt<128>::fromDec(prevAmountStr));
}







std::string Amount::amountString() const
{
	std::string ret;
	BigInt<128> tempAmount(amount);
	
	if(tempAmount.isNegative())
	{
		ret += "-";
		tempAmount = tempAmount * -1;
	}

	const BigInt<128> hundredMillion(100000000);
	const BigInt<128> amountWhole(tempAmount / hundredMillion);
	const BigInt<128> amountFraction(tempAmount % hundredMillion);

	
	ret += amountWhole.strDec();
	ret += ".";
	ret +=  amountFraction.strDec(8);
	
	return ret;
}

