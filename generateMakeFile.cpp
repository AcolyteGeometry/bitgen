
#include <iostream>
#include <set>


#include <sys/types.h>
#include <dirent.h>

#include "util.h"
#include "FileUtil.h"


class Directory
{
public:
	Directory(const std::string& path) : dir(0)
	{
		dir = opendir(path.c_str());
		if(dir == 0)
		{
			std::string errMsg("Could not open dir: ");
			errMsg += path;
			throw errMsg;
		}		
	}

	~Directory()
	{
		closedir(dir);
	}


	bool readEntry(std::string& fname) const
	{		
		for(;;)
		{
			const struct dirent* dirEntry = readdir(dir);
			if(dirEntry == 0)
			{
				return false;
			}
		
			const std::string filename(dirEntry->d_name);
			if(filename == "." || filename == "..")
			{
				continue;
			}

			fname = filename;
			return true;
		}
	}

private:
	DIR* dir; 
};





std::string objFilename(const std::string& filename)
{
	const std::string withoutcpp = removeEnding(filename, ".cpp");
	const std::string objfile = withoutcpp + ".o";
	return objfile;
}


std::string objCFilename(const std::string& filename)
{
	const std::string withoutcpp = removeEnding(filename, ".c");
	const std::string objfile = withoutcpp + ".o";
	return objfile;
}


std::string oneObjectFile(const std::string& filename, bool windows)
{
	std::string ret;
	
	const std::string withoutcpp = removeEnding(filename, ".cpp");
	const std::string objfile = withoutcpp + ".o";
	
	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}
	
	ret += objfile;
	ret += " ";
	
	return ret;
}



std::string oneCObjectFile(const std::string& filename, bool windows)
{
	std::string ret;
	
	const std::string withoutcpp = removeEnding(filename, ".c");
	const std::string objfile = withoutcpp + ".o";

	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}

	ret += objfile;
	ret += " ";
	
	return ret;
}




class CppFile
{
public:
	CppFile(const std::string& inPath, const std::string& inFilename) : 
		path(inPath), filename(inFilename) {}
	
	
	
	const std::string path;
	const std::string filename;
};

bool operator<(const CppFile& a, const CppFile& b)
{
	if(a.path < b.path)
	{
		return true;
	}
	else if(b.path < a.path)
	{
		return false;
	}
	else
	{
		return (a.filename < b.filename);
	}
}


std::string allObjectFiles(std::set<CppFile>& filenames, bool windows)
{
	std::string ret;
	
	for(std::set<CppFile>::const_iterator it = filenames.begin();
		it != filenames.end() ;
		++it)
	{
		const CppFile& cppFile = (*it);
		const std::string& filename = cppFile.filename;
		
		ret += oneObjectFile(filename, windows);
		
	}


	return ret;
}



std::string allCObjectFiles(std::set<std::string>& filenames, bool windows)
{
	std::string ret;
	
	for(std::set<std::string>::const_iterator it = filenames.begin();
		it != filenames.end() ;
		++it)
	{
		const std::string& filename = (*it);
		
		ret += oneCObjectFile(filename, windows);
		
	}


	return ret;
}



std::string cppFileDependency(const CppFile& thefile, bool windows)
{
	std::string ret;
	
	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}
	
	//ret += "obj/";
	ret += objFilename(thefile.filename) ;
	ret += ": src/";
	
	if(!thefile.path.empty())
	{
		ret += thefile.path;
		ret += "/";
	}
	
	ret += thefile.filename;
	ret += "\n";
	ret += "\t$(COMPILE) -o ";

	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}

	//ret += "obj/";
	ret += objFilename(thefile.filename);

	
	ret += " src/";

	if(!thefile.path.empty())
	{
		ret += thefile.path;
		ret += "/";
	}
	
	ret += thefile.filename;
	ret += "\n";
	
	return ret;
}



std::string cFileDependency(const std::string& filename, bool windows)
{
	std::string ret;
	
	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}
	
	//ret += "obj/";
	ret += objCFilename(filename) ;
	ret += ": src/qrenc/";
	ret += filename;
	ret += "\n";
	ret += "\t";

	if(windows)
	{
		ret += "i686-w64-mingw32-gcc";
	}
	else
	{
		ret += "gcc";
	}
	
	ret += " -DHAVE_CONFIG_H -o ";
	
	if(windows)
	{
		ret += "objwin/";			
	}
	else
	{
		ret += "obj/";	
	}
	
	//obj/";
	ret += objCFilename(filename) ;	
	ret += " -c src/qrenc/";
	ret += filename;
	ret += "\n";
	
	return ret;
}



