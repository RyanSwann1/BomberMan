#pragma once

enum class GameEvent
{
	ClearMap = 0,
	ReloadCurrentLevel,
	PlayerDeath,
	ChangeToNextLevel,
	WinGame,
	EnemyDeath,
	EnemyAggressive,
	NewState,
	CloseWindow,
	Pause,
	Unpause,
	EnemySpawned,
	StartedNewLevel
};