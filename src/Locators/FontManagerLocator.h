#pragma once

#include <assert.h>
class FontManager;
class FontManagerLocator
{
public:
	static void provide(FontManager& fontManager)
	{
		m_fontManager = &fontManager;
	}

	static FontManager& getFontManager()
	{
		assert(m_fontManager);
		return *m_fontManager;
	}

private:
	static FontManager* m_fontManager;
};