#include <SFML\Graphics.hpp>

class EntityManager;
class Entity;
namespace CollisionHandler
{
	void handleCollisions(const sf::Vector2f& entityPosition, const EntityManager& entityManager, sf::Vector2f& movement, Entity& entity);
}