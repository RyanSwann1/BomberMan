#pragma once

#include <string>
#include <utility>

class FileDirectory
{
public:
	FileDirectory(std::string&& fileName, std::string&& fileDirectory)
		: m_fileName(std::move(fileName)),
		m_fileDirectory(std::move(fileDirectory))
	{}

	const std::string m_fileName;
	const std::string m_fileDirectory;
};