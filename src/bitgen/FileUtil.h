#ifndef FILE_UTIL_H
#define FILE_UTIL_H

/**
 * util.h - Bitcoin poscript generator
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

bool fileWrite(const std::string& filename, const std::string& str);
std::string readFile(const std::string& name);
void deleteFile(const std::string& filename);
bool directoryExist(const std::string& folderName);
bool fileExist(const std::string& fileName);
void createDirectory(const std::string& folderName);

std::string removeEnding(const std::string& str, const std::string& end);


#endif
