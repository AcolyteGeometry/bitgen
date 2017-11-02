
#include <string>


void assert(const bool val)
{
	if(!val)
	{
		throw std::string("Assert error");
	}
}



