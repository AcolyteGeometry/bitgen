/**
 * random.cpp - Bitcoin poscript generator
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

#include "random.h"




BigInt<256> getRandomFrom6SideDice(const std::string& dice6, const int numThrows)
{	
	if(dice6.size() < numThrows)
	{
		std::cout << "Error, must have at least " << numThrows << " dice throws, have: " << dice6.size() << std::endl;
		throw std::string("Need more throws");
	}
	
	BigInt<256> dice;
	
	for(int i = 0 ; i < dice6.size() ; i++)
	{
		const char c = dice6[i];
		if(!(c >= '1' && c <= '6'))
		{
			std::cout << "Error, illegal char: " << c << std::endl;
			throw std::string("Illegal char");			
			//exit(1);
		}
		const int val = c - '1';
		dice *= 6;		
		dice += val;
	}
	
	//std::cout << "Result (hex): " << dice << std::endl;
	//std::cout << "Result digits: " << dice.numberBitsNeeded() << std::endl;

	return dice;
}
