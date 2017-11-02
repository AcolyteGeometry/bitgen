
#include "bitsigConfig.h"
#include "FileUtil.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef __linux
#include <pwd.h>
#else
#include <shlobj.h>
#endif


#include "util.h"

Pkif getPublicKeyForName(const std::string& alias)
{
	const std::set<PubKeyAlias> allAlias = BitsigConfig::getAllStoredPub();
	for(std::set<PubKeyAlias>::const_iterator it = allAlias.begin() ;
		it != allAlias.end() ;
		++it)
	{
		const PubKeyAlias& pka = (*it);
		const std::string& thisAlias = pka.alias;	
		if(thisAlias == alias)
		{
			if(pka.isAddr())
			{
				std::cout << "Found name: " << alias << ", but have no public key" << std::endl;
				throw std::string("Error, is an address, not a key");
			}
			
			std::cout << "Found: " << pka.data << std::endl;
			
			const Pkif pkif(pka.data); // = Pkif::parsePkif(pka.data);
			return pkif;
		}
	}
	
	std::cout << "Could not find: " << alias << std::endl;
	throw std::string("Could not find given name");
}





BigInt<256> getPrivKeyForSigning(bool& compressed)
{
	//std::cout << "No private key given" << std::endl;
	const BigInt<256> privKey = BitsigConfig::getDefaultPrivkey(compressed);
	//std::cout << "1 The Private Key is " << (compressed ? "compressed" : "noncompressed") << std::endl;
			
	//const ECPoint publicKeyPoint = getPublicPointFromPrivateKey(privKey);
	const PrivKey privateKey(privKey, compressed);
	BitcoinEllipticCurve ec;		
	const ECPoint publicKeyPoint = privateKey.point(ec);
	
	const PubKey p(publicKeyPoint, compressed);
	const PubAddress pub = p.getAddress();	
	//const PubAddress pub = PubAddress::bitcoinAddresFromPubkey(publicKeyPoint, compressed);
			
	//const PubAddress pub = PubAddress::getPublicFromPrivateBinary(compressed, privKey);

	std::cout << "No key specified, using the default key: " << pub << std::endl;			
	//std::cout << "2 The Private Key is " << (compressed ? "compressed" : "noncompressed") << std::endl;
	return privKey;		
}



int BitsigConfig::numPublicKeys()
{
	const std::set<PubKeyAlias> pubAddress = getAllStoredPub();
	return pubAddress.size();
}


bool BitsigConfig::hasPublic(const PubAddress& myPubAddr)
{
	const std::set<PubKeyAlias> allStoredPub = getAllStoredPub();
	for(std::set<PubKeyAlias>::const_iterator it = allStoredPub.begin() ;
		it != allStoredPub.end() ;
		++it)
	{
		const PubKeyAlias pka = (*it);
		const PubAddress pubAddr = pka.getPubAddr();
		
		if(pubAddr == myPubAddr)
		{
			return true;
		}
	}
	
	return false;
}



void BitsigConfig::deleteDefault()
{
	const std::string configDir = getConfigDir();	

	
	const std::string defaultPriv = configDir + "/default_key.defpriv";
	
	if(unlink(defaultPriv.c_str()) == 0)
	{
		std::cout << "Deleted default indication" << std::endl;
	}	
}








void BitsigConfig::deletePublic(const PubAddress& myPubAddr)
{
	if(!hasPublic(myPubAddr))
	{
		throw std::string("Error, does not have this public address");
	}


	const std::string configDir = getConfigDir();	
	const std::string keyStoreFileName = configDir + "/" + myPubAddr.toString() /*.address*/ + ".bitpub";			

	std::cout << "Will delete: " << keyStoreFileName << std::endl;

	if(unlink(keyStoreFileName.c_str()) != 0)
	{
		std::cout << "Could not delete file" << std::endl;
		return;
	}
	
	//std::cout << "Deleting default pub not yet implemented" << std::endl;
	const std::set<PubKeyAlias> allRemainingPub = getAllStoredPub();
	if(allRemainingPub.empty())
	{
		std::cout << "No public keys remain" << std::endl;
		//Delete the def key file if it exist
		//deleteDefaultPublic();
		return;		
	}	
}











