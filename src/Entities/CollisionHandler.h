#include <SFML\Graphics.hpp>

class Level;
namespace CollisionHandler
{
	void ClampMovement(sf::Vector2f& movement, const sf::Vector2f& entityPosition, const Level& level);
}