std::string exeName(const std::string& coin)
{
	if(coin == "bitcoin")
	{
		return "bitgen";
	}
	else if(coin == "btc")
	{
		return "bitgen-btc";
	}
	else if(coin == "bitcoincash")
	{
		return "bitgen-bcc";
	}		
	else if(coin == "litecoin")
	{
		return "bitgen-ltc";
	}
	else if(coin == "namecoin")
	{
		return "bitgen-nmc";
	}
	else if(coin == "peercoin")
	{
		return "bitgen-ppc";
	}
	else if(coin == "dogecoin")
	{
		return "bitgen-doge";
	}
	else if(coin == "bctest")
	{
		return "bitgen-bctest";
	}
	else if(coin == "vertcoin")
	{
		return "bitgen-vtc";
	}
	//else if(coin == "reddcoin")
	//{
	//	return "bitgen-rdd";
	//}
	else if(coin == "feathercoin")
	{
		return "bitgen-ftc";
	}			
	else
	{
		std::cerr << "Unknown coin: " << coin << std::endl;
		throw std::string("Unknown coin");
	}
	
}





void generateBinaryForBitcoin(const std::string& appname, const bool windows, std::set<CppFile>& filenames, std::set<std::string>& c_filenames)
{
	std::cout << appname << ": ";
	if(windows)
	{
		std::cout << "objwin/";
	}
	else
	{
		std::cout << "obj/";
	}
	
	std::cout << appname << ".o " << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) << oneObjectFile("currencySpecific_bitcoin_specific.cpp", windows) << std::endl;
	std::cout << "\t";
	if(windows)
	{
		std::cout << "i686-w64-mingw32-g++ -pthread -static";
	}
	else
	{
		std::cout << "g++";
	}
	
	std::cout << " -o " << appname; //bitsig";

	if(windows)
	{
		std::cout << ".exe";
	}

	std::cout << " ";


	if(windows)
	{
		std::cout << "objwin/";
	}
	else
	{
		std::cout << "obj/";
	}
	
	
	std::cout << appname << ".o " << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) << oneObjectFile("currencySpecific_bitcoin_specific.cpp", windows) << std::endl;
	std::cout << std::endl;
}






void linkCoinExecutable(const std::string& coin, const bool windows, std::set<CppFile>& filenames, std::set<std::string>& c_filenames, const bool bitgenBitcoin)
{
	std::cout << (bitgenBitcoin ? "btc" : coin) << ": ";
		
	const std::string mainFile = "bitgen_" + coin + ".cpp";
	std::cout << oneObjectFile(mainFile, windows) << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) << 
		oneObjectFile("currencySpecific_" + coin + "_specific.cpp", windows);

	std::cout << std::endl;
	std::cout << "\t";
	if(windows)
	{
		std::cout << "i686-w64-mingw32-g++ -pthread -static";
	}
	else
	{
		std::cout << "g++";
	}
		
	std::cout << " -o " << exeName(bitgenBitcoin ? "btc" : coin);
	if(windows)
	{
		std::cout << ".exe";
	}
		
	std::cout << " " << oneObjectFile(mainFile, windows) << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) <<
		oneObjectFile("currencySpecific_" + coin + "_specific.cpp", windows);
		
	std::cout << std::endl;
	std::cout << std::endl;
}



