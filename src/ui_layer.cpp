#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    resetView();

    bgElements.push_back(UIBackground(game, sf::Color(15, 15, 15, 220), assetManager->getTileSet("test"), this, 4, {0, 0}, {500, 300}));
    bgElements.push_back(UIBackground(game, sf::Color(255, 255, 255, 200), assetManager->getTileSet("test"), this, 4, {0, 0}, {400, 200}, 0));
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

void UILayer::resetView()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    for (int i = 0; i < bgElements.size(); i++)
    {
        bgElements[i].draw();

        if (i == 1)
        {
            if (getControl("INTERACT"))
            {
                bgElements[i].resize({bgElements[i].getSize().x + 5.f, bgElements[i].getSize().y + 1.f});
            }
            if (getControl("BACK"))
            {
                bgElements[i].resize({bgElements[i].getSize().x - 5.f, bgElements[i].getSize().y - 1.f});
            }
        }
    }

    game->getWindow()->setView(camera->getView());
}