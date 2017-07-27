#pragma once

#include <assert.h>
class AnimationNameConverter;
class AnimationNameConverterLocator
{
public:
	static void provide(AnimationNameConverter& animationNameConverter)
	{
		m_animationNameConverter = &animationNameConverter;
	}

	static const auto& getAnimationNameConverter()
	{
		assert(m_animationNameConverter);
		return m_animationNameConverter;
	}

private:
	static AnimationNameConverter* m_animationNameConverter;
};

