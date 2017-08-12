#include "GUIManager.h"
#include <Managers\FontManager.h>
#include <Game\MessageHandler.h>
#include <Locators\GameEventMessengerLocator.h>
#include <Managers\LevelManager.h>
#include <Locators\LevelManagerLocator.h>
#include <Managers\StateManager.h>
#include <assert.h>

//GUIButton
GUIManager::GUIButton::GUIButton(const sf::Vector2f & position, const sf::Vector2f& size, const std::string & text, FontManager & fontManager, ButtonName name)
	: GUIComponent(ComponentType::Button),
	m_name(name),
	m_AABB(position, size),
	m_fontManager(fontManager)
{
	m_text.setFont(fontManager.getResource("arial"));
	m_text.setString(text);
	m_text.setPosition(position);
	m_text.setCharacterSize(15);
	m_text.setFillColor(sf::Color::Black);

	m_button.setFillColor(sf::Color::White);
	m_button.setPosition(position);
	m_button.setSize(size);
}

GUIManager::GUIButton::~GUIButton()
{
	m_fontManager.releaseResource("arial");
}

GUIManager::GUIButton::GUIButton(GUIButton & orig)
	: GUIComponent(orig.m_type),
	m_name(orig.m_name),
	m_AABB(orig.m_AABB),
	m_fontManager(orig.m_fontManager),
	m_button(orig.m_button),
	m_text(orig.m_text)
{
	m_text.setFont(m_fontManager.getResource("arial"));
}

void GUIManager::GUIText::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

const sf::FloatRect & GUIManager::GUIButton::getAABB() const
{
	return m_AABB;
}

GUIManager::ButtonName GUIManager::GUIButton::getName() const
{
	return m_name;
}

void GUIManager::GUIButton::draw(sf::RenderWindow & window)
{
	window.draw(m_button);
	window.draw(m_text);
}

GUIManager::GUIText::GUIText(const std::string& text, const sf::Vector2f& position, FontManager& fontManager, int characterSize)
	: GUIComponent(ComponentType::Text),
	m_fontManager(fontManager),
	m_text(text, fontManager.getResource("arial"), characterSize)
{
	m_text.setPosition(position);
	m_text.setFillColor(sf::Color::White);
}

GUIManager::GUIText::GUIText(GUIText & orig)
	: GUIComponent(orig.m_type),
	m_fontManager(orig.m_fontManager)
{
	m_text.setPosition(orig.m_text.getPosition());
	m_text.setCharacterSize(orig.m_text.getCharacterSize());
	m_text.setString(orig.m_text.getString());
	m_text.setFont(m_fontManager.getResource("arial"));
}

GUIManager::GUIText::~GUIText()
{
	m_fontManager.releaseResource("arial");
}

//GUILayout
GUIManager::GUIBase::GUIBase(FontManager & fontManager)
	: m_components()
{}

void GUIManager::GUIBase::update(sf::RenderWindow& window)
{
	if(m_components.empty() || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return;
	}

	const auto& mousePosition = sf::Mouse::getPosition(window);
	const sf::FloatRect mouseRect(sf::Vector2f(mousePosition.x, mousePosition.y), sf::Vector2f(16, 16));

	for (const auto& component : m_components)
	{
		if (component->m_type != ComponentType::Button)
		{
			continue;
		}

		if (mouseRect.intersects(static_cast<GUIButton*>(component.get())->getAABB()))
		{
			activateButton(static_cast<GUIButton*>(component.get())->getName());
			break;
		}
	}
}

void GUIManager::GUIBase::draw(sf::RenderWindow & window)
{
	for (auto& component : m_components)
	{
		component->draw(window);
	}
}

void GUIManager::GUIBase::addButton(const sf::Vector2f & position, const sf::Vector2f& size, const std::string & text, FontManager & fontManager, ButtonName name)
{
	//m_components.insert(position, size, text, fontManager, name);
	m_components.emplace_back(std::make_unique<GUIButton>(position, size, text, fontManager, name));
}

void GUIManager::GUIBase::addText(const sf::Vector2f & position, const std::string & text, FontManager & fontManager, int characterSize)
{
	//GUIText(const std::string& text, const sf::Vector2f& position, FontManager& fontManager, int characterSize = 30);
	m_components.emplace_back(std::make_unique<GUIText>(text, position, fontManager, characterSize));
}

