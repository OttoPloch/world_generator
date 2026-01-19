#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    IDCounter = 0;

    int currID;
    
    std::array<sf::Texture*, 3> buttonTextures = {assetManager->getTexture("button_up", "images/ui/"), assetManager->getTexture("button_hover", "images/ui/"), assetManager->getTexture("button_down", "images/ui/")};
    
    currID = getNewID(); bgElements[currID] = std::make_unique<UIBackground>(game, "win 1", currID, 0, toV2F(80, 80), toV2F(220, 200), sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll", "images/ui/"));
    currID = getNewID(); textElements[currID] = std::make_unique<UIText>(game, "win 1 title", currID, 5, toV2F(0, -20), assetManager->getFont("White Storm"), "Speed adjust", 30, sf::Color::Black, "win 1");
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "faster button", currID, 3, toV2F(-25, -10), toV2F(50, 50), buttonTextures, "win 1");
    currID = getNewID(); textElements[currID] = std::make_unique<UIText>(game, "faster button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go faster!", 20, sf::Color::Black, "faster button");
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "slower button", currID, 2, toV2F(25, -10), toV2F(50, 50), buttonTextures, "win 1");
    currID = getNewID(); textElements[currID] = std::make_unique<UIText>(game, "slower button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go slower!", 20, sf::Color::Black, "slower button");
    currID = getNewID(); textElements[currID] = std::make_unique<UIText>(game, "speed display", currID, 0, toV2F(15, 30), assetManager->getFont("White Storm"), "----", 30, sf::Color::Black, "win 1");

    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 5, toV2F(0, 0),      toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 5, toV2F(100, 100),  toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 6, toV2F(0, 0),      toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 6, toV2F(100, 100),  toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 7, toV2F(0, 0),      toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 7, toV2F(-100, 10),  toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 8, toV2F(0, 0),      toV2F(50, 50), buttonTextures);
    currID = getNewID(); buttonElements[currID] = std::make_unique<UIButton>(game, "", currID, 8, toV2F(100, -100), toV2F(50, 50), buttonTextures);
    
    getElement("speed display")->getAsText()->setBaseText("Speed: ###");
    getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));

    for (auto& i : bgElements)
    {
        namesToIDs[i.second->getName()] = i.first;
    }

    for (auto& i : textElements)
    {
        namesToIDs[i.second->getName()] = i.first;
    }

    for (auto& i : buttonElements)
    {
        namesToIDs[i.second->getName()] = i.first;
    }

    interactiveUIManager.init(game, &buttonElements, UIBackground(game, "controller", getNewID(), 0, {0, 0}, {50, 50}, sf::Color::Transparent, assetManager->getTileSet("16px"), assetManager->getTexture("ui_select", "images/ui/"), 36.f));
    namesToIDs[interactiveUIManager.controllerUI_indicator.getName()] = interactiveUIManager.controllerUI_indicator.getID();

    reset();
}

UIElement* UILayer::getElement(int ID)
{
    if (ID > -1)
    {
        if (interactiveUIManager.controllerUI_indicator.getID() == ID) return &interactiveUIManager.controllerUI_indicator;

        if (bgElements.size() > 0)
        {
            if (bgElements.find(ID) != bgElements.end())
            {
                return bgElements[ID].get();
            }
        }

        if (textElements.size() > 0)
        {
            if (textElements.find(ID) != textElements.end())
            {
                return textElements[ID].get();
            }
        }

        if (buttonElements.size() > 0)
        {
            if (buttonElements.find(ID) != buttonElements.end())
            {
                return buttonElements[ID].get();
            }
        }
        
        // if there are no entries with that ID, assume
        // it has been deleted and remove it from namesToIDs
        if (namesToIDs.size() > 0)
        {
            for (auto& i : namesToIDs)
            {
                if (i.second == ID)
                {
                    namesToIDs.erase(i.first);
                }
            }
        }
    }

    return nullptr;
}

UIElement* UILayer::getElement(std::string name)
{
    if (namesToIDs.find(name) != namesToIDs.end())
    {
        return getElement(namesToIDs[name]);
    }
    else
    {
        bool found = false;

        if (bgElements.size() > 0)
        {
            for (auto& i : bgElements)
            {
                if (i.second->getName() == name)
                {
                    namesToIDs[i.second->getName()] = i.first;

                    found = true;
                }
            }
        }
        
        if (textElements.size() > 0)
        {        
            for (auto& i : textElements)
            {
                if (i.second->getName() == name)
                {
                    namesToIDs[i.second->getName()] = i.first;
                    
                    found = true;
                }
            }
        }
        
        if (buttonElements.size() > 0)
        {
            for (auto& i : buttonElements)
            {
                if (i.second->getName() == name)
                {
                    namesToIDs[i.second->getName()] = i.first;
                    
                    found = true;
                }
            }
        }

        if (found) return getElement(name);
    }

    return nullptr;
}

int UILayer::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

sf::Vector2u UILayer::getScreenSize()
{
    return game->getWindow()->getSize();
}

sf::Vector2f UILayer::getViewSize()
{
    return game->getWindow()->getWindow().getView().getSize();
}

void UILayer::reset()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);

    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second->updateSize();
        }
    }

    if (textElements.size() > 0)
    {
        for (auto& i : textElements)
        {
            i.second->updateSize();
        }
    }

    if (buttonElements.size() > 0)
    {
        for (auto& i : buttonElements)
        {
            i.second->updateSize();
        }
    }

    interactiveUIManager.controllerUI_indicator.updateSize();
}

void UILayer::UIUpdate()
{
    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second->update();
        }
    }

    if (textElements.size() > 0)
    {
        for (auto& i : textElements)
        {
            i.second->update();
        }
    }

    if (buttonElements.size() > 0)
    {
        for (auto& i : buttonElements)
        {
            i.second->update();
        }
    }

    interactiveUIManager.update();
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second->draw();

            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    if (textElements.size() > 0)
    {
        for (auto& i : textElements)
        {
            i.second->draw();

            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    if (buttonElements.size() > 0)
    {
        for (auto& i : buttonElements)
        {
            i.second->draw();

            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    interactiveUIManager.draw();

    game->getWindow()->setView(camera->getView());
}