#pragma once

#include "StateBase.h"
#include <Managers\LevelManager.h>
#include <Managers\TileSheetManager.h>

class StateGame : public StateBase
{
public:
	StateGame(StateType stateType);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:
	TileSheetManager m_tileSheetManager;
	LevelManager m_levelManager;
};