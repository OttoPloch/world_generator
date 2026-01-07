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
    // bgElements.push_back(UIBackground(game, sf::Color(255, 255, 255, 200), assetManager->getTileSet("test"), this, 4, {0, 0}, {400, 200}, 0));
    bgElements.push_back(UIBackground(game, sf::Color(255, 198, 163), assetManager->getTileSet("scroll"), this, 0, {75, 75}, {350, 150}, 0));

    textElements.push_back(UIText(game, assetManager->getFont("White Storm"), "Hello World!", 30, this, 4, {0, 0}, 1));
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
        bgElements[i].updateSize();
    }

    for (int i = 0; i < textElements.size(); i++)
    {
        textElements[i].updateSize();
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    for (int i = 0; i < bgElements.size(); i++)
    {
        bgElements[i].draw();

        if (i == 1)
        {
            if (game->getInput()->getControl("INTERACT"))
            {
                bgElements[i].resize({bgElements[i].getSize().x + 5.f, bgElements[i].getSize().y + 1.f});

                for (int j = 0; j < bgElements.size(); j++) bgElements[j].updateSize();
                for (int j = 0; j < textElements.size(); j++) textElements[j].updateSize();
            }

            if (game->getInput()->getControl("MENU"))
            {
                bgElements[i].resize({bgElements[i].getSize().x - 5.f, bgElements[i].getSize().y - 1.f});

                for (int j = 0; j < bgElements.size(); j++) bgElements[j].updateSize();
                for (int j = 0; j < textElements.size(); j++) textElements[j].updateSize();
            }
        }

        // sf::CircleShape circle(5.f);
        // circle.setFillColor(sf::Color::Red);
        // circle.setOrigin({5.f, 5.f});
        // circle.setPosition(bgElements[i].getScreenCenter());
        // game->getWindow()->getWindow().draw(circle);
    }

    for (int i = 0; i < textElements.size(); i++)
    {
        textElements[i].draw();

        // sf::CircleShape circle(5.f);
        // circle.setFillColor(sf::Color::Red);
        // circle.setOrigin({5.f, 5.f});
        // circle.setPosition(textElements[i].getScreenCenter());
        // game->getWindow()->getWindow().draw(circle);
    }

    game->getWindow()->setView(camera->getView());
}