void BitsigConfig::deletePrivate(const PubAddress& myPubAddr)
{
	if(!hasPrivateForPublic(myPubAddr))
	{
		throw std::string("Error, does not have this address");
	}


	const std::string configDir = getConfigDir();	
	const std::string keyStoreFileName = configDir + "/" + myPubAddr.toString() /*.address*/ + ".bitpriv";			

	if(unlink(keyStoreFileName.c_str()) != 0)
	{
		std::cout << "Could not delete file" << std::endl;
		return;
	}
	
	
	std::string defPrivContent;
	const std::set<BitcoinKeyPair> ownPairs = getOwnKeypairs(defPrivContent);
	if(ownPairs.empty())
	{
		std::cout << "This was the last address, no private addresses exist" << std::endl;
		
		//Delete the def key file if it exist
		deleteDefault();
		return;
	}
	
	//Was this the default?
	const PubAddress defaultPubAddr = getPubForDefaultPriv();
	if(defaultPubAddr != myPubAddr)
	{
		return;
	}

	//Change the default address to a random existing
	const BitcoinKeyPair randomPair = *ownPairs.begin();


	const PubAddress pubPart = randomPair.pubaddr;
	
	std::cout << "This was the default address, setting to:" << pubPart << std::endl;
	
	setDefaultKey(pubPart);	
}







bool BitsigConfig::hasPrivateForPublic(const PubAddress& myPubAddr)
{
	//Does this private key exist?
	std::string defPrivContent;
	const std::set<BitcoinKeyPair> ownPairs = BitsigConfig::getOwnKeypairs(defPrivContent);
	
	for(std::set<BitcoinKeyPair>::const_iterator it = ownPairs.begin() ;
		it != ownPairs.end() ;
		++it)
	{
		const BitcoinKeyPair& myPair = (*it);
		if(myPair.pubaddr == myPubAddr)
		{
			return true;
		}
	}
	
	return false;
}


int BitsigConfig::numOwnKeypairs()
{
	std::string defPrivContent;
	const std::set<BitcoinKeyPair> ownPairs = getOwnKeypairs(defPrivContent);

	return ownPairs.size();
}





std::string BitsigConfig::getConfigDir()
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
	const std::string configDir = homeDir + "/.bitsig";
	
	return configDir;
}



void BitsigConfig::createConfigDir(const std::string& configDir)
{
	std::cout << "Config folder does not exist, creating..." << std::endl;
#ifdef __linux
	const int ret = mkdir(configDir.c_str(), S_IRWXU /*| S_IRWXG | S_IRWXO*/);
#else
	const int ret = mkdir(configDir.c_str());
#endif
	if(ret != 0)
	{
		std::cout << "Could not create: " << configDir << std::endl;
		throw std::string("Could not create work folder");
	}
}








std::string BitsigConfig::getOrCreateConfigDir()
{
	const std::string configDir = getConfigDir();	
	
	if(!directoryExist(configDir))
	{
		//Create the dir
		std::cout << "Creating folder: " << configDir << std::endl;
#ifdef __linux
		const int ret = mkdir(configDir.c_str(), S_IRWXU | S_IRWXG /*| S_IRWXO*/);
#else
		const int ret = mkdir(configDir.c_str());
#endif
		if(ret != 0)
		{
			std::cout << "Could not create: " << configDir << std::endl;
			throw std::string("Could not create work folder");
		}
	}
	
	return configDir;
}









std::set<PubKeyAlias> BitsigConfig::getAllStoredPub()
{
	std::set<PubKeyAlias> ret;
	
	const std::string configDir = getOrCreateConfigDir();
	
	//Todo: Use class Directory
	DIR* dir = opendir(configDir.c_str());
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
		
		if(endsWith(filename, ".bitpub"))
		{
			//std::cout << "Imported public key: " << filename << std::endl;
			const std::string fullPath = configDir + "/" + filename;
			const PubKeyAlias keyAndAlias = parseImportedPublic(fullPath);
			ret.insert(keyAndAlias);
			//std::cout << keyAndAlias.pubkey << " " << keyAndAlias.alias << std::endl;
		}
	}

	//Todo: Close dir!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	return ret;
}






