#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include <vector>


bool hexChar(const char c);
bool oneToSix(const char c);
bool everythingOk(const char c);


std::string readStdinLine(const int minChars, const bool exactNum, bool (*charOk)(const char c));
std::vector<std::string> getExtraArgs(const std::vector<std::string>& args, const int start);



#endif
