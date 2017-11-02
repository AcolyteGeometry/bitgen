/**
 * FileUtil.cpp
 *
 * Copyright (C)2017 The bitgen developers <bitgen@openmailbox.org>
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
 
#include "FileUtil.h"

#include <string>

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <fstream>
#include <vector>


bool fileWrite(const std::string& filename, const std::string& str)
{
	std::ofstream file(filename.c_str(), std::ios::binary);

	file.write(str.c_str(), str.size());
	if(file.bad())
	{
		std::cout << "FILE WRITE FAILED: " << filename << std::endl;
		throw std::string("File write failed");
	}
	
	return true;
}

std::string readFile(const std::string& name)
{
	std::ifstream stream(name.c_str(), std::ios::binary);
	if(!stream)
	{
		std::string errMsg("Could not open file: ");
		errMsg += name;
		throw errMsg;
	}
	
	stream.seekg(0, std::ios::end);
	const int size = stream.tellg();
	stream.seekg(0, std::ios::beg);
	std::vector<char> buf(size);
	stream.read(&buf[0], size);
	
	const std::string content(&buf[0], size);
	return content;	
}




void deleteFile(const std::string& filename)
{
	const int ret = unlink(filename.c_str());
	if(ret != 0)
	{
		std::cout << "Could not delete: " << filename << std::endl;
		throw std::string("Could not delete file");
	}
}


bool directoryExist(const std::string& folderName)
{
	struct stat st;	
	if(stat(folderName.c_str(), &st) != 0)
	{
		return false;
	}
	
	const bool isdir = S_ISDIR(st.st_mode);
	return isdir;
}




bool fileExist(const std::string& fileName)
{
	struct stat sta;	
	if(stat(fileName.c_str(), &sta) != 0)
	{
		return false;
	}
	
	const bool defaultExist = S_ISREG(sta.st_mode);	
	return defaultExist;
}



std::string removeEnding(const std::string& str, const std::string& end)
{
	const int endSize = end.size();
	const std::string actualEnding = str.substr(str.size() - endSize);
	//std::cout << "Actual ending: " << actualEnding << std::endl;
	if(actualEnding != end)
	{
		std::cout << "Incorrect end: " << actualEnding << std::endl;
		std::cout << "String: " << str << std::endl;
		
		std::string errMsg("Incorrect ending: ");
		errMsg += end;
		throw errMsg;
	}
	
	const std::string beginStr = str.substr(0, str.size() - endSize);
	return beginStr;
}




void createDirectory(const std::string& folderName)
{
	std::cout << "Creating folder: " << folderName << std::endl;
#ifdef __linux	
	const int ret = mkdir(folderName.c_str(), S_IRWXU | S_IRWXG);
#else
	throw std::string("Windows function not implemented");
#endif
}




