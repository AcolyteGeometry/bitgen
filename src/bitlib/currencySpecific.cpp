

#include <string>
#include <iostream>




//Todo!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::string getExtendedPrivPrefixHex()
{
#ifdef CURRENCY_LITECOIN
	return "019D9CFE";	//Litecoin
#elif defined CURRENCY_DOGECOIN
	return "02FAC398";		//Dogecoin
#elif defined CURRENCY_BITCOINTESTNET
	return "00000000";		//TBD
#elif defined CURRENCY_NAMECOIN
	return "00000000";		//TBD
#elif defined CURRENCY_PEERCOIN		
	return "00000000";		//TBD
#elif defined CURRENCY_BITCOIN
	return "0488ADE4";		//Bitcoin
#elif defined CURRENCY_BITCOINCASH
	return "0488ADE4";		//Bitcoin cash	
#elif defined CURRENCY_VERTCOIN
	return "00000000";		//TBD
#elif defined CURRENCY_REDDCOIN
	return "00000000";		//TBD
#elif defined CURRENCY_FEATHERCOIN
	return "00000000";		//TBD
#else
	Error no currency defined
#endif	

}


//Todo!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::string getExtendedPubPrefixHex()
{
#ifdef CURRENCY_LITECOIN
	return "019DA462";	//Litecoin
#elif defined CURRENCY_DOGECOIN
	return "02FACADF";		//Dogecoin
#elif defined CURRENCY_BITCOINTESTNET
	return "00000000";		//TBD
#elif defined CURRENCY_NAMECOIN
	return "00000000";		//TBD
#elif defined CURRENCY_PEERCOIN		
	return "00000000";		//TBD
#elif defined CURRENCY_BITCOIN
	return "0488B21E";		//Bitcoin
#elif defined CURRENCY_BITCOINCASH
	return "0488B21E";		//Bitcoin cash
#elif defined CURRENCY_VERTCOIN
	return "00000000";		//TBD
#elif defined CURRENCY_REDDCOIN
	return "00000000";		//TBD
#elif defined CURRENCY_FEATHERCOIN
	return "00000000";		//TBD
#else
	Error no currency defined
#endif	

}


char getWifPrefix()
{
#ifdef CURRENCY_LITECOIN
	const char wifPrefix(0xB0);
#elif defined CURRENCY_DOGECOIN
	const char wifPrefix(0x9E);
#elif defined CURRENCY_BITCOINTESTNET
	const char wifPrefix(0xEF);
#elif defined CURRENCY_NAMECOIN
	const char wifPrefix(0x80);
#elif defined CURRENCY_PEERCOIN		
	const char wifPrefix(0xB7);
#elif defined CURRENCY_BITCOIN
	const char wifPrefix(0x80);
#elif defined CURRENCY_BITCOINCASH
	const char wifPrefix(0x80);
#elif defined CURRENCY_VERTCOIN
	const char wifPrefix(0xC7);
#elif defined CURRENCY_REDDCOIN
	const char wifPrefix(0xBD);		//Todo
#elif defined CURRENCY_FEATHERCOIN
	const char wifPrefix(0x8E);		//Todo
#else
	Error no currency defined
#endif	

	return wifPrefix;
	
}



const char* getWifHighBytePrefix()
{
#ifdef CURRENCY_LITECOIN
	 return "B0";
#elif defined CURRENCY_DOGECOIN
	return "9E";
#elif defined CURRENCY_BITCOINTESTNET
	return "EF";		
#elif defined CURRENCY_NAMECOIN
	return "80";
#elif defined CURRENCY_PEERCOIN		
	return "B7";
#elif defined CURRENCY_BITCOIN
	return "80";
#elif defined CURRENCY_BITCOINCASH
	return "80";
#elif defined CURRENCY_VERTCOIN
	return "C7";
#elif defined CURRENCY_REDDCOIN
	return "BD";					//Todo	
#elif defined CURRENCY_FEATHERCOIN
	return "8E";					//Todo	
#else
	Error no currency defined
#endif	
}




std::string getUrlPrefix()
{	
#ifdef CURRENCY_LITECOIN
	return "litecoin:";
#elif defined CURRENCY_DOGECOIN
	return "dogecoin:";
#elif defined CURRENCY_BITCOINTESTNET
	return "bitcoin:";	
#elif defined CURRENCY_NAMECOIN
	return "namecoin:";
#elif defined CURRENCY_PEERCOIN		
	return "peercoin:";
#elif defined CURRENCY_BITCOIN
	return "bitcoin:";
#elif defined CURRENCY_BITCOINCASH
	return "bitcoincash:";	
#elif defined CURRENCY_VERTCOIN
	return "vertcoin:";
#elif defined CURRENCY_REDDCOIN
	return "reddcoin:";				//Todo	
#elif defined CURRENCY_FEATHERCOIN
	return "feathercoin:";				//Todo	
#else
	Error no currency defined
#endif

}


