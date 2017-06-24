#pragma once

#include <assert.h>
class AudioPlayer;
class AudioPlayerLocator
{
public:
	static void provide(AudioPlayer& audioClipPlayer)
	{
		m_audioPlayer = &audioClipPlayer;
	}

	static AudioPlayer& getAudioClipPlayer()
	{
		assert(m_audioPlayer);
		return *m_audioPlayer;
	}

private:
	static AudioPlayer* m_audioPlayer;
};