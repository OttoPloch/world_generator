#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    IDCounter = 0;

    reset();

    int currID;

    currID = getNewID(); bgElements[currID] = UIBackground(game, "win 2", currID, 0, {80, 80}, {220, 200}, sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll"));
    currID = getNewID(); buttonElements[currID] = UIButton(game, "faster button", currID, 3, {-25, -10}, {50, 50}, {assetManager->getTexture("button_up"), assetManager->getTexture("button_hover"), assetManager->getTexture("button_down")}, "win 2");
    currID = getNewID(); textElements[currID] = UIText(game, "faster button text", currID, 5, {0, -45}, assetManager->getFont("White Storm"), "Click me\nto go faster!", 20, sf::Color::Black, "faster button");
    currID = getNewID(); buttonElements[currID] = UIButton(game, "slower button", currID, 2, {25, -10}, {50, 50}, {assetManager->getTexture("button_up"), assetManager->getTexture("button_hover"), assetManager->getTexture("button_down")}, "win 2");
    currID = getNewID(); textElements[currID] = UIText(game, "slower button text", currID, 5, {0, -45}, assetManager->getFont("White Storm"), "Click me\nto go slower!", 20, sf::Color::Black, "slower button");
    currID = getNewID(); textElements[currID] = UIText(game, "speed display", currID, 0, {15, 0}, assetManager->getFont("White Storm"), "----", 30, sf::Color::Black, "win 2");

    getElement("speed display")->getAsText()->setBaseText("Speed: ###");
    getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));

    for (auto& i : bgElements)
    {
        namesToIDs[i.second.getName()] = i.first;
    }

    for (auto& i : textElements)
    {
        namesToIDs[i.second.getName()] = i.first;
    }

    for (auto& i : buttonElements)
    {
        namesToIDs[i.second.getName()] = i.first;
    }
}

UIElement* UILayer::getElement(int ID)
{
    if (ID > -1)
    {
        if (bgElements.find(ID) != bgElements.end())
        {
            return &bgElements[ID];
        }

        if (textElements.find(ID) != textElements.end())
        {
            return &textElements[ID];
        }

        if (buttonElements.find(ID) != buttonElements.end())
        {
            return &buttonElements[ID];
        }
        
        // if there are no entries with that ID, assume
        // it has been deleted and remove it from namesToIDs
        for (auto& i : namesToIDs)
        {
            if (i.second == ID)
            {
                namesToIDs.erase(i.first);
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

        for (auto& i : bgElements)
        {
            if (i.second.getName() == name)
            {
                namesToIDs[i.second.getName()] = i.first;

                found = true;
            }
        }
        
        for (auto& i : textElements)
        {
            if (i.second.getName() == name)
            {
                namesToIDs[i.second.getName()] = i.first;
                
                found = true;
            }
        }
        
        for (auto& i : buttonElements)
        {
            if (i.second.getName() == name)
            {
                namesToIDs[i.second.getName()] = i.first;
                
                found = true;
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

void UILayer::reset()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);

    for (auto& i : bgElements)
    {
        i.second.updateSize();
    }

    for (auto& i : textElements)
    {
        i.second.updateSize();
    }

    for (auto& i : buttonElements)
    {
        i.second.updateSize();
    }
}

void UILayer::tick()
{
    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second.tick();
        }
    }

    if (textElements.size() > 0)
    {
        for (auto& i : textElements)
        {
            i.second.tick();
        }
    }

    if (buttonElements.size() > 0)
    {
        for (auto& i : buttonElements)
        {
            i.second.tick();
        }
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second.draw();
    
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
            i.second.draw();
    
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
            i.second.draw();
    
            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    game->getWindow()->setView(camera->getView());
}