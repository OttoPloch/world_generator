#include "action_component.hpp"
#include "entity_component.hpp"

ActionComponent::ActionComponent(Entity* myEntity, std::string mainAction, std::string secondaryAction) : EntityComponent(myEntity), mainAction(mainAction), secondaryAction(secondaryAction) {}

void ActionComponent::setAction(std::string action, bool secondary)
{
    if (secondary) this->secondaryAction = action;
    else this->mainAction = action;
}

void ActionComponent::createRequest(std::string actionInput)
{
    if (actionInput == "MAIN ACTION")
    {
        std::cout << mainAction << '\n';
    }
    else if (actionInput == "SECONDARY ACTION")
    {
        std::cout << secondaryAction << '\n';
    }
}