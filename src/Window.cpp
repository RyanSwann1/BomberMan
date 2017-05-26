#include "Window.h"

Window::Window(const std::string & name, const sf::Vector2f & size)
	: m_window(sf::VideoMode(size.x, size.y), name, sf::Style::Default)
{
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

void Window::draw(sf::Drawable & drawable)
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
