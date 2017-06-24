#include <Managers\AudioClipManager.h>
#include <Locators\AudioClipManagerLocator.h>

AudioClipManager::AudioClipManager()
	: ResourceManager(Utilities::getFileDirectories(Utilities::getResourceDirectory() + "AudioClips.txt"))
{
	AudioClipManagerLocator::provide(*this);
}

const sf::SoundBuffer & AudioClipManager::loadFromFile(const std::string & fileDirectory) const
{
	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	const bool soundBufferLoaded = soundBuffer->loadFromFile(fileDirectory);
	assert(soundBufferLoaded);
	return *soundBuffer;
}
