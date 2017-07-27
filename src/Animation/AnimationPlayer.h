#pragma once

#include <Game\Timer.h>
#include <Animation\AnimationName.h>
#include <Entities\Direction.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>

enum class Direction;
class TileSheet;
class AnimationPlayer
{
	class Animation
	{
	public:
		Animation(const std::string& tileSheetName, AnimationName name, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);

		AnimationName getName() const;
		bool isFinished() const;
		sf::IntRect getDrawLocation(const TileSheet& tileSheet) const;
		virtual void update(float deltaTime);
		void reset();

	protected:
		const std::string m_tileSheetName;
		const int m_startFrame;
		const int m_endFrame;
		//const float m_frameTime;
		const bool m_animationReversible;
		int m_currentFrame;
		//float m_elaspedTime;
		bool m_animationFinished;
		bool m_proceedToNextFrame;
		bool m_animationPlaying;
		bool m_reverseAnimation;
		Timer m_frameTimer;
	private:
		const AnimationName m_name;
		const bool m_animationRepeatable;
		const sf::Vector2f m_drawLocationSize;
	};

	class AnimationHorizontal : public Animation
	{
	public:
		AnimationHorizontal(const std::string& tileSheetName, AnimationName name, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);

		void update(float deltaTime) override;
	};

	class AnimationVertical : public Animation
	{
	public:
		AnimationVertical(const std::string& tileSheetName, AnimationName name, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);
	
		void update(float deltaTime) override;
	};


public:
	AnimationPlayer(const std::string& entityName);
	~AnimationPlayer();

	const Animation& getCurrentAnimation(AnimationName name) const;
	const TileSheet& getTileSheet() const;

	void play(AnimationName name, Direction moveDirection = Direction::None);
	void update(float deltaTime);
	void draw(const sf::Vector2f& entityPosition, sf::RenderWindow& window);
	
private:
	std::unordered_map<AnimationName, std::unique_ptr<Animation>> m_animations;
	sf::Sprite m_sprite;
	const TileSheet* m_tileSheet;
	Animation* m_currentAnimation;

	void switchToAnimation(AnimationName name);
};