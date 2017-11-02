#ifndef BINARY_SCRIPT_CLASS_H
#define BINARY_SCRIPT_CLASS_H

/**
 * Script.h - Bitcoin poscript generator
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
#include "bigintutil.h"

class BinaryScript
{
public:
	explicit BinaryScript(const std::string& inScript) : script(inScript) {}
	
	std::string raw() const { return script; }
	
	std::string hex() const
	{
		return hexString(raw());
	}
	
	std::string scriptToAddress() const;
	std::string scriptTypeString()  const;
	
	
	const std::string script;
};



#endif