int getCurrencyBip44Pos()
{
#ifdef CURRENCY_LITECOIN
	return 2;
#elif defined CURRENCY_DOGECOIN
	return 3;
#elif defined CURRENCY_BITCOINTESTNET
	return 1;
#elif defined CURRENCY_NAMECOIN
	return 7;
#elif defined CURRENCY_PEERCOIN		
	return 6;
#elif defined CURRENCY_BITCOIN
	return 0;
#elif defined CURRENCY_BITCOINCASH
	return 145;
#elif defined CURRENCY_VERTCOIN
	return 28;
#elif defined CURRENCY_REDDCOIN
	return 4;
#elif defined CURRENCY_FEATHERCOIN
	return 8;
#else
	Error no currency defined
#endif
	
}


std::string getCurrencyName()
{
#ifdef CURRENCY_LITECOIN
	return "Litecoin";
#elif defined CURRENCY_DOGECOIN
	return "Dogecoin";
#elif defined CURRENCY_BITCOINTESTNET
	return "Bitcoin testnet";
#elif defined CURRENCY_NAMECOIN
	return "Namecoin";	
#elif defined CURRENCY_PEERCOIN		
	return "Peercoin";				
#elif defined CURRENCY_BITCOIN
	return "Bitcoin";
#elif defined CURRENCY_BITCOINCASH
	return "Bitcoin cash";
#elif defined CURRENCY_VERTCOIN
	return "Vertcoin";
#elif defined CURRENCY_REDDCOIN
	return "Reddcoin";				//Todo
#elif defined CURRENCY_FEATHERCOIN
	return "Feathercoin";				//Todo
#else
	Error no currency defined
#endif
}


std::string getCurrencyShortName()
{
#ifdef CURRENCY_LITECOIN
	return "ltc";
#elif defined CURRENCY_DOGECOIN
	return "doge";
#elif defined CURRENCY_BITCOINTESTNET
	return "bctest";
#elif defined CURRENCY_NAMECOIN
	return "nmc";	
#elif defined CURRENCY_PEERCOIN		
	return "ppc";				
#elif defined CURRENCY_BITCOIN
	return "btc";
#elif defined CURRENCY_BITCOINCASH
	return "bcc";
#elif defined CURRENCY_VERTCOIN
	return "vtc";
#elif defined CURRENCY_REDDCOIN
	return "rdd";					//Todo
#elif defined CURRENCY_FEATHERCOIN
	return "ftc";					//Todo
#else
	Error no currency defined
#endif
}





unsigned char getMultisigVersion()
{
#ifdef CURRENCY_LITECOIN
	return '\x32';		//This is not yet supported by the litecoin network
	//return '\x05';		//Old multisig version
#elif defined CURRENCY_DOGECOIN
	return '\x16';
#elif defined CURRENCY_BITCOINTESTNET
	return '\xc4';
#elif defined CURRENCY_NAMECOIN
	return '\x0D';
#elif defined CURRENCY_PEERCOIN
	return '\x75';
#elif defined CURRENCY_BITCOIN
	return '\x05';
#elif defined CURRENCY_BITCOINCASH
	return '\x05';
#elif defined CURRENCY_VERTCOIN
	return '\x05';
#elif defined CURRENCY_REDDCOIN
	return '\x05';					//Todo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
#elif defined CURRENCY_FEATHERCOIN
	return '\x05';					//Todo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
#else
	throw std::string("Multisig not defined");
	//Error no currency defined
#endif
}




unsigned char getNetVersion()
{
#ifdef CURRENCY_LITECOIN
	return '\x30';
#elif defined CURRENCY_DOGECOIN
	return '\x1E';
#elif defined CURRENCY_BITCOINTESTNET
	return '\x6F';	
#elif defined CURRENCY_NAMECOIN
	return '\x34';
#elif defined CURRENCY_PEERCOIN
	return '\x37';
#elif defined CURRENCY_BITCOIN
	return '\0';
#elif defined CURRENCY_BITCOINCASH
	return '\0';
#elif defined CURRENCY_VERTCOIN
	return '\x47';
#elif defined CURRENCY_REDDCOIN
	return '\x3D';				//Todo
#elif defined CURRENCY_FEATHERCOIN
	return '\x0E';				//Todo
#else
	Error no currency defined
#endif
}


bool possibleFirstChar(const char c)
{
#ifdef CURRENCY_LITECOIN
	return c == 'L';
#elif defined CURRENCY_DOGECOIN
	return c == 'D';
#elif defined CURRENCY_BITCOINTESTNET
	return c == 'm';
#elif defined CURRENCY_NAMECOIN
	return (c == 'N' || c == 'M');
#elif defined CURRENCY_PEERCOIN
	return c == 'P';
#elif defined CURRENCY_BITCOIN
	return c == '1';
#elif defined CURRENCY_BITCOINCASH
	return c == '1';
#elif defined CURRENCY_VERTCOIN
	return c == 'V';
#elif defined CURRENCY_REDDCOIN
	return c == 'R';				//Todo
#elif defined CURRENCY_FEATHERCOIN
	return (c == '6' || c == '7');
#else
	Error no currency defined
#endif
}

