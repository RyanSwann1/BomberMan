#include <GUI\GUI.h>
#include <Managers\FontManager.h>
#include <Locators\FontManagerLocator.h>
#include <Game\Timer.h>
#include <Locators\WindowLocator.h>
#include <Window.h>
#include <States\StateBase.h>
#include <Managers\StateManager.h>
#include <assert.h>

//GUIComponent
GUI::GUIComponent::GUIComponent(const std::string& name, ComponentType type)
	: m_name(name),
	m_type(type)
{}

GUI::GUIComponent::GUIComponent(GUIComponent & orig)
	: m_type(orig.m_type),
	m_name(orig.m_name)
{
}

const std::string & GUI::GUIComponent::getName() const
{
	return m_name;
}

//GUI Text
GUI::GUIText::GUIText(const std::string & text, const sf::Vector2f & position, const std::string& name, FontManager & fontManager, int characterSize)
	: GUIComponent(name, ComponentType::Text),
	m_fontManager(fontManager),
	m_text(text, m_fontManager.getResource("arial"), characterSize)
{
	m_text.setPosition(position);
	m_text.setFillColor(sf::Color::White);
}

GUI::GUIText::GUIText(GUIText& orig)
	: GUIComponent(orig.m_name, ComponentType::Text),
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
GUI::GUIButton::GUIButton(const sf::Vector2f & position, const sf::Vector2f & size, const std::string & text, const std::string& name, FontManager & fontManager)
	: GUIComponent(name, ComponentType::Button),
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
	: GUIComponent(orig.m_name, orig.m_type),
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

void GUI::GUIButton::draw(sf::RenderWindow & window)
{
	window.draw(m_button);
	window.draw(m_text);
}

//GUI
GUI::GUI(StateBase& owner, const StateManager& stateManager)
	: m_stateManager(stateManager),
	m_owner(owner),
	m_fontManager(FontManagerLocator::getFontManager()),
	m_activateButtonTimer(0.5f, false),
	m_guiComponents(),
	m_buttonQueue()
{
}

void GUI::addButton(const sf::Vector2f & position, const sf::Vector2f & size, const std::string & text, const std::string& name)
{
	m_guiComponents.emplace_back(std::make_unique<GUIButton>(position, size, text, name, m_fontManager));
}

void GUI::addText(const sf::Vector2f & position, const std::string & text, std::string&& name, int characterSize)
{
	m_guiComponents.emplace_back(std::make_unique<GUIText>(text, position, name, m_fontManager, characterSize));
}

void GUI::updateText(const std::string & newText, const std::string& name)
{
	auto iter = std::find_if(m_guiComponents.begin(), m_guiComponents.end(), [&name](const auto& component) { return component->m_name == name; });
	assert(iter != m_guiComponents.cend());
	assert(iter->get()->m_type == ComponentType::Text);

	static_cast<GUIText*>(iter->get())->m_text.setString(newText);
}

void GUI::draw(sf::RenderWindow & window)
{
	for (auto& component : m_guiComponents)
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

	const auto& nextButton = m_buttonQueue.back();
	m_owner.activateButton(nextButton);
	m_activateButtonTimer.reset();
	m_buttonQueue.clear();
}

void GUI::handleButtonInteraction()
{
	if (m_owner.getType() != m_stateManager.getCurrentStateType())
	{
		return;
	}

	if (m_guiComponents.empty() || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		return;
	}

	const auto mousePosition = sf::Mouse::getPosition(WindowLocator::getWindow().getRenderWindow());
	const sf::FloatRect mouseRect(sf::Vector2f(mousePosition.x, mousePosition.y), sf::Vector2f(16, 16));

	for (const auto& component : m_guiComponents)
	{
		if (component->m_type != ComponentType::Button)
		{
			continue;
		}

		if (mouseRect.intersects(static_cast<GUIButton*>(component.get())->m_AABB))
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