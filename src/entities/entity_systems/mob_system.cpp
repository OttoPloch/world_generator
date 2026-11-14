#include "mob_system.hpp"
#include "../../core/scene.hpp"
#include "../components/mob_component.hpp"
#include "../../core/game.hpp"

MobSystem::MobSystem() {}

MobSystem::MobSystem(Game* p_game, Scene* p_scene) : m_game(p_game), m_scene(p_scene), m_entityLayer(m_scene->getEntityLayer()) {}

void MobSystem::tick()
{
    std::vector<unsigned int> l_noLongerValidEntities;

    for (auto entity : m_validEntities)
    {
        auto l_entityMovementComponent = entity->getComponent<MovementComponent>();
        auto l_entityMobComponent = entity->getComponent<MobComponent>();

        if (!l_entityMovementComponent || !l_entityMobComponent)
        {
            l_noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        l_entityMovementComponent->movementInput = l_entityMobComponent->m_stats.movementDirection;

        l_entityMobComponent->m_moveTimer--;
        if (l_entityMobComponent->m_moveTimer > 0) continue;
        l_entityMobComponent->m_moveTimer = l_entityMobComponent->m_stats.ticksToMove;

        if (m_game->random.getRandInt("generic", 1, 100) > l_entityMobComponent->m_stats.percentChanceToMoveWhenCan) continue;
        
        l_entityMobComponent->m_stats.movementDirection = sf::Vector2f(m_game->random.getRandInt("generic", -10, 10) / 10.f, m_game->random.getRandInt("generic", -10, 10) / 10.f);
    }

    removeAllEntityIDsInVec(m_validEntities, l_noLongerValidEntities);
}

void MobSystem::refactorEntityCache()
{
    m_validEntities = m_entityLayer->getEntitiesWithComponents<MovementComponent, MobComponent>();
}