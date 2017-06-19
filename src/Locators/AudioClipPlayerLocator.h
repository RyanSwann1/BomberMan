#pragma once

#include <assert.h>
class AudioClipPlayer;
class AudioClipPlayerLocator
{
public:
	static void provide(AudioClipPlayer& audioClipPlayer)
	{
		m_audioClipPlayer = &audioClipPlayer;
	}

	static AudioClipPlayer& getAudioClipPlayer()
	{
		assert(m_audioClipPlayer);
		return *m_audioClipPlayer;
	}

private:
	static AudioClipPlayer* m_audioClipPlayer;
};