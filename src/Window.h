#pragma once

#include <Managers\EventManager.h>
#include <Game\GameEvent.h>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class StateManager;
template <class GameEvent>
class MessageHandler;
class Window
{
public:
	Window(const StateManager& stateManager, MessageHandler<GameEvent>& gameEventMessenger, const std::string& name, const sf::Vector2i& size);
	~Window();

	sf::RenderWindow& getRenderWindow();
	bool isOpen() const;
	void draw(const sf::Drawable& drawable);
	void update();
	void display();

private:
	sf::RenderWindow m_window;
	EventManager m_eventManager;
	
	void closeWindow();
};