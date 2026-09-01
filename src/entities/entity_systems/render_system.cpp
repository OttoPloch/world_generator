#include "render_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../entity_layer.hpp"
#include "../components/components.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <algorithm>

RenderSystem::RenderSystem() {}

RenderSystem::RenderSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void RenderSystem::update(float dt)
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entitySpriteComponent = entity->getComponent<SpriteComponent>();

        if (!entitySpriteComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        entitySpriteComponent->sprite.update(dt);
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void RenderSystem::draw(bool debug)
{
    std::sort(validEntities.begin(), validEntities.end(), [](Entity* a, Entity* b){
        return a->getComponent<SpriteComponent>()->sprite.bottom() < b->getComponent<SpriteComponent>()->sprite.bottom();
    });

    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entitySpriteComponent = entity->getComponent<SpriteComponent>();

        if (!entitySpriteComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        auto& entitySprite = entitySpriteComponent->sprite;

        entitySprite.syncPos();

        entitySprite.draw(game->getWindow()->getWindow());

        if (debug)
        {
            if (auto entityCollisionComponent = entity->getComponent<CollisionComponent>())
            {
                sf::Vector2f entityCollisionComponentTopleft = entityCollisionComponent->rect.position.getPosition() - (entityCollisionComponent->rect.size / 2.f);

                std::array<sf::Vertex, 8> debugVertices = VertexGroup::createLineVerts(entityCollisionComponentTopleft, entityCollisionComponent->rect.size, sf::Color::Red);
                game->getWindow()->getWindow().draw(debugVertices.data(), debugVertices.size(), sf::PrimitiveType::Lines);
            }
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void RenderSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<SpriteComponent>();
}