#include "animation_system.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void AnimationSystem::update(float dt)
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityMovementComponent = entity->getComponent<MovementComponent>();
        auto entitySpriteComponent = entity->getComponent<SpriteComponent>();

        if (!entityMovementComponent || !entitySpriteComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        AnimState activeAnimState = AnimState::IDLE;

        if      (entityMovementComponent->velocity.x < -0.1f) activeAnimState = AnimState::LEFT;
        else if (entityMovementComponent->velocity.x > 0.1f) activeAnimState = AnimState::RIGHT;
        else if (entityMovementComponent->velocity.y < -0.1f) activeAnimState = AnimState::UP;
        else if (entityMovementComponent->velocity.y > 0.1f) activeAnimState = AnimState::DOWN;

        auto animSet = entitySpriteComponent->sprite.animSet.get();

        if (animSet)
        {
            if (animSet->getActiveState() != activeAnimState) animSet->setActiveAnimation(activeAnimState);
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void AnimationSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, SpriteComponent>();
}