#pragma once

#include <assert.h>
class TextureManager;
class TextureManagerLocator
{
public:
	static void provide(TextureManager& textureManager)
	{
		m_textureManager = &textureManager;
	}

protected:
	static TextureManager& getTextureManager()
	{
		assert(m_textureManager);
		return *m_textureManager;
	}

private:
	static TextureManager* m_textureManager;
};