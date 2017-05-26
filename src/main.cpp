
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

int main()
{
	sf::RenderWindow window (sf::VideoMode(300, 300), "SFML_WINDOW", sf::Style::Default);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
			{
				window.close();
			}
			}

			window.clear(sf::Color::Black);
			window.display();
		}
	}

	return 0;
}