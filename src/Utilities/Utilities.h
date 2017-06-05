#pragma once

#include <string>
#include <vector>

class FileDirectory;
namespace Utilities
{
	void initialize();

	std::vector<FileDirectory> getFileDirectories(const std::string& fileName);
}