#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    reset();

    bgElements.push_back(UIBackground(game, sf::Color(15, 15, 15, 220), assetManager->getTileSet("test"), this, 0, {50, 50}, {500, 300}));
    bgElements.push_back(UIBackground(game, sf::Color(255, 255, 255, 200), assetManager->getTileSet("test"), this, 3, {-50, -50}, {400, 200}, 0));
    // bgElements.push_back(UIBackground(game, sf::Color(30, 30, 30, 220), assetManager->getTileSet("test"), this, 0, {25, 25}, {360, 125}, 0));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 5}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 35}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 65}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 95}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {25, 175}, {350, 25}, 0));
}

UIElement* UILayer::getElement(int index)
{
    if (index > -1)
    {
        return &bgElements[index];
    }

    return nullptr;
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
        bgElements[i].resize();
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    for (int i = 0; i < bgElements.size(); i++)
    {
        bgElements[i].draw();

        if (i == 0)
        {
            if (game->getInput()->getControl("INTERACT"))
            {
                bgElements[i].resize({bgElements[i].getSize().x + 5.f, bgElements[i].getSize().y + 1.f});
            }
            if (game->getInput()->getControl("MENU"))
            {
                bgElements[i].resize({bgElements[i].getSize().x - 5.f, bgElements[i].getSize().y - 1.f});
            }
        }
        else
        {
            if (game->getInput()->getControl("INTERACT") || game->getInput()->getControl("MENU"))
            {
                bgElements[i].resize();
            }
        }
    }

    game->getWindow()->setView(camera->getView());
}