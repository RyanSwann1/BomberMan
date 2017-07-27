#pragma once

#include <SFML\Graphics.hpp>

namespace DebugOverlay
{
	void draw(sf::RenderWindow& window);
	void addRect(const sf::Vector2f& position);
}