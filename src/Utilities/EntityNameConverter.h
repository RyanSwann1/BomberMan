#pragma once

#include <Entities\EntityName.h>
#include <Utilities\NameConverter.h>
#include <Locators\EntityNameConverterLocator.h>

class EntityNameConverter : public NameConverter<EntityName>
{
public:
	EntityNameConverter()
	{
		EntityNameConverterLocator::provide(*this);

		NameConverter::addName("Player", EntityName::Player);
		NameConverter::addName("Bomb", EntityName::Bomb);
		NameConverter::addName("Explosion", EntityName::Explosion);
		NameConverter::addName("Enemy1", EntityName::Enemy1);
		NameConverter::addName("Enemy2", EntityName::Enemy2);
		NameConverter::addName("Enemy3", EntityName::Enemy3);
		NameConverter::addName("Crate", EntityName::Crate);
	}
};