/**
 * SimpleFile.cpp - Bitcoin poscript generator
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


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "nonblockFile.h"

#include <iostream>
#include <string.h>

NonblockFile::NonblockFile() : fileHandle(0)
{
}


NonblockFile::NonblockFile(const std::string& filename) : 
		fileHandle(open(filename.c_str(), O_RDONLY))
{
#ifdef __linux	
	if(fileHandle == -1)
	{
		std::string errMsg("Error, could not open: ");
		errMsg += filename;
		throw errMsg;
	}
	
	const int saved_flags = fcntl(fileHandle, F_GETFL);
	fcntl(fileHandle, F_SETFL, saved_flags | O_NONBLOCK);	
#else
	throw std::string("NonblockFile not implemented");
#endif
}



NonblockFile::~NonblockFile()
{
	close(fileHandle);
}


unsigned char NonblockFile::readOneChar(bool& gotChar)
{
	unsigned char buf(0x55);
	std::cout << "Will read" << std::endl;
	const int retval = read(fileHandle, &buf, 1);
	std::cout << "After read" << std::endl;
	
	std::cout << "retval: " << retval << std::endl;
	if(retval == -1)
	{
		std::cout << "errno: " << errno << std::endl;
		std::cout << "errstr: " << strerror(errno) << std::endl;
		
		if(errno == EAGAIN)
		{
			gotChar = false;
			return 0x66;
		}
		
		throw std::string("Could not read file");			
	}
	else if(retval == 0)
	{
		std::cout << "Got zero bytes" << std::endl;
		gotChar = false;
		return 0x77;		
	}
	
	gotChar = true;
	return buf;
}


/*
int NonblockFile::fileSize()
{
	struct stat buf;
	int ret = fstat(fileHandle, &buf);
	if(ret != 0)
	{
		throw std::string("Error, could not get file size");
	}

	const int size = buf.st_size;
}


std::string NonblockFile::getChunk()
{
	char buf[1024];	
	int readSize = read(fileHandle, buf, 1024);
	if(readSize == -1)
	{
		throw std::string("File read error");
	}
	
	const std::string ret((const char*)buf, readSize);
	return ret;	
}


std::string NonblockFile::getStr(const int maxSize)
{
	const int myMaxSize = ((maxSize > 1024) ? 1024 : maxSize);
	
	char buf[1024];	
	int readSize = read(fileHandle, buf, myMaxSize);
	if(readSize == -1)
	{
		throw std::string("File read error");
	}
	
	const std::string ret((const char*)buf, readSize);
	return ret;	
	
}
*/
