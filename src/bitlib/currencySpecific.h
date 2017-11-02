#ifndef CURRENCY_SPECIFIC_H
#define CURRENCY_SPECIFIC_H

#include <string>

std::string getExtendedPrivPrefixHex();
std::string getExtendedPubPrefixHex();

char getWifPrefix();	//Todo: unsigned??
const char* getWifHighBytePrefix();

int getCurrencyBip44Pos();

std::string getUrlPrefix();
std::string getCurrencyName();
std::string getCurrencyShortName();

unsigned char getMultisigVersion();
unsigned char getNetVersion();
bool possibleFirstChar(const char c);

#endif
