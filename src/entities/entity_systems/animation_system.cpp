#include "animation_system.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

AnimationSystem::AnimationSystem() {}

AnimationSystem::AnimationSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void AnimationSystem::update(float dt)
{
    std::vector<unsigned int> noLongerValidEntities;

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

        if      (entityMovementComponent->velocity.x < -0.01f) activeAnimState = AnimState::LEFT;
        else if (entityMovementComponent->velocity.x > 0.01f) activeAnimState = AnimState::RIGHT;
        else if (entityMovementComponent->velocity.y < -0.01f) activeAnimState = AnimState::UP;
        else if (entityMovementComponent->velocity.y > 0.01f) activeAnimState = AnimState::DOWN;

        if (entitySpriteComponent->m_animSpeedMultChangedByVelocity && activeAnimState != AnimState::IDLE)
        {
            entitySpriteComponent->sprite.animSpeedMult = entitySpriteComponent->m_baseAnimSpeedMult * (entityMovementComponent->velocity.length() / entityMovementComponent->stats.speed);
        }
        else
        {
            entitySpriteComponent->sprite.animSpeedMult = entitySpriteComponent->m_baseAnimSpeedMult;
        }

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