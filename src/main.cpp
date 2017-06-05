#include "Game\Game.h"

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