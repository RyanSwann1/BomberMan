#pragma once

#include <assert.h>
class AudioClipManager;
class AudioClipManagerLocator
{
public:
	static void provide(AudioClipManager& audioManager)
	{
		m_audioManager = &audioManager;
	}

protected:
	static AudioClipManager& getAudioManager()
	{
		assert(m_audioManager);
		return *m_audioManager;
	}

private:
	static AudioClipManager* m_audioManager;
};