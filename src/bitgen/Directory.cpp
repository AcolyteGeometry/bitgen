/**
 * Directory.cpp
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
 
#include "Directory.h"

#include <sys/types.h>
#include <dirent.h>

#include <iostream>

Directory::Directory()
{
	openDir(".");
}

Directory::Directory(const std::string& folderName)
{
	openDir(folderName);
}



void Directory::openDir(const std::string& folderName)
{
	DIR* dir = opendir(folderName.c_str());
	if(dir == 0)
	{
		throw std::string("Could not open dir");		
	}
	
	for(;;)
	{
		const struct dirent* dirEntry = readdir(dir);
		if(dirEntry == 0)
		{
			break;
		}
		
		const std::string filename(dirEntry->d_name);
		if(filename == "." || filename == "..")
		{
			continue;
		}
		
		//std::cout << "Adding: " << filename << std::endl;
		entries.insert(filename);		
	}
}



bool Directory::hasItem(const std::string& item) const
{
	return (entries.find(item) != entries.end());
}
