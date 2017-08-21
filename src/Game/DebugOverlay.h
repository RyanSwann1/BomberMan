#pragma once

#include <string>
#include <SFML\Graphics.hpp>

namespace DebugOverlay
{
	void draw(sf::RenderWindow& window);
	void addRect(const sf::Vector2f& position);
	void printMessage(const std::string& message);
}