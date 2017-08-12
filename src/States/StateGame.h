#pragma once

#include "StateBase.h"
#include <Managers\LevelManager.h>
#include <Managers\TileSheetManager.h>
#include <Managers\EntityManager.h>
#include <Managers\AnimationDetailsManager.h>
#include <Managers\GameManager.h>
#include <Utilities\AnimationNameConverter.h>
#include <Game\EntityMessenger.h>

class StateGame : public StateBase
{
public:
	StateGame(StateManager& stateManager, StateType stateType);
	~StateGame();

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:
	GameManager m_gameManager;
	EntityMessenger m_entityMessenger;
	AnimationNameConverter m_animationNameConverter;
	AnimationDetailsManager m_animationDetailsManager;
	TileSheetManager m_tileSheetManager;
	EntityManager m_entityManager;
	LevelManager m_levelManager;
	
	bool m_gamePaused;

	void pauseGame();
	void unpauseGame();
	void onNewLevel();
};