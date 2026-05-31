#include "animation_system.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void AnimationSystem::update(float dt)
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, SpriteComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();
        auto s = e->getComponent<SpriteComponent>();

        AnimState activeAnimState = AnimState::IDLE;

        if      (m->velocity.x < -0.1f) activeAnimState = AnimState::LEFT;
        else if (m->velocity.x > 0.1f) activeAnimState = AnimState::RIGHT;
        else if (m->velocity.y < -0.1f) activeAnimState = AnimState::UP;
        else if (m->velocity.y > 0.1f) activeAnimState = AnimState::DOWN;

        auto animSet = s->sprite.animSet.get();

        if (animSet)
        {
            if (animSet->getActiveState() != activeAnimState) animSet->setActiveAnimation(activeAnimState);
        }
    }
}