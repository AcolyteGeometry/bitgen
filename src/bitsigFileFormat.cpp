
#include "bitsigFileFormat.h"
#include "FileUtil.h"


bool operator<(const PubKeyAlias& a, const PubKeyAlias& b)
{
	return (a.data < b.data);
}




PubKeyAlias parsePubAddr(const std::string& str)
{
	const std::string pubBegin("----- BEGIN IMPORTED PUBLIC BITCOIN ADDRESS -----\n");
	
	const int pubStart = str.find(pubBegin);
	
	if(pubStart == std::string::npos)
	{
		throw std::string("Could not parse pubfile");
	}
	
	const int infoStart = pubStart + pubBegin.size();
	
	const std::string pubEnd("\n----- END IMPORTED PUBLIC BITCOIN ADDRESS -----");
	const int infoEnd = str.find(pubEnd);
	
	const std::string info = str.substr(infoStart, infoEnd - infoStart);
	
	const int delimPos = info.find("\n");
	const std::string pubAddress = info.substr(0, delimPos);
	//std::cout << "Pub addr: " << pubAddress << std::endl;
	
	const int aliasPos = delimPos + 1;
	const std::string alias = info.substr(aliasPos);
	//std::cout << "Alias: " << alias << std::endl;
	
	
	PubKeyAlias palias(PubAddress(pubAddress), alias);
	return palias;
}


PubKeyAlias parsePubKey(const std::string& str)
{
	const std::string pubBegin("----- BEGIN IMPORTED PUBLIC KEY BITCOIN ADDRESS -----\n");
	
	const int pubStart = str.find(pubBegin);
	
	if(pubStart == std::string::npos)
	{
		throw std::string("Could not parse pubfile");
	}
	
	const int infoStart = pubStart + pubBegin.size();
	
	const std::string pubEnd("\n----- END IMPORTED PUBLIC KEY BITCOIN ADDRESS -----");
	const int infoEnd = str.find(pubEnd);
	
	const std::string info = str.substr(infoStart, infoEnd - infoStart);
	
	const int delimPos = info.find("\n");
	const std::string pubAddress = info.substr(0, delimPos);
	//std::cout << "Pub addr: " << pubAddress << std::endl;
	
	const int aliasPos = delimPos + 1;
	const std::string alias = info.substr(aliasPos);
	//std::cout << "Alias: " << alias << std::endl;
	
	
	//const Pkif pkif = Pkif::parsePkif(pubAddress);
	
	PubKeyAlias palias(pubAddress /*pkif*/, alias, false);
	return palias;
}




PubKeyAlias parseImportedPublic(const std::string& pubfile)
{
	const std::string str = readFile(pubfile);
	
	//Does the file contain an address or a public key?
	const std::string pubBegin("----- BEGIN IMPORTED PUBLIC BITCOIN ADDRESS -----\n");
	const std::string keyBegin("----- BEGIN IMPORTED PUBLIC KEY BITCOIN ADDRESS -----\n");
	
	const int astartPos = str.find(pubBegin);
	if(astartPos != std::string::npos)
	{
		//std::cout << "PUB ADDRESS" << std::endl;
		return parsePubAddr(str);
	}
	else
	{
		const int kstartPos = str.find(keyBegin);
		if(kstartPos != std::string::npos)
		{
			//std::cout << "PUB KEY" << std::endl;			
			return parsePubKey(str);			
		}
		else
		{
			throw std::string("Error in pubfile");
		}
	}	


	//PubKeyAlias dummy;
	//return dummy;
}



std::string generateKeyStoreFile(const BitcoinKeyPair& keypair)
{
	std::string ret;
	ret += "----- BEGIN BITCOIN SECP256K1 PRIVKEY -----\n";
	ret += keypair.privaddr.toString();
	ret += "\n";
	ret += "----- END BITCOIN SECP256K1 PRIVKEY -----\n";
	
	return ret;
}



std::string parseWifFromPrivFile(const std::string& str)
{
	const std::string beginStr("----- BEGIN BITCOIN SECP256K1 PRIVKEY -----");
	const int begPos = str.find(beginStr);
	
	if(begPos == std::string::npos)
	{
		std::cout << "NOTE: DATABASE FORMAT HAS CHANGED. " << std::endl;
		std::cout << "NOTE: You might need to rename the .bitsig folder, " << std::endl;
		std::cout << "NOTE: and then import all keys with this application version" << std::endl;
		
		throw std::string("File format error for private key begin");
	}
	
	const int wifBegPos = begPos + beginStr.size();
	
	
	const std::string endStr("----- END BITCOIN SECP256K1 PRIVKEY -----");
	const int endPos = str.find(endStr, wifBegPos);

	
	if(endPos == std::string::npos)
	{
		throw std::string("File format error for private key end");
	}

	const std::string content = str.substr(wifBegPos, endPos - wifBegPos);
	
	std::string ret;
	for(int i = 0 ; i < content.size() ; i++)
	{
		const char c = content[i];
		if(isBase58Char(c))
		{
			ret += c;
		}
	}

	//std::cout << "Just the wif: " << ret << std::endl;
	
	return ret;
}



