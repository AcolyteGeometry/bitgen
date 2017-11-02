/**
 * bitcoin.cpp - Bitcoin address generator
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
 
/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
*/
 


#include "SaveFolderHandler.h"

#include "currencySpecific.h"
#include "FileUtil.h"
#include "Directory.h"
#include "util.h"
#include <stdlib.h>



int SaveFolderHandler::getNextNumberAndCreateBaseFolders()
{
	const std::string rootFolder("cold");
	const std::string pubFold("pub");
	const std::string privFold("priv");

	const std::string pubFolder(rootFolder + "/" + pubFold);
	const std::string privFolder(rootFolder + "/" + privFold);
	const std::string currencyShortName = getCurrencyShortName();	
	const std::string pubCoinFolder = pubFolder + "/" + currencyShortName;
	const std::string privCoinFolder = privFolder + "/" + currencyShortName;
	const std::string pubFolderPrefix("pub_" + currencyShortName + "_");
	const std::string privFolderPrefix("priv_" + currencyShortName + "_");


	if(!directoryExist(rootFolder))
	{
		createDirectory(rootFolder);
	}

		
	if(!directoryExist(pubFolder))
	{
		createDirectory(pubFolder);
	}

	if(!directoryExist(privFolder))
	{
		createDirectory(privFolder);
	}
		
		
		
	if(!directoryExist(pubCoinFolder))
	{
		createDirectory(pubCoinFolder);
	}
		
	if(!directoryExist(privCoinFolder))
	{
		createDirectory(privCoinFolder);
	}
		
	//Which is the last folder stored?
	Directory currentDirectory(rootFolder);
	if(!currentDirectory.hasItem(pubFold))
	{
		throw std::string("Pub folder DOES NOT exist");			
	}

	if(!currentDirectory.hasItem(privFold))
	{
		throw std::string("Priv folder DOES NOT exist");			
	}

	Directory pubDirectory(pubFolder);		
	if(!pubDirectory.hasItem(currencyShortName))
	{
		throw std::string("Pub coin folder DOES NOT exist");						
	}

	Directory privDirectory(privFolder);		
	if(!privDirectory.hasItem(currencyShortName))
	{
		throw std::string("Priv coin folder DOES NOT exist");						
	}
		
		
	int largestUsed(0);
		
		
	std::cout << "CHECKING: " << pubCoinFolder << std::endl;
	Directory pubCoinDirectory(pubCoinFolder);
	const std::set<std::string>& pubEntries = pubCoinDirectory.getEntries();
	for(std::set<std::string>::const_iterator it = pubEntries.begin() ;
		it != pubEntries.end() ;
		++it)
	{
		const std::string& entryName = (*it);
		std::cout << "Checking: " << entryName << std::endl;
		if(startsWith(entryName, pubFolderPrefix))
		{
			const std::string numStr = removePrefix(entryName, pubFolderPrefix);
			const int num = atoi(numStr.c_str());
			if(num == 0)
			{
				throw std::string("Item #0 not allowed");
			}
				
			if(num > largestUsed)
			{
				largestUsed = num;
			}
		}
	}


	Directory privCoinDirectory(privCoinFolder);
	const std::set<std::string>& privEntries = privCoinDirectory.getEntries();
	for(std::set<std::string>::const_iterator it = privEntries.begin() ;
		it != privEntries.end() ;
		++it)
	{
		const std::string& entryName = (*it);
		std::cout << "Checking: " << entryName << std::endl;
		if(startsWith(entryName, privFolderPrefix))
		{
			const std::string numStr = removePrefix(entryName, privFolderPrefix);
			const int num = atoi(numStr.c_str());
			if(num == 0)
			{
				throw std::string("Item #0 not allowed");
			}
				
			if(num > largestUsed)
			{
				largestUsed = num;
			}
		}
	}

		
	const int newNum = largestUsed + 1;
	return newNum;
}
	
	
	

std::pair<std::string, std::string> SaveFolderHandler::createNewFolders(const int newNum)
{
	const std::string rootFolder("cold");
	
	const std::string pubFold("pub");
	const std::string privFold("priv");
	
	const std::string pubFolder(rootFolder + "/" + pubFold);
	const std::string privFolder(rootFolder + "/" + privFold);
	
	const std::string currencyShortName = getCurrencyShortName();		
	const std::string pubCoinFolder = pubFolder + "/" + currencyShortName;
	const std::string privCoinFolder = privFolder + "/" + currencyShortName;
	const std::string pubFolderPrefix("pub_" + currencyShortName + "_");
	const std::string privFolderPrefix("priv_" + currencyShortName + "_");
	
	
	const std::string pubFolderMiddle = "/" + pubFolderPrefix;
	const std::string privFolderMiddle = "/" + privFolderPrefix;
	
	const std::string newPubFolder = pubCoinFolder + pubFolderMiddle + intToString(newNum);
	const std::string newPrivfolder = privCoinFolder + privFolderMiddle + intToString(newNum);
	
	//std::cout << "Creating the folder: " << newPubFolder << std::endl;
	createDirectory(newPubFolder);
	createDirectory(newPrivfolder);
		
	std::pair<std::string, std::string>	folders(newPubFolder, newPrivfolder);
	return folders;
}