int doTheWork(const std::string& param)
{
	const bool windows = (param == "windows");

	if(windows)
	{
		std::cout << "#Generated makefile for cross-compiling for Windows on Linux" << std::endl;
	}
	else
	{
		std::cout << "#Generated makefile for Linux" << std::endl;
	}
	
	std::cout << std::endl;

	if(windows)
	{
		std::cout << "COMPILE=i686-w64-mingw32-g++ -O2 -msse2 -I. -Isrc -Isrc/bitlib/bigintctl -Isrc/bitlib -Isrc/qrenc -Isrc/argon -Isrc/bitgen -DSHA2_USE_INTTYPES_H -DBYTE_ORDER=LITTLE_ENDIAN -static -c" << std::endl;
	}
	else
	{
		std::cout << "ifeq (\"$(MAKECMDGOALS)\",\"withoutbw\")" << std::endl;
		std::cout << "\tCOMPILE=g++ -I. -Isrc -Isrc/bitlib/bigintctl -Isrc/bitlib -Isrc/qrenc -Isrc/argon -Isrc/bitgen -c" << std::endl;
		std::cout << "else" << std::endl;
		std::cout << "\tCOMPILE=g++ -O2 -msse2 -I. -Isrc -Isrc/bitlib/bigintctl -Isrc/bitlib -Isrc/qrenc -Isrc/argon -Isrc/bitgen -c" << std::endl;
		std::cout << "endif" << std::endl;		
	}
	
	std::cout << std::endl;

	std::set<std::string> coins;
	coins.insert("bitcoin");
	coins.insert("bitcoincash");	
	coins.insert("litecoin");
	coins.insert("dogecoin");
	coins.insert("bctest");
	coins.insert("namecoin");
	coins.insert("peercoin");
	coins.insert("vertcoin");
	//coins.insert("reddcoin");
	coins.insert("feathercoin");


	std::cout << "all: bitsig bitcry bitcalc btc ";

	for(std::set<std::string>::const_iterator it = coins.begin() ;
		it != coins.end() ;
		++it)
	{
		const std::string& coin = (*it);
		std::cout << coin << " ";
	}

	std::cout << std::endl;
	std::cout << std::endl;
		

	std::cout << "withoutbw: bitsig bitcry bitcalc btc ";

	for(std::set<std::string>::const_iterator it = coins.begin() ;
		it != coins.end() ;
		++it)
	{
		const std::string& coin = (*it);
		std::cout << coin << "_nobw ";
	}

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "test:" << std::endl;
	std::cout << "\t./bitgen test" << std::endl;
	std::cout << std::endl;




	
	
	Directory thisDir("src");

	std::set<CppFile/*std::string*/> filenames;
	std::set<std::string> c_filenames;

	for(;;)
	{
		std::string filename;
		if(!thisDir.readEntry(filename))
		{
			break;
		}
	
		if(endsWith(filename, "_specific.cpp"))
		{
			continue;
		}

		if(filename == "currencySpecific.cpp")
		{
			continue;
		}



		if(startsWith(filename, "bitgen_"))
		{
			continue;
		}

		if(filename == "bitsig.cpp" || filename == "bitcry.cpp" | filename == "bitcalc.cpp")
		{
			continue;
		}

		//if(filename == "unittest.cpp")
		//{
		//	continue;
		//}

	
		if(endsWith(filename, ".cpp"))
		{
			CppFile theFile("", filename);
			filenames.insert(theFile);
		}
		else if(endsWith(filename, ".c"))
		{
			c_filenames.insert(filename);
		}
		//std::cout << "Filename: " << filename << std::endl;
	}
	
	
	Directory argonDir("src/argon");
	for(;;)
	{
		std::string filename;
		if(!argonDir.readEntry(filename))
		{
			break;
		}
	
		if(endsWith(filename, ".cpp"))
		{
			CppFile myFile("argon", filename);
			filenames.insert(myFile);
		}
	
	}

	Directory bitgenDir("src/bitgen");
	for(;;)
	{
		std::string filename;
		if(!bitgenDir.readEntry(filename))
		{
			break;
		}
	
		if(endsWith(filename, ".cpp"))
		{
			CppFile myFile("bitgen", filename);
			filenames.insert(myFile);
		}
	
	}

	
	Directory bigintDir("src/bitlib/bigintctl");
	for(;;)
	{
		std::string filename;
		if(!bigintDir.readEntry(filename))
		{
			break;
		}
	
		if(endsWith(filename, ".cpp"))
		{
			//const std::string totalFilename = "bigintctl/" + filename;
			CppFile myFile("bitlib/bigintctl", filename);
			filenames.insert(myFile); //filename, "bigintctl");
		}
	
	}
	

	Directory bitlibDir("src/bitlib");
	for(;;)
	{
		std::string filename;
		if(!bitlibDir.readEntry(filename))
		{
			break;
		}
	
		if(endsWith(filename, "_specific.cpp"))
		{
			continue;
		}

		if(filename == "currencySpecific.cpp")
		{
			continue;
		}
	
	
		if(endsWith(filename, ".cpp"))
		{
			CppFile myFile("bitlib", filename);
			filenames.insert(myFile);
		}
	
	}

	
	
	Directory qrencDir("src/qrenc");
	for(;;)
	{
		std::string filename;
		if(!qrencDir.readEntry(filename))
		{
			break;
		}
		
		if(endsWith(filename, ".c"))
		{
			c_filenames.insert(filename);
		}
		
	}
	
	//filenames.insert("currencySpecific_bctest_specific.cpp");	
	//filenames.insert("currencySpecific_dogecoin_specific.cpp");	
	//filenames.insert("currencySpecific_litecoin_specific.cpp");	
	//filenames.insert("currencySpecific_dogecoin_specific.cpp");	
	//filenames.insert("currencySpecific_peercoin_specific.cpp");	
	
/*	
	for(std::set<std::string>::const_iterator it = filenames.begin();
		it != filenames.end() ;
		++it)
	{
		const std::string& filename = (*it);
	}
*/
	
	

	for(std::set<std::string>::const_iterator it = coins.begin() ;
		it != coins.end() ;
		++it)
	{
		const std::string& coin = (*it);
		linkCoinExecutable(coin, windows, filenames, c_filenames, false);
		if(coin == "bitcoin")
		{
			linkCoinExecutable(coin, windows, filenames, c_filenames, true);			
		}
		
/*		
		std::cout << coin << ": ";
		
		const std::string mainFile = "bitgen_" + coin + ".cpp";
		std::cout << oneObjectFile(mainFile, windows) << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) << 
			oneObjectFile("currencySpecific_" + coin + "_specific.cpp", windows);

		std::cout << std::endl;
		std::cout << "\t";
		if(windows)
		{
			std::cout << "i686-w64-mingw32-g++ -pthread -static";
		}
		else
		{
			std::cout << "g++";
		}
		
		std::cout << " -o " << exeName(coin);
		if(windows)
		{
			std::cout << ".exe";
		}
		
		std::cout << " " << oneObjectFile(mainFile, windows) << allObjectFiles(filenames, windows) << allCObjectFiles(c_filenames, windows) <<
			oneObjectFile("currencySpecific_" + coin + "_specific.cpp", windows);
		
		std::cout << std::endl;
		std::cout << std::endl;
*/
	}


//--------------------------------------------

	generateBinaryForBitcoin("bitsig", windows, filenames, c_filenames);
	generateBinaryForBitcoin("bitcry", windows, filenames, c_filenames);
	generateBinaryForBitcoin("bitcalc", windows, filenames, c_filenames);
	
	
	
//--------------------------------------------


	std::set<CppFile> compileFilename = filenames;
	
	CppFile file_bitcoin("", "bitgen_bitcoin.cpp");	
	compileFilename.insert(file_bitcoin);

	CppFile file_bitcoincash("", "bitgen_bitcoincash.cpp");	
	compileFilename.insert(file_bitcoincash);

	CppFile file_bctest("", "bitgen_bctest.cpp");		
	compileFilename.insert(file_bctest);

	CppFile file_dogecoin("", "bitgen_dogecoin.cpp");			
	compileFilename.insert(file_dogecoin);

	CppFile file_litecoin("", "bitgen_litecoin.cpp");				
	compileFilename.insert(file_litecoin);

	CppFile file_namecoin("", "bitgen_namecoin.cpp");					
	compileFilename.insert(file_namecoin);

	CppFile file_peercoin("", "bitgen_peercoin.cpp");						
	compileFilename.insert(file_peercoin);

	CppFile file_vertcoin("", "bitgen_vertcoin.cpp");						
	compileFilename.insert(file_vertcoin);

	//CppFile file_reddcoin("", "bitgen_reddcoin.cpp");						
	//compileFilename.insert(file_reddcoin);

	CppFile file_feathercoin("", "bitgen_feathercoin.cpp");						
	compileFilename.insert(file_feathercoin);
	
	CppFile file_bitsig("", "bitsig.cpp");						
	compileFilename.insert(file_bitsig);
	
	CppFile file_bitcry("", "bitcry.cpp");							
	compileFilename.insert(file_bitcry);

	CppFile file_bitcalc("", "bitcalc.cpp");							
	compileFilename.insert(file_bitcalc);

	
	CppFile file_unittest("", "unittest.cpp");								
	compileFilename.insert(file_unittest);


	for(std::set<std::string>::const_iterator it = coins.begin() ;
		it != coins.end() ;
		++it)
	{
		const std::string& coin = (*it);

		std::string myfilename ="currencySpecific_" + coin + "_specific.cpp";
		
		CppFile myfile("bitlib", myfilename);								
		
		compileFilename.insert(myfile);
	}


	for(std::set<CppFile>::const_iterator it = compileFilename.begin();
		it != compileFilename.end() ;
		++it)
	{
		//const std::string& filename = (*it);
		CppFile myfile = (*it); //("", filename);								
		
		std::cout << cppFileDependency(myfile, windows) << std::endl;		
	}

	for(std::set<std::string>::const_iterator it = c_filenames.begin();
		it != c_filenames.end() ;
		++it)
	{
		const std::string& filename = (*it);
		std::cout << cFileDependency(filename, windows) << std::endl;		
	}


	//std::cout << cppFileDependency("currencySpecific_bctest_specific.cpp") << std::endl;



std::cout << std::endl;
std::cout << std::endl;
std::cout << "clean:" << std::endl;
std::cout << "\trm -f *.o *.exe obj/*.o objwin/*.o bitgen bitsig bitcry bitcalc bitgen-btc bitgen-ltc bitgen-doge bitgen-testnet bitgen-nmc bitgen-ppc bitgen-vtc bitgen-rdd bitgen-ftc bitgen-bctest bitgen-bcc" << std::endl;
std::cout << std::endl;

	
}



int main(int argc, const char** argv)
{
	std::string param;
	if(argc > 1)
	{
		param = argv[1];
		//std::cout << "Arg: " << param << std::endl;
	}
	
	try
	{
		doTheWork(param);
	}
	catch(const std::string& err)
	{
		std::cerr << "Error: " << err << std::endl;
	}
}