void BitsigConfig::listPublicKeys()
{		
	std::set<PubKeyAlias> allpub = getAllStoredPub();
	if(allpub.empty())
	{
		std::cout << "No public keys exist" << std::endl;
	}
	
	//std::cout << "Will print public addresses" << std::endl;
	
	int i(1);
	for(std::set<PubKeyAlias>::const_iterator it = allpub.begin() ;
		it != allpub.end() ;
		++it)
	{
		const PubKeyAlias ka = (*it);
		
		const bool isAddress = ka.isAddr();
		if(isAddress)
		{
			continue;
		}
		
		const Pkif pkif = ka.getPkif();
		
		//const PubAddress pubAddr = ka.getPubAddr();
		
		std::cout << i << " " << pkif.toString() << " ";
		std::cout << " " << ka.alias << std::endl;
		i++;
	}	
}




void BitsigConfig::listPublicAddresses(const bool listOnlyAddresses)
{		
	std::set<PubKeyAlias> allpub = getAllStoredPub();
	if(allpub.empty())
	{
		std::cout << "No public keys exist" << std::endl;
	}
	
	//std::cout << "Will print public addresses" << std::endl;
	
	int i(1);
	for(std::set<PubKeyAlias>::const_iterator it = allpub.begin() ;
		it != allpub.end() ;
		++it)
	{
		const PubKeyAlias ka = (*it);
		const bool isAddress = ka.isAddr();
		
		if(listOnlyAddresses)
		{
			std::cout << (isAddress ? " " : "*");
			std::cout << " " << ka.alias << std::endl;	
		}
		else
		{
			const PubAddress pubAddr = ka.getPubAddr();
			std::cout << i << " " << minSize(pubAddr.toString() /*.address*/, 34) << " ";
			std::cout << (isAddress ? " " : "*");
			std::cout << " " << ka.alias << std::endl;
		}
		i++;
	}	
}






std::set<Wif> BitsigConfig::getOwnPrivate(std::string& defPrivContent)
{
	const std::string configDir = getOrCreateConfigDir();

	DIR* dir = opendir(configDir.c_str());
	if(dir == 0)
	{
		throw std::string("Could not open dir");		
	}
	
	
	const std::string defaultPriv = configDir + "/default_key.defpriv";
	
	
	if(fileExist(defaultPriv))
	{
		defPrivContent = readFile(defaultPriv);
	}
	
	std::set<Wif> privateKeys;
	for( ; ; )
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
		
		const std::string bitpriv(".bitpriv");
		
		if(endsWith(filename, bitpriv))
		{
			const std::string pubkey = filename.substr(0, filename.size() - bitpriv.size());
			
			const std::string privfile = configDir + "/" + filename;
			const std::string privFileContent = readFile(privfile);
			const Wif wif = Wif(parseWifFromPrivFile(privFileContent));
			
			//Check checksum
			//bool compressed(false);
			//const BigInt<256> not_used_PrivKey = wif.getPrivKey(compressed);			
			
			//const BitcoinKeyPair keypair(PubAddress(pubkey), wif);
			privateKeys.insert(wif);
		}
	}
	
	return privateKeys;
}














std::set<BitcoinKeyPair> BitsigConfig::getOwnKeypairs(std::string& defPrivContent)
{	
	const std::string configDir = getOrCreateConfigDir();

	DIR* dir = opendir(configDir.c_str());
	if(dir == 0)
	{
		throw std::string("Could not open dir");		
	}
	
	
	const std::string defaultPriv = configDir + "/default_key.defpriv";
	
	
	if(fileExist(defaultPriv))
	{
		defPrivContent = readFile(defaultPriv);
	}
	
	std::set<BitcoinKeyPair> keyPairs;
	for( ; ; )
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
		
		const std::string bitpriv(".bitpriv");
		
		if(endsWith(filename, bitpriv))
		{
			const std::string pubkey = filename.substr(0, filename.size() - bitpriv.size());
			
			const std::string privfile = configDir + "/" + filename;
			const std::string privFileContent = readFile(privfile);
			const Wif wif = Wif(parseWifFromPrivFile(privFileContent));
			
			
			//Check checksum
			const PrivKey nonUsed = wif.getPriv();
			//bool compressed(false);
			//const BigInt<256> not_used_PrivKey = wif.getPrivKey(compressed);			
			
			const BitcoinKeyPair keypair(PubAddress(pubkey), wif);
			keyPairs.insert(keypair);
		}
	}
	
	return keyPairs;
}



