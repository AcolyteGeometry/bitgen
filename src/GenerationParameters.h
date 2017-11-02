#ifndef GENERATION_PARAMETERS_H
#define GENERATION_PARAMETERS_H

/**
 * GenerationParameters.h - Bitcoin poscript generator
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
#include <vector>


class GenerationParameters
{
public:	
	GenerationParameters() : 
		getName(false),
		compressed(true),
		generatePS(true),
		generateTXT(true),	
		useFolder(false),
		currencyspecific(false),
		getPasswordParam(false),
		generateExtended(true),
		printExtendedLegacy(false)
		{}

	GenerationParameters(const bool inCompressed, const bool genPS, const bool genTXT) :
		getName(false),
		compressed(inCompressed),
		generatePS(genPS),
		generateTXT(genTXT),	
		useFolder(false),
		currencyspecific(false),
		getPasswordParam(false),
		generateExtended(false)
		{}

	void setAccountName(const std::string& inAccountName)
	{
		accountName = inAccountName;
	}


	bool useArg(const std::string& arg);
	bool useCurrencySpecific(const std::string& arg);		
	void finally() const;
	int setAccountNameforFolder();
	void parseArgs(const std::vector<std::string>& allExtraArgs);


	bool getCompressed() const { return compressed; }
	bool getGeneratePS() const { return generatePS; }
	bool getGenerateTXT() const { return generateTXT; }
	bool getUseFolder() const { return useFolder; }
	std::string getAccountName() const { return accountName; }
	bool getCurrencySpecific() const { return currencyspecific; }
	std::string getPassword() const { return password; }
	bool getGenerateExtended() const { return generateExtended; }
	bool getPrintExtendedLegacy() const { return printExtendedLegacy; }


private:
	bool getName;			//Used internally
	bool getPasswordParam;	//Used internally


	bool compressed;
	bool generatePS;
	bool generateTXT;	
	bool useFolder;
	std::string accountName;
	bool currencyspecific;
	std::string password;
	bool generateExtended;
	bool printExtendedLegacy;
	
};

#endif


