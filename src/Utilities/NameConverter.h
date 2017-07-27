#pragma once

#include <unordered_map>
#include <string>
#include <assert.h>
#include <utility>
#include <algorithm>

template <class T>
class NameConverter
{
public:
	const std::string& getStringName(T nameToConvert) const
	{
		auto cIter = std::find_if(m_names.cbegin(), m_names.cend(), [nameToConvert](const auto& name) { return name.second == nameToConvert; });
		assert(cIter != m_names.cend());

		return cIter->first;
	}

	T convertName(const std::string& name) const
	{
		auto iter = m_names.find(name);
		assert(iter != m_names.cend());
		
		return iter->second;
	}

protected:
	void addName(std::string&& stringName, T name)
	{
		assert(m_names.find(stringName) == m_names.cend());
		m_names.emplace(std::move(stringName), name);
	}

private:
	std::unordered_map<std::string, T> m_names;
};