void GUIManager::GUIBase::activateButton(ButtonName name)
{
	switch (name)
	{
		case ButtonName::Exit :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::CloseWindow);
			break;
		}
		case ButtonName::MainMenu :
		{
			auto& stateManager = StateManagerLocator::getStateManager();
			stateManager.switchToAndRemoveState(StateType::MainMenu, StateType::Game);
			stateManager.removeState(StateType::PauseMenu);
			break;
		}
		case ButtonName::StartGame :
		{
			auto& stateManager = StateManagerLocator::getStateManager();
			stateManager.switchToAndRemoveState(StateType::Game, StateType::MainMenu);
			stateManager.createState(StateType::PauseMenu);
			break;
		}
		case ButtonName::NextLevel :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::ChangeToNextLevel);
			break;
		}
		case ButtonName::Pause :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::Pause);
			StateManagerLocator::getStateManager().switchToState(StateType::PauseMenu);
			break;
		}
		case ButtonName::Resume :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::Unpause);
			StateManagerLocator::getStateManager().switchToState(StateType::Game);
			break;
		}
		case ButtonName::Retry :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::ReloadCurrentLevel);

			auto& stateManager = StateManagerLocator::getStateManager();
			stateManager.switchToAndRemoveState(StateType::Game, StateType::RoundFailed);
			break;
		}
	}
}

//GUIPauseMenu
GUIManager::GUIPauseMenu::GUIPauseMenu(FontManager & fontManager)
	: GUIBase(fontManager)
{
	addButton(sf::Vector2f(175, 50), sf::Vector2f(100, 75), "Resume", fontManager, ButtonName::Resume);
	addButton(sf::Vector2f(50, 50), sf::Vector2f(100, 75), "Main Menu", fontManager, ButtonName::MainMenu);
}

//GUIMainMenu
GUIManager::GUIMainMenu::GUIMainMenu(FontManager & fontManager)
	: GUIBase(fontManager)
{

	addText(sf::Vector2f(75, 20), "Bomberman", fontManager, 30);
	addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Play", fontManager, ButtonName::StartGame);
	addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "Exit", fontManager, ButtonName::Exit);
}

//GUIRetryMenu
GUIManager::GUIRetryMenu::GUIRetryMenu(FontManager & fontManager)
	: GUIBase(fontManager)
{
	addText(sf::Vector2f(75, 20), "You Died!", fontManager, 30);
	addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Retry", fontManager, ButtonName::Retry);
	addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "MainMenu", fontManager, ButtonName::MainMenu);
}

//GUIGame
GUIManager::GUIGame::GUIGame(FontManager & fontManager)
	: GUIBase(fontManager)
{
	addButton(sf::Vector2f(250, 300), sf::Vector2f(50, 20), "Pause", fontManager, ButtonName::Pause);
}

GUIManager::GUIWinMenu::GUIWinMenu(FontManager & fontManager)
	: GUIBase(fontManager)
{

}

//GUIManager
GUIManager::GUIManager(MessageHandler<GameEvent>& gameEventMessenger, FontManager& fontManager)
	: m_guiLayouts(),
	m_currentGUI(nullptr)
{
	gameEventMessenger.subscribe(std::bind(&GUIManager::switchGUIToCurrentState, this), "GUIManager", GameEvent::NewState);

	//Create new GUI's here
	addNewGUI<GUIMainMenu>(fontManager, StateType::MainMenu);
	addNewGUI<GUIPauseMenu>(fontManager, StateType::PauseMenu);
	addNewGUI<GUIGame>(fontManager, StateType::Game);
	addNewGUI<GUIRetryMenu>(fontManager, StateType::RoundFailed);
}

GUIManager::~GUIManager()
{
	auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
	gameEventMessenger.unsubscribe("GUIManager", GameEvent::NewState);
}

void GUIManager::update(sf::RenderWindow& window)
{
	if (m_currentGUI)
	{
		m_currentGUI->update(window);
	}
}

void GUIManager::draw(sf::RenderWindow & window)
{
	if (m_currentGUI)
	{
		m_currentGUI->draw(window);
	}
}

void GUIManager::switchGUIToCurrentState()
{
	const auto currentStateType = StateManagerLocator::getStateManager().getCurrentStateType();
	auto iter = m_guiLayouts.find(currentStateType);
	(iter != m_guiLayouts.cend() ? m_currentGUI = iter->second.get() : m_currentGUI = nullptr);
}
