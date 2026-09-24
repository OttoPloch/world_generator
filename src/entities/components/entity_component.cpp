#include "entity_component.hpp"
#include "../entity.hpp"

EntityComponent::EntityComponent(Entity* myEntity, std::string componentTypeIdentifier) : myEntity(myEntity), componentTypeIdentifier(componentTypeIdentifier) {}

EntityComponent::~EntityComponent() {}