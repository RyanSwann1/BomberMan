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
	: m_name(name),
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
	: m_name(orig.m_name),
	m_AABB(orig.m_AABB),
	m_fontManager(orig.m_fontManager),
	m_button(orig.m_button),
	m_text(orig.m_text)
{
	m_text.setFont(m_fontManager.getResource("arial"));
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

//GUILayout
GUIManager::GUIBase::GUIBase(FontManager & fontManager)
	: m_buttons()
{}

void GUIManager::GUIBase::update(sf::RenderWindow& window)
{
	if(m_buttons.empty() || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return;
	}

	const auto& mousePosition = sf::Mouse::getPosition(window);
	const sf::FloatRect mouseRect(sf::Vector2f(mousePosition.x, mousePosition.y), sf::Vector2f(16, 16));

	for (const auto& button : m_buttons)
	{
		if (mouseRect.intersects(button.getAABB()))
		{
			activateButton(button.getName());
			break;
		}
	}
}

void GUIManager::GUIBase::draw(sf::RenderWindow & window)
{
	for (auto& button : m_buttons)
	{
		button.draw(window);
	}
}

void GUIManager::GUIBase::addButton(const sf::Vector2f & position, const sf::Vector2f& size, const std::string & text, FontManager & fontManager, ButtonName name)
{
	assert(std::find_if(m_buttons.cbegin(), m_buttons.cend(), [name](const auto& button) { return button.getName() == name; }) == m_buttons.cend());
	m_buttons.emplace_back(position, size, text, fontManager, name);
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
			break;
		}
		case ButtonName::StartGame :
		{
			auto& stateManager = StateManagerLocator::getStateManager();
			stateManager.switchToAndRemoveState(StateType::Game, StateType::MainMenu);
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
			break;
		}
		case ButtonName::Resume :
		{
			auto& gameEventMessenger = GameEventMessengerLocator::getGameEventMessenger();
			gameEventMessenger.broadcast(GameEvent::Unpause);
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
	addButton(sf::Vector2f(150, 50), sf::Vector2f(100, 75), "Play", fontManager, ButtonName::StartGame);
	addButton(sf::Vector2f(150, 150), sf::Vector2f(100, 75), "Exit", fontManager, ButtonName::Exit);
}

//GUIGame
GUIManager::GUIGame::GUIGame(FontManager & fontManager)
	: GUIBase(fontManager)
{
	addButton(sf::Vector2f(250, 300), sf::Vector2f(50, 20), "Pause", fontManager, ButtonName::Pause);
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
	if (iter != m_guiLayouts.cend())
	{
		m_currentGUI = iter->second.get();
	}
	else
	{
		m_currentGUI = nullptr;
	}
}