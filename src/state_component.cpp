#include "state_component.hpp"
#include "entity.hpp"
#include "movement_component.hpp"

StateComponent::StateComponent(Entity* myEntity) : EntityComponent(myEntity) {}

void StateComponent::update()
{
    if (auto c = myEntity->getComponent<MovementComponent>())
    {
        AnimState activeAnimState = AnimState::IDLE;

        if      (c->velocity.x < -0.5f) activeAnimState = AnimState::LEFT;
        else if (c->velocity.x > 0.5f) activeAnimState = AnimState::RIGHT;
        else if (c->velocity.y < -0.5f) activeAnimState = AnimState::UP;
        else if (c->velocity.y > 0.5f) activeAnimState = AnimState::DOWN;

        if (auto s = myEntity->getSprite())
        {
            if (auto a = s->animSet.get())
            {
                if (a->getActiveState() != activeAnimState) a->setActiveAnimation(activeAnimState);
            }
        }
    }
}