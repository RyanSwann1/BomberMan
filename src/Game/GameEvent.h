#pragma once

enum class GameEvent
{
	ClearMap = 0,
	ReloadCurrentLevel,
	ChangeToNextLevel,
	WinGame,
	EnemyDeath,
	PlayerDeath
};