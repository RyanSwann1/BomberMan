#pragma once

#include "NameConverter.h"
#include <Animation\AnimationName.h>
#include <Locators\AnimationNameConverterLocator.h>
#include <string>

class AnimationNameConverter : public NameConverter<AnimationName>
{
public:
	AnimationNameConverter()
	{
		AnimationNameConverterLocator::provide(*this);

		NameConverter::addName("Default", AnimationName::Default);
		NameConverter::addName("WalkingLeft", AnimationName::WalkingLeft);
		NameConverter::addName("WalkingRight", AnimationName::WalkingRight);
		NameConverter::addName("WalkingUp", AnimationName::WalkingUp);
		NameConverter::addName("WalkingDown", AnimationName::WalkingDown);
	}
};