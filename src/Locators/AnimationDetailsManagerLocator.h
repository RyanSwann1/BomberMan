#pragma once

#include <assert.h>
class AnimationDetailsManager;
class AnimationDetailsManagerLocator
{
public:
	static void provide(AnimationDetailsManager& animationDetailsManager)
	{
		m_animationDetailsManager = &animationDetailsManager;
	}

	static const AnimationDetailsManager& getAnimationDetailsManager()
	{
		assert(m_animationDetailsManager);
		return *m_animationDetailsManager;
	}

private:
	static AnimationDetailsManager* m_animationDetailsManager;
};