void BitsigConfig::listOwnPublicAddresses()
{
	std::string defPrivContent;
	std::set<BitcoinKeyPair> keyPairs = getOwnKeypairs(defPrivContent);

	if(keyPairs.empty())
	{
		std::cout << "No private keys exist" << std::endl;
	}

	int i = 1;
	for(std::set<BitcoinKeyPair>::const_iterator it = keyPairs.begin() ;
		it != keyPairs.end() ;
		++it)
	{
			const BitcoinKeyPair& keypair = (*it);
			std::cout << i << " " << keypair.pubaddr; 
			if(keypair.pubaddr.toString() /*.address*/ == defPrivContent)
			{
				std::cout << " def";
			}
			std::cout << std::endl;
			
			i++;
	}
	
}



void BitsigConfig::listOwnPublicKeys()
{
	std::string defPrivContent;
	std::set<BitcoinKeyPair> keyPairs = getOwnKeypairs(defPrivContent);

	BitcoinEllipticCurve ec;


	int i = 1;
	for(std::set<BitcoinKeyPair>::const_iterator it = keyPairs.begin() ;
		it != keyPairs.end() ;
		++it)
	{
			const BitcoinKeyPair& keypair = (*it);
			const Wif& wif = keypair.privaddr;
			
			const PrivKey privKey = wif.getPriv(); //PrivKey::fromWif(wif);
			
			const PubKey mypubKey = privKey.getPubkey(ec);
			//const Pkif mypkif = privKey.getPkif();
			const std::string pkif = mypubKey.toString();
			
			/*keypair.pubaddr << " -> "*/
			std::cout << i << " " <<  pkif;
			if(keypair.pubaddr.toString() /*.address*/ == defPrivContent)
			{
				std::cout << " def";
			}
			std::cout << std::endl;
			
			i++;
	}
	
	
}




void BitsigConfig::listPrivateAddresses()
{
	std::string defPrivContent;
	std::set<BitcoinKeyPair> keyPairs = getOwnKeypairs(defPrivContent);

	int i = 1;
	for(std::set<BitcoinKeyPair>::const_iterator it = keyPairs.begin() ;
		it != keyPairs.end() ;
		++it)
	{
			const BitcoinKeyPair& keypair = (*it);
			std::cout << i << " " << keypair.pubaddr << " -> " << keypair.privaddr;
			if(keypair.pubaddr.toString() /*.address*/ == defPrivContent)
			{
				std::cout << " def";
			}
			std::cout << std::endl;
			
			i++;
	}
	
	
}





void BitsigConfig::addPublicKey(const Pkif& pkif, const std::string& name)
{
	const PubKey pubKey = pkif.getPubKey();
	const PubAddress bitcoinAddress = pubKey.getAddress();
	
	const std::string configDir = getOrCreateConfigDir();	

	const std::string thisPubFile = configDir + "/" + bitcoinAddress.toString() /*.address*/ + ".bitpub";	
	
	std::string pubAddrContent;
	pubAddrContent += "----- BEGIN IMPORTED PUBLIC KEY BITCOIN ADDRESS -----\n";
	pubAddrContent += pkif.toString();
	pubAddrContent += "\n";	
	pubAddrContent += name;
	pubAddrContent += "\n";	
	pubAddrContent += "----- END IMPORTED PUBLIC KEY BITCOIN ADDRESS -----";
	
	std::cout << "Checking file: " << thisPubFile << std::endl;
	if(fileExist(thisPubFile))
	{
		//Delete the old file
		std::cout << "Address already present, replacing the entry" << std::endl;
		deleteFile(thisPubFile);
	}

	fileWrite(thisPubFile, pubAddrContent);	
}




