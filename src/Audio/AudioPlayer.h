#pragma once

#include <Locators\AudioClipManagerLocator.h>
#include "AudioClipName.h"
#include <EventQueue.h>
#include <SFML\Audio.hpp>
#include <unordered_map>

class AudioPlayer 
{
	class AudioClip : private AudioClipManagerLocator
	{
	public:
		AudioClip(std::string&& audioClipName);
		~AudioClip();
		AudioClip(const AudioClip&) = delete;

		sf::Sound m_audioClip;
		const std::string m_name;
	};

public:
	AudioPlayer();
	
	void playAudioClip(AudioClipName audioName);
	void playMusic(const std::string& musicName);
	void stopMusic();

	void update();

private:
	std::unordered_map<AudioClipName, AudioClip> m_audioClips;
	sf::Music m_music;

	EventQueue<AudioClipName> m_audioEventQueue;
};