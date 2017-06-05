#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class Window
{
	friend class Game;
public:
	Window(const std::string& name, const sf::Vector2i& size);
		
	sf::RenderWindow& getWindow();
	bool isOpen() const;
	void draw(const sf::Drawable& drawable);

private:
	sf::RenderWindow m_window;

	void update();
	void display();
};