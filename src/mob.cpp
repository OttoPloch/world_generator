#include "mob.hpp"

Mob::Mob() : Entity() {}

Mob::Mob(Game* game, int ID, sf::Vector2f position) : Entity(game, ID, position), velocity({0, 0}) {}

Mob::Mob(Game* game, int ID, WorldPosition position) : Entity(game, ID, position), velocity({0, 0}) {}

void Mob::tick()
{
    lastPosition = position.getPos();

    position.position->x += velocity.x;
    position.position->y += velocity.y;
}