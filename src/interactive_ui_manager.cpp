#include "interactive_ui_manager.hpp"
#include "game.hpp"

InteractiveUIManager::InteractiveUIManager() {}

InteractiveUIManager::InteractiveUIManager(Game* game, std::map<int, std::unique_ptr<UIElement>>* elements, UIElement* controllerUI_indicator)
{
    init(game, elements, controllerUI_indicator);
}

void InteractiveUIManager::init(Game* game, std::map<int, std::unique_ptr<UIElement>>* elements, UIElement* controllerUI_indicator)
{
    this->game = game;
    
    uiLayer = game->getScene()->getUILayer();

    this->elements = elements;

    this->controllerUI_indicator = controllerUI_indicator;

    indicatorSize = {50.f, 50.f};

    controllerUI_indicator->resize(indicatorSize);

    controllerUI_selectedElement = nullptr;

    active = false;
}
    
void InteractiveUIManager::moveIndicator(sf::Vector2i direction)
{
    if (elements->size() <= 1) return;

    if (direction == sf::Vector2i(0, 0)) return;

    active = true;

    UIElement* target = elements->begin()->second.get();

    if (controllerUI_selectedElement) target = controllerUI_selectedElement;

    sf::Vector2f center = controllerUI_indicator->getScreenCenter();

    for (auto& i : (*elements))
    {
        if (i.second->getAsButton() == nullptr) continue;

        sf::Vector2f candidateCenter = i.second->getScreenCenter();
        sf::Vector2f targetCenter = target->getScreenCenter();

        if (direction.x < 0)
        {
            if (candidateCenter.x < center.x)
            {
                if (targetCenter.x < center.x)
                {
                    if (candidateCenter.x > targetCenter.x)
                    {
                        target = i.second.get();
                    }
                    else if (candidateCenter.x == targetCenter.x)
                    {
                        if (getDistance(center, candidateCenter) < getDistance(center, targetCenter)) target = i.second.get();
                    }
                }
                else
                {
                    target = i.second.get();
                }
            }
        }
        else if (direction.x > 0)
        {
            if (candidateCenter.x > center.x)
            {
                if (targetCenter.x > center.x)
                {
                    if (candidateCenter.x < targetCenter.x)
                    {
                        target = i.second.get();
                    }
                    else if (candidateCenter.x == targetCenter.x)
                    {
                        if (getDistance(center, candidateCenter) < getDistance(center, targetCenter)) target = i.second.get();
                    }
                }
                else
                {
                    target = i.second.get();
                }
            }
        }
        else if (direction.y < 0)
        {
            if (candidateCenter.y < center.y)
            {
                if (targetCenter.y < center.y)
                {
                    if (candidateCenter.y > targetCenter.y)
                    {
                        target = i.second.get();
                    }
                    else if (candidateCenter.y == targetCenter.y)
                    {
                        if (getDistance(center, candidateCenter) < getDistance(center, targetCenter)) target = i.second.get();
                    }
                }
                else
                {
                    target = i.second.get();
                }
            }
        }
        else if (direction.y > 0)
        {
            if (candidateCenter.y > center.y)
            {
                if (targetCenter.y > center.y)
                {
                    if (candidateCenter.y < targetCenter.y)
                    {
                        target = i.second.get();
                    }
                    else if (candidateCenter.y == targetCenter.y)
                    {
                        if (getDistance(center, candidateCenter) < getDistance(center, targetCenter)) target = i.second.get();
                    }
                }
                else
                {
                    target = i.second.get();
                }
            }
        }
    }

    if (target != controllerUI_selectedElement) controllerUI_selectedElement = target;

    controllerUI_indicator->setPosition({target->getScreenCenter().x - indicatorSize.x / 2.f, target->getScreenCenter().y - indicatorSize.y / 2.f});
}

void InteractiveUIManager::click()
{
    if (controllerUI_selectedElement != nullptr && active)
    {
        controllerUI_selectedElement->getAsButton()->activate();
    }
}

void InteractiveUIManager::disableControllerUI()
{
    active = false;
}

bool InteractiveUIManager::isControllerUIActive()
{
    return active;
}

UIElement* InteractiveUIManager::getSelectedElement() { return controllerUI_selectedElement; }

void InteractiveUIManager::draw()
{
    if (active) controllerUI_indicator->draw();
}