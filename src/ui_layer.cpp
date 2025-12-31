#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    resetView();

    bgElements.push_back(UIBackground(game, sf::Color(15, 15, 15, 220), assetManager->getTileSet("test"), this, 0, {50, 50}, {500, 300}));
    // bgElements.push_back(UIBackground(game, sf::Color(15, 15, 15, 220), assetManager->getTileSet("test"), this, 0, {800, 50}, {500, 300}));
    // bgElements.push_back(UIBackground(game, sf::Color(30, 30, 30, 220), assetManager->getTileSet("test"), this, 0, {25, 25}, {360, 125}, 0));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 5}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 35}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 65}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {5, 95}, {350, 25}, 1));
    // bgElements.push_back(UIBackground(game, sf::Color(225, 0, 0), assetManager->getTileSet("test"), this, 0, {25, 175}, {350, 25}, 0));

    if (bgElements.size() > 0)
    {
        for (int i = 0; i < bgElements.size(); i++)
        {
            UIBackground* currElement = &bgElements[i];

            sf::Vertex tlVert;
            tlVert.position = {currElement->left(), currElement->top()};
            tlVert.color = currElement->getColor();
            sf::Vertex trVert;
            trVert.position = {currElement->right(), currElement->top()};
            trVert.color = currElement->getColor();
            sf::Vertex blVert;
            blVert.position = {currElement->left(), currElement->bottom()};
            blVert.color = currElement->getColor();
            sf::Vertex brVert;
            brVert.position = {currElement->right(), currElement->bottom()};
            brVert.color = currElement->getColor();

            bgVertices.push_back(tlVert);
            bgVertices.push_back(trVert);
            bgVertices.push_back(blVert);
            bgVertices.push_back(trVert);
            bgVertices.push_back(brVert);
            bgVertices.push_back(blVert);
        }
    }
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

    game->getWindow()->getWindow().draw(&bgVertices[0], bgVertices.size(), sf::PrimitiveType::Triangles);

    for (int i = 0; i < bgElements.size(); i++)
    {
        bgElements[i].draw();
    }
}