void BitsigConfig::addPublicAddress(const PubAddress& addr, const std::string& name)
{
	const std::string configDir = getOrCreateConfigDir();	

	const std::string thisPubFile = configDir + "/" + addr.toString() /* .address */ + ".bitpub";	
	
	std::string pubAddrContent;
	pubAddrContent += "----- BEGIN IMPORTED PUBLIC BITCOIN ADDRESS -----\n";
	pubAddrContent += addr.toString(); //.address;
	pubAddrContent += "\n";	
	pubAddrContent += name;
	pubAddrContent += "\n";	
	pubAddrContent += "----- END IMPORTED PUBLIC BITCOIN ADDRESS -----";
	
	if(!fileExist(thisPubFile))
	{
		fileWrite(thisPubFile, pubAddrContent);
	}
	
	//const std::string defaultPub = readFile(defaultPubFile);
	
	//std::cout << "The default verify key: " << defaultPub << std::endl;
	
	//return defaultPub;
}




PubAddress BitsigConfig::getPubForDefaultPriv()
{		
	const std::string configDir = BitsigConfig::getConfigDir();
			
	struct stat st;	
	stat(configDir.c_str(), &st);
	bool isdir = S_ISDIR(st.st_mode);
	if(!isdir)
	{
		std::cout << "No config folder exist: " << configDir << std::endl;
		throw std::string("No config folder exist");
	}
	

			
	//const std::string keyStoreFileName = configDir + "/" + keypair.pubaddr + ".bitpriv";			
	//const std::string keyStoreFile = generateKeyStoreFile(keypair);
			
	//std::cout << "Saving to file: " << keyStoreFileName << std::endl;
	//fileWrite(keyStoreFileName, keyStoreFile);
			
	//Is there a default key?
	const std::string defaultFile = configDir + "/default_key.defpriv";	
	
	
	struct stat sta;	
	stat(defaultFile.c_str(), &sta);
	const bool defaultExist = S_ISREG(sta.st_mode);
	if(!defaultExist)
	{
		std::cout << "No default key exist: " << defaultFile << std::endl;
		throw std::string("No default key exist");
	}
		
	const std::string defaultPub = readFile(defaultFile);
	
	PubAddress pubaddr(defaultPub);
	
	return pubaddr; //defaultPub;
}


BigInt<256> BitsigConfig::getDefaultPrivkey(bool& compressed)
{
	const PubAddress defaultPubAddr = getPubForDefaultPriv();
	const std::string defaultPub = defaultPubAddr.toString(); //.address;
		
	//std::cout << "Default pub: " << defaultPub << std::endl;
	
	//Read the private key
	const std::string configDir = getOrCreateConfigDir();
	
	const std::string keyfile = configDir + "/" + defaultPub + ".bitpriv";
	
	const std::string fileContent = readFile(keyfile);
	
	//std::cout << "Priv: " << std::endl << fileContent << std::endl;
	
	const Wif wif = Wif(parseWifFromPrivFile(fileContent));
	
	const PrivKey pk = wif.getPriv();
	compressed = pk.compressed;
	const BigInt<256> privKey = pk.key; //wif.getPrivKey(compressed);
	return privKey;
}



bool BitsigConfig::checkIfDefaultExist()
{
	const std::string configDir = getOrCreateConfigDir();
	
	const std::string defaultFile = configDir + "/default_key.defpriv";	
	struct stat sta;	
	stat(defaultFile.c_str(), &sta);
	const bool defaultExist = S_ISREG(sta.st_mode);
	
	return defaultExist;
}


void BitsigConfig::setDefaultKey(const PubAddress& addr)
{
	const std::string configDir = getOrCreateConfigDir();
	const std::string defaultFile = configDir + "/default_key.defpriv";	
	
	std::cout << "Default indication file is: " << defaultFile << std::endl;
	fileWrite(defaultFile, addr.toString() /*.address*/);
}





