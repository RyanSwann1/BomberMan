#pragma once

#include "StateBase.h"
#include <Managers\LevelManager.h>
#include <Managers\TileSheetManager.h>
#include <Managers\EntityManager.h>
#include <Managers\AnimationDetailsManager.h>
#include <Managers\GameManager.h>

class StateGame : public StateBase
{
public:
	StateGame(StateManager& stateManager, StateType stateType);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:
	AnimationDetailsManager m_animationDetailsManager;
	TileSheetManager m_tileSheetManager;
	EntityManager m_entityManager;
	LevelManager m_levelManager;
	GameManager m_gameManager;
};