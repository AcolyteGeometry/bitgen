/**
 * qrencode - QR Code encoder
 *
 * QR Code encoding tool
 * Copyright (C) 2006-2013 Kentaro Fukuchi <kentaro@fukuchi.org>
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


#include "qrenc.h"
#include "qrencode.h"
#include "util.h"
#include "FileUtil.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

//Todo: Rename?
QRInfo QRInfo::writeEPS(QRcode *qrcode)
{	
	
	const unsigned char* p = qrcode->data;
	const int width = qrcode->width;
	QRInfo qrinfo(width);
	
	
	for(int y=0; y<qrcode->width; y++) 
	{			
		for(int x=0; x < width ; x++) 
		{	
			const unsigned char* pp = p + y * width + x;
			const bool val = (*pp) & 0x1;
			qrinfo.add(val);			
		}
	}


	return qrinfo;
}




QRInfo QRInfo::qrEncodeWif(const std::string& wif)
{
	const QRInfo privinfo = QRInfo::qrencode((const unsigned char*)wif.c_str(), wif.size());
	return privinfo;
}



QRInfo QRInfo::qrencode(const unsigned char *intext, int length)
{
	static int version = 0;

	static QRecLevel level = QR_ECLEVEL_L;

	
	QRcode* qrcode = QRcode_encodeData(length, intext, version, level);
		
	if(qrcode == NULL) {
		perror("Failed to encode the input data");
		exit(EXIT_FAILURE);
	}

	QRInfo info = writeEPS(qrcode);

	QRcode_free(qrcode);
	
	return info;
}



	
	
