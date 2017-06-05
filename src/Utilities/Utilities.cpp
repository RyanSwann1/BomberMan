#include "Utilities.h"
#include "FileDirectory.h"
#include <fstream>
#include <sstream>
#include <assert.h>
std::vector<FileDirectory> Utilities::getFileDirectories(const std::string & fileName)
{
	std::vector<FileDirectory> fileDirectories;
	std::ifstream file(fileName);
	assert(file.is_open());

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		std::string name;
		keyStream >> name;
		std::string directory;
		keyStream >> directory;
		fileDirectories.emplace_back(std::move(name), std::move(directory));
	}

	assert(!fileDirectories.empty());
	return fileDirectories;
}