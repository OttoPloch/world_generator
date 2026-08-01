#include "render_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../entity_layer.hpp"
#include "../components/components.hpp"
#include <algorithm>

RenderSystem::RenderSystem() {}

RenderSystem::RenderSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void RenderSystem::update(float dt)
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<SpriteComponent>();

    for (auto e : validEntities)
    {
        e->getComponent<SpriteComponent>()->sprite.update(dt);
    }
}

void RenderSystem::draw()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<SpriteComponent>();

    std::sort(validEntities.begin(), validEntities.end(), [](Entity* a, Entity* b){
        return a->getComponent<SpriteComponent>()->sprite.bottom() < b->getComponent<SpriteComponent>()->sprite.bottom();
    });

    for (auto e : validEntities)
    {
        auto& sprite = e->getComponent<SpriteComponent>()->sprite;

        sprite.syncPos();

        sprite.draw(game->getWindow()->getWindow());
    }
}