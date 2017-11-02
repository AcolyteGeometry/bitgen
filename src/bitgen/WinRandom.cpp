/**
 * WinRandom.cpp - Bitcoin poscript generator
 *
 * Copyright (C)2016 The bitgen developers <bitgen@openmailbox.org>
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
 
#ifndef __linux 
#include <windows.h>
#include <wincrypt.h>


#include "WinRandom.h"
#include <string>


WinRandom::WinRandom() : hProvider(0)
{
	if(CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		return;
    }
    
    if(GetLastError() == NTE_BAD_KEYSET)
	{
		if(CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) 
		{
			return;
		}
	}
    
    if(!hProvider) 
    {
		throw std::string("Error, no Windows crypt providder");
	}    
	
	throw std::string("Error for CryptAcquireContext");		
}



WinRandom::~WinRandom()
{
	if(hProvider) 
	{
		CryptReleaseContext(hProvider, 0U);
	}
}



std::string WinRandom::generate(int len)
{	
	if(len > 128)
	{
		throw std::string("At most 128 random bytes possible");
	}

	char buf[128];
				
    if(!CryptGenRandom(hProvider, (DWORD)len, (BYTE*)buf))
    {
		throw std::string("CryptGenRandom error");		
	}
    
	const std::string ret(buf, len);
    
    return ret;
}
 
 
#endif






