#pragma once

#include <string>
#include <vector>

class FileDirectory;
namespace Utilities
{
	const std::string& getResourceDirectory();
	std::vector<FileDirectory> getFileDirectories(const std::string& fileName);
}