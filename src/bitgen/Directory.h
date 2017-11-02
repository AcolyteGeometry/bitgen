#ifndef DIRECTORY_HEADER
#define DIRECTORY_HEADER


#include <set>
#include <string>


class Directory
{
public:
	Directory();
	Directory(const std::string& folderName);
	
	const std::set<std::string>& getEntries() const
	{
		return entries;
	}

	bool hasItem(const std::string& item) const;
	
private:
	void openDir(const std::string& folderName);

	std::set<std::string> entries;
};




#endif
