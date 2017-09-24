#pragma once

#include <Game\Timer.h>
#include <string>
#include <SFML\Graphics.hpp>
#include <memory>

class StateManager;
enum class StateType;
class StateBase;
class FontManager;
class GUI
{
	enum class ComponentType
	{
		Button = 0,
		Text
	};

	class GUIComponent
	{
	public:
		GUIComponent(const std::string& name, ComponentType type);
		GUIComponent(GUIComponent& orig);
		virtual ~GUIComponent() {}

		virtual void draw(sf::RenderWindow& window) = 0;
		const std::string& getName() const;

		const ComponentType m_type;
		const std::string m_name;
	};

	class GUIText : public GUIComponent
	{
	public:
		GUIText(const std::string& text, const sf::Vector2f& position, const std::string& name, FontManager& fontManager, int characterSize = 30);
		GUIText(GUIText& orig);
		~GUIText();

		void draw(sf::RenderWindow& window);
		FontManager& m_fontManager;
		sf::Text m_text;
	};

	class GUIButton : public GUIComponent
	{
	public:
		GUIButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const std::string& name, FontManager& fontManager);
		GUIButton(GUIButton& orig);
		~GUIButton();

		void draw(sf::RenderWindow& window);

		const sf::FloatRect m_AABB;
		FontManager& m_fontManager;
		sf::RectangleShape m_button;
		sf::Text m_text;
	};

public:
	GUI(StateBase& owner, const StateManager& stateManager);

	void addButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const std::string& name);
	void addText(const sf::Vector2f& position, const std::string& text, std::string&& name, int characterSize);
	void updateText(const std::string& newText, const std::string& name);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	
private:
	const StateManager& m_stateManager;
	StateBase& m_owner;
	FontManager& m_fontManager;
	Timer m_activateButtonTimer;
	std::vector<std::unique_ptr<GUIComponent>> m_guiComponents;
	std::vector<std::string> m_buttonQueue;

	void handleButtonQueue();
	void handleButtonInteraction();
};