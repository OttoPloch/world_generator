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

    bgElements.push_back(UIBackground(game, this, getNewID(), 8, {0, -50}, {600, 300}, sf::Color(0, 0, 0, 0), assetManager->getTileSet("16px filled"), assetManager->getTexture("ui_tech")));
    bgElements.push_back(UIBackground(game, this, getNewID(), 4, {0, 0}, {300, 200}, sf::Color(0, 0, 0, 0), assetManager->getTileSet("16px filled"), assetManager->getTexture("ui_tech"), bgElements[0].getID()));
    bgElements.push_back(UIBackground(game, this, getNewID(), 0, {20, 20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, bgElements[0].getID()));
    bgElements.push_back(UIBackground(game, this, getNewID(), 1, {-20, 20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, bgElements[0].getID()));
    bgElements.push_back(UIBackground(game, this, getNewID(), 2, {20, -20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, bgElements[0].getID()));
    bgElements.push_back(UIBackground(game, this, getNewID(), 3, {-20, -20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, bgElements[0].getID()));

    textElements.push_back(UIText(game, this, getNewID(), 0, {0, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 50, sf::Color(255, 255, 255), bgElements[0].getID()));
    textElements.push_back(UIText(game, this, getNewID(), 0, {0, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 50, sf::Color(255, 255, 255), bgElements[1].getID()));
    textElements.push_back(UIText(game, this, getNewID(), 4, {0, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 50, sf::Color(255, 255, 255), bgElements[1].getID()));
}

UIElement* UILayer::getElement(int ID)
{
    if (ID > -1)
    {
        if (bgElements.size() > 0)
        {
            for (int i = 0; i < bgElements.size(); i++)
            {
                if (bgElements[i].getID() == ID)
                {
                    return &bgElements[i];
                }
            }
        }

        if (textElements.size() > 0)
        {
            for (int i = 0; i < textElements.size(); i++)
            {
                if (textElements[i].getID() == ID)
                {
                    return &textElements[i];
                }
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

void UILayer::reset()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);

    for (int i = 0; i < bgElements.size(); i++)
    {
        bgElements[i].updateSize();
    }

    for (int i = 0; i < textElements.size(); i++)
    {
        textElements[i].updateSize();
    }
}

void UILayer::tick()
{
    if (bgElements.size() > 0)
    {
        for (int i = 0; i < bgElements.size(); i++)
        {
            if (i == 0)
            {
                if (game->getInput()->getControl("INTERACT"))
                {
                    bgElements[i].resize({bgElements[i].getSize().x + 10.f, bgElements[i].getSize().y + 2.f});
        
                    for (int j = 0; j < bgElements.size(); j++) bgElements[j].updateSize();
                    for (int j = 0; j < textElements.size(); j++) textElements[j].updateSize();
                }
        
                if (game->getInput()->getControl("MENU"))
                {
                    bgElements[i].resize({bgElements[i].getSize().x - 10.f, bgElements[i].getSize().y - 2.f});
        
                    for (int j = 0; j < bgElements.size(); j++) bgElements[j].updateSize();
                    for (int j = 0; j < textElements.size(); j++) textElements[j].updateSize();
                }
            }
        }
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (bgElements.size() > 0)
    {
        for (int i = 0; i < bgElements.size(); i++)
        {
            bgElements[i].draw();
    
            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(bgElements[i].getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    if (textElements.size() > 0)
    {
        for (int i = 0; i < textElements.size(); i++)
        {
            textElements[i].draw();
    
            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(textElements[i].getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    game->getWindow()->setView(camera->getView());
}