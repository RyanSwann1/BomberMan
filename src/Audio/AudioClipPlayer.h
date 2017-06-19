#pragma once

#include <Locators\AudioClipManagerLocator.h>
#include "AudioClipName.h"
#include <EventQueue.h>
#include <SFML\Audio.hpp>
#include <unordered_map>

class AudioClipPlayer 
{
	class AudioClip : private AudioClipManagerLocator
	{
	public:
		AudioClip(std::string&& audioClipName);
		~AudioClip();

		sf::Sound m_audioClip;
		const std::string m_name;
	};

public:
	AudioClipPlayer();
	
	void playSound(AudioClipName audioName);
	void update();

private:
	std::unordered_map<AudioClipName, AudioClip> m_audioClips;
	EventQueue<AudioClipName> m_audioEventQueue;
};