void BitsigConfig::storePrivateKey(const BitcoinKeyPair& keypair)
{
	const std::string configDir = getOrCreateConfigDir();
		
			
	const std::string keyStoreFileName = configDir + "/" + keypair.pubaddr.toString() /*.address*/ + ".bitpriv";			
	const std::string keyStoreFile = generateKeyStoreFile(keypair);
			
	std::cout << "Saving to file: " << keyStoreFileName << std::endl;
	fileWrite(keyStoreFileName, keyStoreFile);
			
	//Is there a default key?
	const bool defaultExist = checkIfDefaultExist();
	
	/*
	const std::string defaultFile = configDir + "/default_key.defpriv";	
	struct stat sta;	
	stat(defaultFile.c_str(), &sta);
	const bool defaultExist = S_ISREG(sta.st_mode);
	*/
	
	if(defaultExist)
	{
		std::cout << "This is NOT set as default, since a default key already exist" << std::endl;
	}
	else
	{
		std::cout << "Setting as default key" << std::endl;
		
		setDefaultKey(keypair.pubaddr);
		//std::cout << "Creating: " << defaultFile << std::endl;
		//fileWrite(defaultFile, keypair.pubaddr.address);				
	}
	
}




bool BitsigConfig::findStoredPubAddr(const PubAddress& bitaddress, std::string& alias)
{
	//First try the public keys (without private key)
	const std::set<PubKeyAlias> storedPub = getAllStoredPub();
	for(std::set<PubKeyAlias>::const_iterator it = storedPub.begin() ;
		it != storedPub.end() ;
		++it)
	{
		const PubKeyAlias ka = *(it);
		const PubAddress pubAddr = ka.getPubAddr();
		
		if(bitaddress != pubAddr)
		{
			continue;
		}
				
		//OK, match!!
		//std::cout << "Using alias: " << ka.alias << " addr: " << ka.pubkey << std::endl;
		alias = ka.alias;
		return true;
	}


	//Now try our own addresses, with private key
	//std::string defPrivContent;
	//const std::set<BitcoinKeyPair> ownPairs = BitsigConfig::getOwnKeypairs(defPrivContent);
	if(hasPrivateForPublic(bitaddress))
	{
		std::cout << "This is our own address" << std::endl;
		alias = "MY KEY";
		return true;
	}
	
	return false;
}


bool BitsigConfig::findStoredPubAddrList(std::list<ECPoint>& possiblePoints, std::string& alias, ECPoint& foundPubkey)
{
	const std::set<PubKeyAlias> storedPub = getAllStoredPub();
	
	
	for(std::list<ECPoint>::const_iterator jt = possiblePoints.begin() ;
		jt != possiblePoints.end() ;
		++jt)
	{
		const ECPoint& point = (*jt);
		const PubKey pComp(point, true);
		const PubKey pUnComp(point, false);
		const PubAddress pubComp = pComp.getAddress();
		const PubAddress pubUnComp = pUnComp.getAddress();
				
		//const PubAddress pubComp = PubAddress::bitcoinAddresFromPubkey(point, true);
		//const PubAddress pubUnComp = PubAddress::bitcoinAddresFromPubkey(point, false);
		
		
		for(std::set<PubKeyAlias>::const_iterator it = storedPub.begin() ;
			it != storedPub.end() ;
			++it)
		{
			const PubKeyAlias ka = *(it);
			const PubAddress pubAddr = ka.getPubAddr();
		
			if(pubComp == pubAddr)
			{
				//OK, match!!
				//std::cout << "Using alias: " << ka.alias << " addr: " << ka.pubkey << std::endl;		
				foundPubkey = point;	
				alias = ka.alias;
				return true;				
			}
			else if(pubUnComp == pubAddr)
			{
				//OK, match!!
				//std::cout << "Using alias: " << ka.alias << " addr: " << ka.pubkey << std::endl;				
				foundPubkey = point;					
				alias = ka.alias;
				return true;				
			}
				
		}
	}
	
	return false;
}


