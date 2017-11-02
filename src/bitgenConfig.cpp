/**
 * bitgenConfig.cpp - Bitcoin generator
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

#include "bitgenConfig.h"
#include "util.h"
#include "FileUtil.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef __linux
#include <pwd.h>
#else
#include <shlobj.h>
#endif

std::string getConfigPath()
{
#ifdef __linux	
	struct passwd *pw = getpwuid(getuid());
	const char* homedir = pw->pw_dir;
	const std::string homeDir(homedir);
#else
	char path[MAX_PATH];
	HRESULT res = SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path);
	if(res != S_OK)
	{
		std::cerr << "Can not get home folder" << std::endl;
		throw std::string("Error, can not get home folder");
	}
	
	const std::string homeDir(path);	
#endif
	
	const std::string configDir = homeDir + "/.bitgen";
	
	return configDir;
}


bool configDirExist()
{
	const std::string configPath = getConfigPath();	
	
	return directoryExist(configPath);
}



std::string getOrCreateTheConfigDir()
{
	std::string confPath = getConfigPath();
	if(directoryExist(confPath))
	{
		return confPath;
	}
	
	
	//Create the dir
	std::cout << "Creating folder: " << confPath << std::endl;
#ifdef __linux	
	const int ret = mkdir(confPath.c_str(), S_IRWXU | S_IRWXG);
#else
	const int ret = mkdir(confPath.c_str());
#endif
	if(ret != 0)
	{
		std::cout << "Could not create: " << confPath << std::endl;
		throw std::string("Could not create work folder");
	}
	
	return confPath;	
}


void configureLang(const std::string& lang)
{
	const std::string confDir = getOrCreateTheConfigDir();
	const std::string confFile = confDir + "/language.conf";
	
	fileWrite(confFile, lang);
	std::cout << "Writing " << confFile << std::endl;
}


std::string getConfiguredLang()
{
	const std::string confPath = getConfigPath();
	const std::string confFile = confPath + "/language.conf";

	//std::cout << "Checking if file exist: " << confFile << std::endl;
	if(!fileExist(confFile))
	{
		//std::cout << "No lang configured" << std::endl;
		return "";
	}
	
	const std::string confLang = readFile(confFile);
	
	//std::cout << "Using lang: " << confLang << std::endl;
	return confLang;
}




