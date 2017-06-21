#include <SFML\Graphics.hpp>
#include <string>

class EntityManager;
class Entity;
namespace CollisionHandler
{
	void handleCollisions(const sf::Vector2f& entityPosition, const EntityManager& entityManager, sf::Vector2f& movement, Entity& entity);
	void clampMovement(const sf::FloatRect& intersection, sf::Vector2f& movement);
	bool isTileAtPosition(const sf::Vector2i& position);
	bool isEntityAtPosition(const std::string& entityName, const sf::Vector2f& position, const EntityManager& entityManager);
}