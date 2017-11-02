#ifndef WINDOWS_RANDOM_H
#define WINDOWS_RANDOM_H

#ifndef __linux 

#include <windows.h>
#include <wincrypt.h>
#include <string>


class WinRandom
{
public:
  WinRandom();
  ~WinRandom();

  std::string generate(int len);

private:  
  HCRYPTPROV hProvider;  
};



#endif
#endif
