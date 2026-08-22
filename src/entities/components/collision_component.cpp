#include "collision_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include "component_data.hpp"
#include "entity_component.hpp"
#include "movement_component.hpp"
#include "sprite_component.hpp"
#include <cstddef>
#include <algorithm>

CollisionComponent::CollisionComponent(Entity* myEntity, GamePosition position, sf::Vector2f size, bool sizeIsScaleOfSprite, RectType type) : EntityComponent(myEntity)
{
    sf::Vector2f adjustedSize = size;

    if (sizeIsScaleOfSprite)
    {
        if (auto s = myEntity->getComponent<SpriteComponent>())
        {
            sf::Vector2f spriteSize = s->sprite.getSize();
            adjustedSize = {spriteSize.x * size.x, spriteSize.y * size.y};

        }
    }
    
    rect = {position, adjustedSize, type};
}