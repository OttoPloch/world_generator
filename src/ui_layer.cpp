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
    std::array<sf::Texture*, 3> blueButtonTextures = {assetManager->getTexture("blue_button_up", "images/ui/"), assetManager->getTexture("blue_button_hover", "images/ui/"), assetManager->getTexture("blue_button_down", "images/ui/")};
    
    currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "win 1", currID, 0, toV2F(80, 80), toV2F(220, 200), sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll", "images/ui/"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "win 1 title", currID, 5, toV2F(0, -20), assetManager->getFont("White Storm"), "Speed adjust", 30, sf::Color::Black, getElement("win 1"));
    
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "faster button", currID, 3, toV2F(-25, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "faster button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go faster!", 20, sf::Color::Black, getElement("faster button"));
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "slower button", currID, 2, toV2F(25, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "slower button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go slower!", 20, sf::Color::Black, getElement("slower button"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "speed display", currID, 0, toV2F(15, 30), assetManager->getFont("White Storm"), "----", 30, sf::Color::Black, getElement("win 1"));

    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button 2", currID, 5, toV2F(0, 0), toV2F(50, 50), blueButtonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 5, toV2F(100, 100), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 6, toV2F(0, 0), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 6, toV2F(100, 100), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 7, toV2F(0, 0), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 7, toV2F(-100, 100), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "", currID, 8, toV2F(0, 0), toV2F(50, 50), buttonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button", currID, 8, toV2F(100, -100), toV2F(50, 50), blueButtonTextures);
    
    getElement("speed display")->getAsText()->setBaseText("Speed: ###");
    getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));

    currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "controller indicator", getNewID(), 0, toV2F(0, 0), toV2F(50, 50), sf::Color::Transparent, assetManager->getTileSet("16px"), assetManager->getTexture("ui_select", "images/ui/"), 36.f);
    interactiveUIManager.init(game, &elements, getElement("controller indicator"));

    reset();
}

UIElement* UILayer::getElement(std::string name)
{
    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            if (i.second->getName() == name)
            {
                return elements[i.first].get();
            }
        }
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

    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            i.second->updateSize();
        }
    }
}

void UILayer::UIUpdate()
{
    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            i.second->baseUpdate();
        }
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            if (i.second->getName() != "controller indicator")
            {
                i.second->draw();
            }

            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second->getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    interactiveUIManager.draw();

    game->getWindow()->setView(camera->getView());
}