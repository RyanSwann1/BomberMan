#include "Window.h"
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Game\GameEvent.h>

Window::Window(MessageHandler<GameEvent>& gameEventMessenger, const std::string & name, const sf::Vector2i & size)
	: m_window(sf::VideoMode(size.x, size.y), name, sf::Style::Default)
{
	m_window.setFramerateLimit(60);
	gameEventMessenger.subscribe(std::bind(&Window::closeWindow, this), "Window", GameEvent::CloseWindow);
}

Window::~Window()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("Window", GameEvent::CloseWindow);
}

sf::RenderWindow & Window::getRenderWindow()
{
	return m_window;
}

bool Window::isOpen() const
{
	return m_window.isOpen();
}

void Window::display()
{
	m_window.display();
	m_window.clear(sf::Color::Black);
}

void Window::closeWindow()
{
	m_window.close();
}

void Window::draw(const sf::Drawable & drawable)
{
	m_window.draw(drawable);
}

void Window::update()
{
	sf::Event sfmlEvent;
	while (m_window.pollEvent(sfmlEvent))
	{
		switch (sfmlEvent.type)
		{
		case sf::Event::Closed :
		{
			m_window.close();
			break;
		}
		}
	}
}
