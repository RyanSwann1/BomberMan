#pragma once

#include <assert.h>
class AudioPlayer;
class AudioClipPlayerLocator
{
public:
	static void provide(AudioPlayer& audioClipPlayer)
	{
		m_audioClipPlayer = &audioClipPlayer;
	}

	static AudioPlayer& getAudioClipPlayer()
	{
		assert(m_audioClipPlayer);
		return *m_audioClipPlayer;
	}

private:
	static AudioPlayer* m_audioClipPlayer;
};