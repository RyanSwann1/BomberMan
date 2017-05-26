
#include "Game\Game.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>



int main()
{
	Game game;
	while (game.getWindow().isOpen())
	{
		game.update();
		game.draw();
		game.lateUpdate();
	}

	return 0;
}