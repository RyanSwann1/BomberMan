#include "AnimationPlayer.h"
#include <Locators\TileSheetManagerLocator.h>
#include <Managers\TileSheetManager.h>
#include <Locators\AnimationDetailsManagerLocator.h>
#include <Managers\AnimationDetailsManager.h>
#include "Tile.h"
#include <assert.h>

AnimationPlayer::Animation::Animation(const std::string & tileSheetName, const std::string & animationName, 
	int startID, int endID, float frameTime, bool repeatable, const sf::Vector2f & drawLocationSize, bool reversible)
	: m_tileSheetName(tileSheetName),
	m_animationName(animationName),
	m_startID(startID),
	m_endID(endID),
	m_frameTime(frameTime),
	m_repeatable(repeatable),
	m_drawLocationSize(drawLocationSize),
	m_reversible(reversible),
	m_frame(),
	m_currentID(m_startID),
	m_elaspedTime(0)
{
}

sf::IntRect AnimationPlayer::Animation::getDrawLocation(const TileSheet & tileSheet) const
{
	return tileSheet.getTileLocation(m_currentID);
}

void AnimationPlayer::Animation::update(float deltaTime)
{
	m_elaspedTime += deltaTime;
}

void AnimationPlayer::Animation::reset()
{
	m_currentID = m_startID;
	m_elaspedTime = 0;
}

//Animation Player
AnimationPlayer::AnimationPlayer(const std::string & entityName)
	: m_tileSheet(nullptr)
{
	//Not 100% sure about this yet.
	//The idea of not loading in the animation details when an entity spawns is good,
	//But the implementation is clunky at best.
	//Load in appropriate animations for entity
	const auto& animationDetailsManager = AnimationDetailsManagerLocator::getAnimationDetailsManager();
	for (const auto& animationDetails : animationDetailsManager.getEntityAnimationDetails(entityName))
	{
		m_animations.emplace(animationDetails.m_animationName,
			Animation(animationDetails.m_tileSheetName, animationDetails.m_animationName, animationDetails.m_startID,
				animationDetails.m_endID, animationDetails.m_frameTime, animationDetails.m_repeatable, animationDetails.m_drawLocationSize, animationDetails.m_reversible));
	}
	//Assign tile sheet
	const auto& tileSheetName = animationDetailsManager.getTileSheetName(entityName);
	m_tileSheet = &TileSheetManagerLocator::getTileSheetManager().getTileSheet(tileSheetName);
	m_sprite.setTexture(m_tileSheet->getTexture());
}

AnimationPlayer::~AnimationPlayer()
{
	m_tileSheet->releaseTileSheet();
}

void AnimationPlayer::play(const std::string& animationName)
{
	if (m_currentAnimation)
	{
		assert(m_currentAnimation->m_animationName != animationName);
		m_currentAnimation->reset();
	}

	auto iter = m_animations.find(animationName);
	assert(iter != m_animations.cend());
	m_currentAnimation = &iter->second;
}

void AnimationPlayer::update(float deltaTime)
{
	assert(m_currentAnimation);
	m_currentAnimation->update(deltaTime);
}

void AnimationPlayer::draw(const sf::Vector2f& entityPosition, sf::RenderWindow & window)
{
	m_sprite.setPosition(entityPosition);
	assert(m_currentAnimation);
	assert(m_tileSheet);
	m_sprite.setTextureRect(m_currentAnimation->getDrawLocation(*m_tileSheet));
	window.draw(m_sprite);
}

const AnimationPlayer::Animation& AnimationPlayer::getCurrentAnimation() const
{
	assert(m_currentAnimation);
	return *m_currentAnimation;
}
