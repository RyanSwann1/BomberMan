#include "DebugOverlay.h"
#include <Locators\LevelManagerLocator.h>
#include <Managers\LevelManager.h>

std::vector<sf::RectangleShape> m_rects;

void DebugOverlay::draw(sf::RenderWindow & window)
{
	for (auto& rect : m_rects)
	{
		window.draw(rect);
	}
}

void DebugOverlay::addRect(const sf::Vector2f & position)
{
	const int tileSize = LevelManagerLocator::getLevelManager().getCurrentLevel()->getTileSize();
	sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
	rect.setPosition(sf::Vector2f(position.x * tileSize, position.y * tileSize));
	rect.setFillColor(sf::Color::Red);
	m_rects.push_back(rect);
}