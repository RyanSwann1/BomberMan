#include <States\StatePauseMenu.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>

StatePauseMenu::StatePauseMenu(StateManager & stateManager, StateType type)
	: StateBase(stateManager, type)
{
}