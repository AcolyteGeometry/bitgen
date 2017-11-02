#ifndef SCRIPT_ITEM_H
#define SCRIPT_ITEM_H

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




class ScriptItem
{
public:
	enum ScriptThing {OP_NOP, OP_0, /*OP_TRUE,*/ OP_HASH160, OP_EQUALVERIFY, OP_CHECKSIG, OP_DUP, OP_HASH256, OP_EQUAL, OP_CHECKMULTISIG,
		OP_PUSHDATA1, OP_RETURN, OP_CHECKLOCKTIMEVERIFY, OP_DROP,
		NUMBER, BOOL, DATA};

	ScriptItem(const ScriptThing thing) : object(thing), dataNum(0) {}
	ScriptItem(const ScriptThing thing, const std::string inData) : object(thing), data(inData), dataNum(0) {}
	ScriptItem(const ScriptThing thing, const int inNum) : object(thing), /*data(std::string(1, inNum)),*/ dataNum(inNum) 
	{
		//if(inNum > 255)
		//{
		//	throw std::string("Number too large");
		//}
	}

	static std::string scriptInteger(const int blockHeight);


	std::string toString() const;
	std::string serialize() const;
	
	const ScriptThing object;
	const std::string data;
	const int dataNum;
};




#endif

