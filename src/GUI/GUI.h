#pragma once

#include <GUI\GUIButtonName.h>
#include <Game\Timer.h>
#include <string>
#include <SFML\Graphics.hpp>
#include <memory>

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
		GUIButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, FontManager& fontManager, GUIButtonName name);
		GUIButton(GUIButton& orig);
		~GUIButton();
		

		const sf::FloatRect& getAABB() const;
		GUIButtonName getName() const;
		void draw(sf::RenderWindow& window);

	private:
		const GUIButtonName m_name;
		const sf::FloatRect m_AABB;
		FontManager& m_fontManager;
		sf::RectangleShape m_button;
		sf::Text m_text;
	};

public:
	GUI(StateBase& stateBase);

	void addButton(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, GUIButtonName name);
	void addText(const sf::Vector2f& position, const std::string& text, int characterSize);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	
private:
	StateBase& m_stateBase;
	FontManager& m_fontManager;
	Timer m_activateButtonTimer;
	std::vector<std::unique_ptr<GUIComponent>> m_components;
	std::vector<GUIButtonName> m_buttonQueue;

	void handleButtonQueue();
	void handleButtonInteraction();
};