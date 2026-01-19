#include "interactive_ui_manager.hpp"
#include "game.hpp"

InteractiveUIManager::InteractiveUIManager() {}

InteractiveUIManager::InteractiveUIManager(Game* game, std::map<int, std::unique_ptr<UIButton>>* buttons, UIBackground controllerUI_indicator)
{
    init(game, buttons, controllerUI_indicator);
}

void InteractiveUIManager::init(Game* game, std::map<int, std::unique_ptr<UIButton>>* buttons, UIBackground controllerUI_indicator)
{
    this->game = game;
    
    uiLayer = game->getScene()->getUILayer();

    this->buttons = buttons;

    this->controllerUI_indicator = controllerUI_indicator;

    indicatorSize = {50.f, 50.f};

    controllerUI_indicator.resize(indicatorSize);

    controllerUI_selectedElement = -1;
}
    
void InteractiveUIManager::moveIndicator(sf::Vector2i direction)
{
    if (buttons->size() == 0) return;

    UIElement* target = buttons->begin()->second.get();
    
    if (controllerUI_selectedElement != -1) target = uiLayer->getElement(controllerUI_selectedElement);

    sf::Vector2f center = controllerUI_indicator.getScreenCenter();

    for (auto& i : (*buttons))
    {
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

    if (target->getID() != controllerUI_selectedElement) controllerUI_selectedElement = target->getID();

    controllerUI_indicator.setPosition({target->getScreenCenter().x - indicatorSize.x / 2.f, target->getScreenCenter().y - indicatorSize.y / 2.f});
}

void InteractiveUIManager::click()
{
    if (controllerUI_selectedElement != -1)
    {
        UIElement* selectedElement = uiLayer->getElement(controllerUI_selectedElement);

        if (selectedElement != nullptr)
        {
            selectedElement->getAsButton()->activate();
        }
    }
}

void InteractiveUIManager::disableControllerUI()
{
    controllerUI_selectedElement = -1;

    controllerUI_indicator.setPosition({0, 0});
}

bool InteractiveUIManager::isControllerUIActive()
{
    return (controllerUI_selectedElement != -1);
}

int InteractiveUIManager::getSelectedElementID() { return controllerUI_selectedElement; }

void InteractiveUIManager::reset()
{
    controllerUI_indicator.updateSize();
}

void InteractiveUIManager::update()
{
    controllerUI_indicator.update();
}

void InteractiveUIManager::draw()
{
    if (controllerUI_selectedElement != -1) controllerUI_indicator.draw();
}