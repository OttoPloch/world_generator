#include "movement_component.hpp"
#include "../../core/game.hpp"
#include "../entity.hpp"
#include "component_data.hpp"
#include "control_component.hpp"

MovementComponent::MovementComponent(Entity* myEntity, sf::Vector2f velocity, MovementComponentData stats) : EntityComponent(myEntity), velocity(velocity), stats(stats) {}