#include "item_component.hpp"
#include "entity_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include "sprite_component.hpp"

ItemComponent::ItemComponent(Entity* myEntity, sf::Vector2f spawnAreaOffset, sf::Vector2f spawnAreaSize) : EntityComponent(myEntity)
{
    myEntity->position.changePosition(spawnAreaOffset);
    myEntity->position.changePosition({
        static_cast<float>(myEntity->game->random.getRandInt(0, spawnAreaSize.x)),
        static_cast<float>(myEntity->game->random.getRandInt(0, spawnAreaSize.y)),
    });
}

void ItemComponent::init(std::string resource, unsigned int amount)
{
    this->resource = resource;
    this->amount = amount;

    if (auto s = &myEntity->getComponent<SpriteComponent>()->sprite)
    {
        s->setTexture(myEntity->game->getAssetManager()->getTexture("items", "texture_atlases/"));
        sf::FloatRect texCoords = getRandomTextureAtlasChoice(myEntity->game, myEntity->game->getAssetManager()->getTextureAtlas(resource, "items/"));
        s->setTextureRect({static_cast<sf::Vector2i>(texCoords.position), static_cast<sf::Vector2i>(texCoords.size)});
        
        float tileScale = getTileScale(myEntity->game);
        s->resize({tileScale, tileScale}, true);
    
        float maxSpawnVelocityScaled = 1000 * myEntity->game->getSettings()->item_maxSpawnVelocity;
        if (auto m = myEntity->getComponent<MovementComponent>())
        {
            m->velocity = {
                static_cast<float>(myEntity->game->random.getRandInt(-maxSpawnVelocityScaled, maxSpawnVelocityScaled)) / 1000,
                static_cast<float>(myEntity->game->random.getRandInt(-maxSpawnVelocityScaled, maxSpawnVelocityScaled)) / 1000
            };
        }
    }
}