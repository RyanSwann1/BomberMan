#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class Window
{
public:
	Window(const std::string& name, const sf::Vector2f& size);
	
	bool isOpen() const;
	void display();
	void draw(sf::Drawable& drawable);
	void update();

private:
	sf::RenderWindow m_window;
};