#include <Managers\FontManager.h>
#include <Utilities\Utilities.h>

FontManager::FontManager()
	: ResourceManager(Utilities::getFileDirectories(Utilities::getResourceDirectory() + "Fonts.txt"))
{

}

const sf::Font & FontManager::loadFromFile(const std::string & fileName) const
{
	sf::Font* font = new sf::Font();
	const bool loadedFromFile = font->loadFromFile(fileName);
	assert(loadedFromFile);
	return *font;	
}
