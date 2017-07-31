#pragma once

#include <Game\GameEvent.h>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

template <class GameEvent>
class MessageHandler;
class Window
{
public:
	Window(MessageHandler<GameEvent>& gameEventMessenger, const std::string& name, const sf::Vector2i& size);
	~Window();

	sf::RenderWindow& getRenderWindow();
	bool isOpen() const;
	void draw(const sf::Drawable& drawable);
	void update();
	void display();

private:
	sf::RenderWindow m_window;
	
	void closeWindow();
};