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
		Pause,
		Retry
	};

	enum class ComponentType
	{
		Button = 0,
		Text
	};

	class GUIComponent
	{
	public:
		GUIComponent(ComponentType type)
			: m_type(type)
		{}

		virtual ~GUIComponent() {}
		virtual void draw(sf::RenderWindow& window) = 0;
		const ComponentType m_type;
	};

	class GUIText : public GUIComponent
	{
	public:
		GUIText(const std::string& text, const sf::Vector2f& position, FontManager& fontManager, int characterSize = 30);
		GUIText(GUIText& orig);
		~GUIText();

		void draw(sf::RenderWindow& window);
	private:
		sf::Text m_text;
		FontManager& m_fontManager;
	};

	class GUIButton : public GUIComponent
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
		void addText(const sf::Vector2f& position, const std::string& text, FontManager& fontManager, int characterSize);

		std::vector<std::unique_ptr<GUIComponent>> m_components;

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

	class GUIWinMenu : public GUIBase
	{
	public:
		GUIWinMenu(FontManager& fontManager);
	};

	class GUIRetryMenu : public GUIBase
	{
	public:
		GUIRetryMenu(FontManager& fontManager);
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