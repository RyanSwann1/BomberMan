#pragma once

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
		Animation(const std::string& tileSheetName, const std::string& animationName, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);

		const std::string& getName() const;
		bool isFinished() const;
		sf::IntRect getDrawLocation(const TileSheet& tileSheet) const;
		virtual void update(float deltaTime) = 0;
		void reset();

	protected:
		const std::string m_tileSheetName;
		const int m_startID;
		const int m_endID;
		const float m_frameTime;
		int m_currentID;
		float m_elaspedTime;
		bool m_animationFinished;

	private:
		const std::string m_animationName;
		const bool m_repeatable;
		const sf::Vector2f m_drawLocationSize;
		const bool m_reversible;
		sf::IntRect m_frame;
	};

	class AnimationHorizontal : public Animation
	{
	public:
		AnimationHorizontal(const std::string& tileSheetName, const std::string& animationName, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);

		void update(float deltaTime) override;
	};

	class AnimationVertical : public Animation
	{
	public:
		AnimationVertical(const std::string& tileSheetName, const std::string& animationName, int startID, int endID,
			float frameTime, bool repeatable, const sf::Vector2f& drawLocationSize, bool reversible);
	
		void update(float deltaTime) override;
	};

public:
	AnimationPlayer(const std::string& entityName);
	~AnimationPlayer();

	const Animation& getCurrentAnimation() const;
	const TileSheet& getTileSheet() const;

	void play(Direction movementDirection);
	void play(const std::string& animationName);
	void update(float deltaTime);
	void draw(const sf::Vector2f& entityPosition, sf::RenderWindow& window);
	
private:
	std::unordered_map<std::string, std::unique_ptr<Animation>> m_animations;
	sf::Sprite m_sprite;
	const TileSheet* m_tileSheet;
	Animation* m_currentAnimation;
};