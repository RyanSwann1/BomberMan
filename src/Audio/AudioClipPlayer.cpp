#include "AudioClipPlayer.h"
#include <Managers\AudioClipManager.h>
#include <Locators\AudioClipPlayerLocator.h>
#include <utility>

//AudioClip
AudioClipPlayer::AudioClip::AudioClip(std::string&& audioClipName)
	: m_audioClip(AudioClipManagerLocator::getAudioManager().getResource(audioClipName)),
	m_name(std::move(audioClipName))
{}

AudioClipPlayer::AudioClip::~AudioClip()
{
	AudioClipManagerLocator::getAudioManager().releaseResource(m_name);
}

//AudioClipPlayer
AudioClipPlayer::AudioClipPlayer()
{
	AudioClipPlayerLocator::provide(*this);
	m_audioClips.emplace(AudioClipName::BombExplode, "BombExplode");
	m_audioClips.emplace(AudioClipName::EnemyDeath, "EnemyDeath");
}

void AudioClipPlayer::playSound(AudioClipName audioName)
{
	m_audioEventQueue.addEvent(audioName);
}

void AudioClipPlayer::update()
{
	while (!m_audioEventQueue.isEmpty())
	{
		const auto audioClipName = m_audioEventQueue.getNextEvent();
		auto iter = m_audioClips.find(audioClipName);
		assert(iter != m_audioClips.cend());
		iter->second.m_audioClip.play();
	}
}
