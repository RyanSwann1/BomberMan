#pragma once

#include <SFML\Audio.hpp>
#include "ResourceManager.h"
#include <Utilities\Utilities.h>

class AudioClipManager : public ResourceManager<sf::SoundBuffer>
{
public:
	AudioClipManager();


private:
	const sf::SoundBuffer& loadFromFile(const std::string& fileDirectory) const override;
};