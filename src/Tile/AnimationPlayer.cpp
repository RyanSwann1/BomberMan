#include "AnimationPlayer.h"
#include <Locators\TileSheetManagerLocator.h>
#include <Managers\TileSheetManager.h>
#include <Locators\AnimationDetailsManagerLocator.h>
#include <Managers\AnimationDetailsManager.h>
#include <Entities\Direction.h>
#include "Tile.h"
#include <assert.h>

//AnimationHorizontal
AnimationPlayer::AnimationHorizontal::AnimationHorizontal(const std::string & tileSheetName, const std::string & animationName, int startID, int endID, float frameTime, bool repeatable, const sf::Vector2f & drawLocationSize, bool reversible)
	: Animation(tileSheetName, animationName, startID, endID, frameTime, repeatable, drawLocationSize, reversible)
{}

void AnimationPlayer::AnimationHorizontal::update(float deltaTime)
{
	Animation::update(deltaTime);
	if (!m_proceedToNextFrame)
	{
		return;
	}

	m_proceedToNextFrame = false;
	if (m_reverseAnimation)
	{
		if (m_currentFrame > m_startFrame)
		{
			--m_currentFrame;
		}
		else if (m_currentFrame < m_startFrame)
		{
			++m_currentFrame;
		}
	}	
	else
	{
		if (m_currentFrame > m_endFrame)
		{
			--m_currentFrame;
		}
		else if (m_currentFrame < m_endFrame)
		{
			++m_currentFrame;
		}
	}
}

//AnimationVertical
AnimationPlayer::AnimationVertical::AnimationVertical(const std::string & tileSheetName, const std::string & animationName, int startID, int endID, float frameTime, bool repeatable, const sf::Vector2f & drawLocationSize, bool reversible)
	: Animation(tileSheetName, animationName, startID, endID, frameTime, repeatable, drawLocationSize, reversible)
{

}

void AnimationPlayer::AnimationVertical::update(float deltaTime)
{
	Animation::update(deltaTime);
	if (!m_proceedToNextFrame)
	{
		return;
	}

	m_proceedToNextFrame = false;
	const auto& tileSheet = TileSheetManagerLocator::getTileSheetManager().getTileSheet(m_tileSheetName);
	if (m_reverseAnimation)
	{
		if (m_currentFrame < m_startFrame)
		{
			m_currentFrame += tileSheet.m_columns;
		}
		else if (m_currentFrame > m_startFrame)
		{
			m_currentFrame -= tileSheet.m_columns;
		}
	}
	else
	{
		if (m_currentFrame < m_endFrame)
		{
			m_currentFrame += tileSheet.m_columns;
		}
		else if (m_currentFrame > m_endFrame)
		{
			m_currentFrame -= tileSheet.m_columns;
		}
	}
}

AnimationPlayer::Animation::Animation(const std::string & tileSheetName, const std::string & animationName,
	int startID, int endID, float frameTime, bool repeatable, const sf::Vector2f & drawLocationSize, bool reversible)
	: m_tileSheetName(tileSheetName),
	m_startFrame(startID),
	m_endFrame(endID),
	m_frameTime(frameTime),
	m_currentFrame(m_startFrame),
	m_elaspedTime(0),
	m_animationFinished(false),
	m_animationName(std::move(animationName)),
	m_animationRepeatable(repeatable),
	m_drawLocationSize(drawLocationSize),
	m_animationReversible(reversible),
	m_proceedToNextFrame(false),
	m_animationPlaying(true),
	m_reverseAnimation(false)
{}

const std::string & AnimationPlayer::Animation::getName() const
{
	return m_animationName;
}

bool AnimationPlayer::Animation::isFinished() const
{
	return !m_animationPlaying;
}

sf::IntRect AnimationPlayer::Animation::getDrawLocation(const TileSheet & tileSheet) const
{
	auto& drawLocation = tileSheet.getTileLocation(m_currentFrame);
	drawLocation.height *= m_drawLocationSize.x;
	drawLocation.width *= m_drawLocationSize.y;

	return drawLocation;
}

void AnimationPlayer::Animation::update(float deltaTime)
{
	if (!m_animationPlaying)
	{
		return;
	}

	m_elaspedTime += deltaTime;
	if (m_elaspedTime < m_frameTime)
	{
		return;
	}

	if (m_currentFrame == m_endFrame)
	{
		if (m_animationReversible)
		{
			m_reverseAnimation = true;
		}
		else
		{
			reset();
			return;
		}
	}

	if (m_reverseAnimation && m_currentFrame == m_startFrame)
	{
		reset();
		return;
	}

	m_proceedToNextFrame = true;
	m_elaspedTime = 0;
}

void AnimationPlayer::Animation::reset()
{
	m_animationFinished = false;
	m_proceedToNextFrame = false;
	m_animationPlaying = m_animationRepeatable;
	if (m_animationPlaying)
	{
		m_currentFrame = m_startFrame;
	}
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
		if (animationDetails.m_animationDirection == "Horizontal")
		{
			m_animations.emplace(animationDetails.m_animationName,
				std::make_unique<AnimationHorizontal>(animationDetails.m_tileSheetName, animationDetails.m_animationName, animationDetails.m_startID,
					animationDetails.m_endID, animationDetails.m_frameTime, animationDetails.m_repeatable, animationDetails.m_drawLocationSize, animationDetails.m_reversible));
		}
		else if (animationDetails.m_animationDirection == "Vertical")
		{
			m_animations.emplace(animationDetails.m_animationName,
				std::make_unique<AnimationVertical>(animationDetails.m_tileSheetName, animationDetails.m_animationName, animationDetails.m_startID,
					animationDetails.m_endID, animationDetails.m_frameTime, animationDetails.m_repeatable, animationDetails.m_drawLocationSize, animationDetails.m_reversible));
		}
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

void AnimationPlayer::play(const std::string& animationName, Direction moveDirection)
{
	if (moveDirection == Direction::Left)
	{
		m_sprite.setScale(-1, 1);
	}
	
	switchToAnimation(animationName);
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
	//Hack.
	//Not sure of work around to get rendered properly
	if (m_sprite.getScale().x - 1)
	{
		m_sprite.setPosition(m_sprite.getPosition().x + 16, m_sprite.getPosition().y);
	}
	m_sprite.setTextureRect(m_currentAnimation->getDrawLocation(*m_tileSheet));
	window.draw(m_sprite);
	m_sprite.setScale(1, 1);
}

void AnimationPlayer::switchToAnimation(const std::string & animationName)
{
	if (m_currentAnimation)
	{
		if (m_currentAnimation->getName() == animationName)
		{
			return;
		}
		m_currentAnimation->reset();
	}

	auto iter = m_animations.find(animationName);
	assert(iter != m_animations.cend());
	m_currentAnimation = iter->second.get();
}

const AnimationPlayer::Animation& AnimationPlayer::getCurrentAnimation(const std::string& animationName) const
{
	assert(m_currentAnimation);
	assert(m_currentAnimation->getName() == animationName);
	return *m_currentAnimation;
}

const TileSheet & AnimationPlayer::getTileSheet() const
{
	assert(m_tileSheet);
	return *m_tileSheet;
}