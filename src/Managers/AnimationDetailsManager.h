#pragma once

#include <SFML\Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class AnimationDetailsManager
{
	class AnimationDetails
	{
	public:
		AnimationDetails(std::string&& tileSheetName, std::string&& animationName, std::string&& animationDirection, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible)
			: m_tileSheetName(std::move(tileSheetName)),
			m_animationName(std::move(animationName)),
			m_animationDirection(std::move(animationDirection)),
			m_startID(startID),
			m_endID(endID),
			m_repeatable(repeatable),
			m_drawLocationSize(drawLocationSize),
			m_reversible(reversible),
			m_frameTime(frameTime)
		{}
	
		const std::string m_tileSheetName;
		const std::string m_animationName;
		const std::string m_animationDirection;
		const int m_startID;
		const int m_endID;
		const bool m_repeatable;
		const sf::Vector2f m_drawLocationSize;
		const bool m_reversible;
		const float m_frameTime;
	};

	class EntityAnimationDetails
	{
	public:
		EntityAnimationDetails(const AnimationDetails& animationDetails)
		{
			m_entityAnimationDetails.push_back(animationDetails);
		}

		std::vector<AnimationDetails> m_entityAnimationDetails;
	};

public:
	AnimationDetailsManager();

	const std::string& getTileSheetName(const std::string& entityName) const;
	const AnimationDetails& getAnimationDetails(const std::string& entityName, const std::string& animationName) const;
	const std::vector<AnimationDetails>& getEntityAnimationDetails(const std::string& entityName) const;

private:
	const std::unordered_map<std::string, EntityAnimationDetails> m_animationDetails;

	std::unordered_map<std::string, EntityAnimationDetails> loadInAnimationDetails(const std::string& fileName) const;
};
