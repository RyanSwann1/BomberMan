#pragma once

#include <Locators\StateManagerLocator.h>
#include <Game\GameEvent.h>
#include <States\StateType.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <memory>
#include <unordered_map>

//template <class GameEvent>
//class MessageHandler;
//class FontManager;
//class GUIManager : private StateManagerLocator
//{
//
//
//	class GUIPauseMenu : public GUIBase
//	{
//	public:
//		GUIPauseMenu(FontManager& fontManager);
//	};
//
//	class GUIMainMenu : public GUIBase
//	{
//	public:
//		GUIMainMenu(FontManager& fontManager);
//	};
//
//	class GUIGame : public GUIBase
//	{
//	public:
//		GUIGame(FontManager& fontManager);
//	};
//
//	class GUIWinMenu : public GUIBase
//	{
//	public:
//		GUIWinMenu(FontManager& fontManager);
//	};
//
//	class GUIRetryMenu : public GUIBase
//	{
//	public:
//		GUIRetryMenu(FontManager& fontManager);
//	};
//
//public:
//	GUIManager(MessageHandler<GameEvent>& gameEventMessenger, FontManager& fontManager);
//	~GUIManager();
//
//	void update(sf::RenderWindow& window);
//	void draw(sf::RenderWindow& window);
//
//private:
//	std::unordered_map<StateType, std::unique_ptr<GUIBase>> m_guiLayouts;
//	GUIBase* m_currentGUI;
//
//	void switchGUIToCurrentState();
//
//	template <class GUI>
//	void addNewGUI(FontManager& fontManager, StateType stateType)
//	{
//		assert(m_guiLayouts.find(stateType) == m_guiLayouts.cend());
//		m_guiLayouts.emplace(stateType, std::make_unique<GUI>(fontManager));
//	}
//};