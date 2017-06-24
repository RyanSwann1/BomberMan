#include "AudioPlayer.h"
#include <Utilities\Utilities.h>
#include <Managers\AudioClipManager.h>
#include <Locators\AudioClipPlayerLocator.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\GameEvent.h>
#include <utility>


//AudioClip
AudioPlayer::AudioClip::AudioClip(std::string&& audioClipName)
	: m_audioClip(AudioClipManagerLocator::getAudioManager().getResource(audioClipName)),
	m_name(std::move(audioClipName))
{}

AudioPlayer::AudioClip::~AudioClip()
{
	AudioClipManagerLocator::getAudioManager().releaseResource(m_name);
}

//AudioClipPlayer
AudioPlayer::AudioPlayer()
{
	AudioClipPlayerLocator::provide(*this);
	m_audioClips.emplace(AudioClipName::BombExplode, "BombExplode");
	m_audioClips.emplace(AudioClipName::EnemyDeath, "EnemyDeath");
	m_audioClips.emplace(AudioClipName::PlayerDeath, "PlayerDeath");
	m_audioClips.emplace(AudioClipName::PlaceBomb, "PlaceBomb");
}

void AudioPlayer::playAudioClip(AudioClipName audioName)
{
	m_audioEventQueue.addEvent(audioName);
}

void AudioPlayer::playMusic(const std::string & musicName)
{
	if (m_music.getStatus() == sf::Music::Playing)
	{
		m_music.stop();
	}
	
	m_music.openFromFile(Utilities::getResourceDirectory() + musicName);
	m_music.play();
}

void AudioPlayer::update()
{
	while (!m_audioEventQueue.isEmpty())
	{
		const auto audioClipName = m_audioEventQueue.getNextEvent();
		auto iter = m_audioClips.find(audioClipName);
		assert(iter != m_audioClips.cend());
		iter->second.m_audioClip.play();
	}
}
