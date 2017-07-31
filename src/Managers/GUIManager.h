#pragma once

#include <Locators\StateManagerLocator.h>
#include <Game\GameEvent.h>
#include <States\StateType.h>
#include <vector>
#include <SFML\Graphics.hpp>
#include <memory>
#include <unordered_map>

template <class GameEvent>
class MessageHandler;
class FontManager;
class GUIManager : private StateManagerLocator
{
	enum class ButtonName
	{
		StartGame = 0,
		Exit,
		NextLevel,
		MainMenu,
		Resume,
		Pause
	};

	class GUIButton
	{
	public:
		GUIButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, FontManager& fontManager, ButtonName name);
		~GUIButton();
		GUIButton(GUIButton& orig);
	
		const sf::FloatRect& getAABB() const;
		ButtonName getName() const;
		void draw(sf::RenderWindow& window);

	private:
		const ButtonName m_name;
		const sf::FloatRect m_AABB;
		FontManager& m_fontManager;
		sf::RectangleShape m_button;
		sf::Text m_text;
	};

	class GUIBase
	{
	public:
		GUIBase(FontManager& fontManager);

		void update(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
		
	protected:
		void addButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text,  FontManager& fontManager, ButtonName name);
		std::vector<GUIButton> m_buttons;

	private:
		void activateButton(ButtonName name);
	};

	class GUIPauseMenu : public GUIBase
	{
	public:
		GUIPauseMenu(FontManager& fontManager);
	};

	class GUIMainMenu : public GUIBase
	{
	public:
		GUIMainMenu(FontManager& fontManager);
	};

	class GUIGame : public GUIBase
	{
	public:
		GUIGame(FontManager& fontManager);
	};

public:
	GUIManager(MessageHandler<GameEvent>& gameEventMessenger, FontManager& fontManager);
	~GUIManager();

	void update(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<StateType, std::unique_ptr<GUIBase>> m_guiLayouts;
	GUIBase* m_currentGUI;

	void switchGUIToCurrentState();

	template <class GUI>
	void addNewGUI(FontManager& fontManager, StateType stateType)
	{
		assert(m_guiLayouts.find(stateType) == m_guiLayouts.cend());
		m_guiLayouts.emplace(stateType, std::make_unique<GUI>(fontManager));
	}
};