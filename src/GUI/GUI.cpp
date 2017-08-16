#include <GUI\GUI.h>
#include <Managers\FontManager.h>
#include <Locators\FontManagerLocator.h>
#include <Game\Timer.h>
#include <Locators\WindowLocator.h>
#include <Window.h>
#include <States\StateBase.h>

//GUI Text
GUI::GUIText::GUIText(const std::string & text, const sf::Vector2f & position, FontManager & fontManager, int characterSize)
	: GUIComponent(ComponentType::Text),
	m_fontManager(fontManager),
	m_text(text, fontManager.getResource("arial"), characterSize)
{
	m_text.setPosition(position);
	m_text.setFillColor(sf::Color::White);
}

GUI::GUIText::GUIText(GUIText & orig)
	: GUIComponent(orig.m_type),
	m_fontManager(orig.m_fontManager)
{
	m_text.setPosition(orig.m_text.getPosition());
	m_text.setCharacterSize(orig.m_text.getCharacterSize());
	m_text.setString(orig.m_text.getString());
	m_text.setFont(m_fontManager.getResource("arial"));
}

GUI::GUIText::~GUIText()
{
	m_fontManager.releaseResource("arial");
}

void GUI::GUIText::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

//GUI Button
GUI::GUIButton::GUIButton(const sf::Vector2f & position, const sf::Vector2f & size, const std::string & text, FontManager & fontManager, GUIButtonName name)
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

GUI::GUIButton::GUIButton(GUIButton & orig)
	: GUIComponent(orig.m_type),
	m_name(orig.m_name),
	m_AABB(orig.m_AABB),
	m_fontManager(orig.m_fontManager),
	m_button(orig.m_button),
	m_text(orig.m_text)
{
	m_text.setFont(m_fontManager.getResource("arial"));
}

GUI::GUIButton::~GUIButton()
{
	m_fontManager.releaseResource("arial");
}

const sf::FloatRect & GUI::GUIButton::getAABB() const
{
	return m_AABB;
}

GUIButtonName GUI::GUIButton::getName() const
{
	return m_name;
}

void GUI::GUIButton::draw(sf::RenderWindow & window)
{
	window.draw(m_button);
	window.draw(m_text);
}

//GUI
GUI::GUI(StateBase& stateBase)
	: m_stateBase(stateBase),
	m_fontManager(FontManagerLocator::getFontManager()),
	m_activateButtonTimer(0.5f, false),
	m_components(),
	m_buttonQueue()
{
}

void GUI::addButton(const sf::Vector2f & position, const sf::Vector2f & size, const std::string & text, GUIButtonName name)
{
	m_components.emplace_back(std::make_unique<GUIButton>(position, size, text, m_fontManager, name));
}

void GUI::addText(const sf::Vector2f & position, const std::string & text, int characterSize)
{
	m_components.emplace_back(std::make_unique<GUIText>(text, position, m_fontManager, characterSize));
}

void GUI::draw(sf::RenderWindow & window)
{
	for (auto& component : m_components)
	{
		component->draw(window);
	}
}

void GUI::update(float deltaTime)
{
	handleButtonInteraction();
	m_activateButtonTimer.update(deltaTime);
	handleButtonQueue();
}

void GUI::handleButtonQueue()
{
	if (!m_activateButtonTimer.isExpired() || m_buttonQueue.empty())
	{
		return;
	}

	assert(!m_buttonQueue.empty());

	const auto nextButton = m_buttonQueue.back();
	m_stateBase.activateButton(nextButton);
	m_activateButtonTimer.reset();
	m_buttonQueue.clear();
}

void GUI::handleButtonInteraction()
{
	if (m_components.empty() || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return;
	}

	const auto mousePosition = sf::Mouse::getPosition(WindowLocator::getWindow().getRenderWindow());
	const sf::FloatRect mouseRect(sf::Vector2f(mousePosition.x, mousePosition.y), sf::Vector2f(16, 16));

	for (const auto& component : m_components)
	{
		if (component->m_type != ComponentType::Button)
		{
			continue;
		}

		if (mouseRect.intersects(static_cast<GUIButton*>(component.get())->getAABB()))
		{
			const auto pressedButtonName = static_cast<GUIButton*>(component.get())->getName();
			if (std::find_if(m_buttonQueue.cbegin(), m_buttonQueue.cend(),
				[pressedButtonName](const auto& buttonName) {return buttonName == pressedButtonName; }) != m_buttonQueue.cend())
			{
				break;
			}

			m_activateButtonTimer.activate();
			m_buttonQueue.push_back(pressedButtonName);
			break;
		}
	}
}
