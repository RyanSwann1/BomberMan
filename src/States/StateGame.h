#pragma once

#include "StateBase.h"
#include <Managers\LevelManager.h>
#include <Managers\TileSheetManager.h>
#include <Managers\EntityManager.h>

class StateGame : public StateBase
{
public:
	StateGame(StateType stateType);
	virtual ~StateGame();

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:
	TileSheetManager m_tileSheetManager;
	EntityManager m_entityManager;
	LevelManager m_levelManager;
};