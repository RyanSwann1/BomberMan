#include "AnimationDetailsManager.h"
#include "Locators\AnimationDetailsManagerLocator.h"
#include <Utilities\Utilities.h>
#include <assert.h>
#include <sstream>
#include <fstream>

AnimationDetailsManager::AnimationDetailsManager()
	: m_animationDetails(loadInAnimationDetails(Utilities::getResourceDirectory() + "Animations.txt"))
{
	AnimationDetailsManagerLocator::provide(*this);
	
}

const std::string & AnimationDetailsManager::getTileSheetName(const std::string & entityName) const
{
	auto iter = m_animationDetails.find(entityName);
	assert(iter != m_animationDetails.cend());
	return iter->second.m_entityAnimationDetails.begin()->m_tileSheetName;
}

const AnimationDetailsManager::AnimationDetails & AnimationDetailsManager::getAnimationDetails(const std::string & entityName, const std::string& animationName) const
{
	auto iter = m_animationDetails.find(entityName);
	assert(iter != m_animationDetails.cend());
	const auto& entityAnimationDetails = iter->second.m_entityAnimationDetails;
	auto animationDetails = std::find_if(entityAnimationDetails.cbegin(), entityAnimationDetails.cend(), [&animationName](const auto& details) {return details.m_animationName == animationName; });
	assert(animationDetails != entityAnimationDetails.cend());
	return *animationDetails;
}

std::unordered_map<std::string, AnimationDetailsManager::EntityAnimationDetails> AnimationDetailsManager::loadInAnimationDetails(const std::string & fileName) const
{
	std::ifstream file(fileName);
	assert(file.is_open());
	std::unordered_map<std::string, EntityAnimationDetails> animationDetails;
	std::string line;

	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		std::string entityName, spriteSheetName, animationName, direction;
		int startID = 0, endID = 0;
		float frameTime = 0;
		bool repeatable, reversible;
		sf::Vector2f drawLocationSize;

		keyStream >> entityName >> spriteSheetName >> animationName >> direction >> startID >> endID >> frameTime >> repeatable 
			>> drawLocationSize.x >> drawLocationSize.y >> reversible;

		auto iter = animationDetails.find(entityName);
		if (iter != animationDetails.cend())
		{
			iter->second.m_entityAnimationDetails.emplace_back(std::move(spriteSheetName), std::move(animationName), 
				std::move(direction), startID, endID, frameTime, repeatable, drawLocationSize, reversible);
		}
		else
		{
			animationDetails.emplace(std::move(entityName), AnimationDetails(std::move(spriteSheetName), 
				std::move(animationName), std::move(direction), startID, endID, frameTime, repeatable, drawLocationSize, reversible));
		}
	}

	assert(!animationDetails.empty());
	return animationDetails;
}

const std::vector<AnimationDetailsManager::AnimationDetails>& AnimationDetailsManager::getEntityAnimationDetails(const std::string & entityName) const
{
	auto iter = m_animationDetails.find(entityName);
	assert(iter != m_animationDetails.cend());
	return iter->second.m_entityAnimationDetails;
}
