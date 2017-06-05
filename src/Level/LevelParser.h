#pragma once

#include <string>
class Level;
namespace LevelParser
{
	Level parseLevel(const std::string& fileDirectory, const std::string& levelName);
}