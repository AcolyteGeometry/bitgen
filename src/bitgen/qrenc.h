#ifndef QR_ENCODE_HEADER
#define QR_ENCODE_HEADER

/**
 * qrenc.h - Bitcoin generator
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

#include <vector>
#include <list>
#include <string>

#include "language.h"
#include "qrencode.h"



class QRInfo
{
public:
	QRInfo(int inWidth) : width(inWidth) {}
	
	void add(const bool val)
	{
		values.push_back(val);
	}
	
	int height() const
	{
		return values.size() / width;
	}
	
	bool get(int x, int y) const
	{
		return values[x + y*width];
	}
	
	static QRInfo qrencode(const unsigned char *intext, int length);
	static QRInfo qrEncodeWif(const std::string& wif);

	static QRInfo writeEPS(QRcode *qrcode);
	
	int width;
	std::vector<bool> values;	
};

















#endif






