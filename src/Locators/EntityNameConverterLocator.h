#pragma once

#include <assert.h>
class EntityNameConverter;
class EntityNameConverterLocator
{
public:
	static void provide(EntityNameConverter& entityNameConverter)
	{
		m_entityNameConverter = &entityNameConverter;
	}

	static const auto& getEntityNameConverter()
	{
		assert(m_entityNameConverter);
		return m_entityNameConverter;
	}

private:
	static EntityNameConverter* m_entityNameConverter;
};
