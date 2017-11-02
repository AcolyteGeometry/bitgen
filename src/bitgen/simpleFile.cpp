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
#include <vector>

#include "simpleFile.h"

#ifdef __linux
SimpleFile::SimpleFile(const std::string& filename) : 
		fileHandle(open(filename.c_str(), O_RDONLY))
{
	if(fileHandle == -1)
	{
		std::string errMsg("Error, could not open: ");
		errMsg += filename;
		throw errMsg;
	}
}



SimpleFile::~SimpleFile()
{
	close(fileHandle);
}


unsigned char SimpleFile::readOneChar()
{
	unsigned char buf;
	const int retval = read(fileHandle, &buf, 1);
	if(retval == -1)
	{
		throw std::string("Could not read file");			
	}
	
	return buf;
}


int SimpleFile::fileSize()
{
	struct stat buf;
	int ret = fstat(fileHandle, &buf);
	if(ret != 0)
	{
		throw std::string("Error, could not get file size");
	}

	const int size = buf.st_size;
	return size;
}


std::string SimpleFile::getChunk()
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


std::string SimpleFile::getStr(const int maxSize)
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

#else

SimpleFile::SimpleFile(const std::string& filename) : 
		stream(filename.c_str(), std::ios::binary), m_fileSize(0)

{
	if(!stream)
	{
		std::string errMsg("Error, could not open: ");
		errMsg += filename;
		throw errMsg;
	}
	
	stream.seekg(0, std::ios::end);
	m_fileSize = stream.tellg();
	stream.seekg(0, std::ios::beg);
}



SimpleFile::~SimpleFile()
{
}


unsigned char SimpleFile::readOneChar()
{
	std::vector<char> buf(1);	
	stream.read(&buf[0], 1);
	
	if(stream.bad())
	{
		std::string errMsg("Error, could not read file c");
		throw errMsg;
	}
	
	return buf[0];
}


int SimpleFile::fileSize()
{
	return m_fileSize;	
}


std::string SimpleFile::getChunk()
{
	std::vector<char> buf(1024);	
	
	stream.read(&buf[0], 1024);
	if(stream.bad())
	{
		std::string errMsg("Error, could not read file b");
		throw errMsg;
	}
	
	int readSize = stream.gcount();
		
	const std::string ret((const char*)(&buf[0]), readSize);
	return ret;	
}


std::string SimpleFile::getStr(const int maxSize)
{
	const int myMaxSize = ((maxSize > 1024) ? 1024 : maxSize);

	char buf[1024];	
	stream.read(&buf[0], myMaxSize);
	if(stream.bad())
	{
		std::string errMsg("Error, could not read file a");
		throw errMsg;
	}
	
	int readSize = stream.gcount();
	
	const std::string ret((const char*)(&buf[0]), readSize);
	return ret;	
	
}




#endif
