#include "tile_layer.hpp"
#include "game.hpp"

TileLayer::TileLayer() {}

TileLayer::TileLayer(Game* game)
{
    init(game);
}

void TileLayer::init(Game* game)
{
    this->game = game;

    this->window = game->getWindow();

    int length = 100;
    sf::Vector2f offset = {-500, 1300};
    float tilesize = 150.f;

    for (int i = 0; i < length * length; i++)
    {
        int type = getRandInt(0, 3);

        if (type == 3)
        {
            tiles.emplace_back(sf::Vector2f(offset.x + (toFloat(i % length) * tilesize), offset.y + (std::floor(toFloat(i) / length) * tilesize)), sf::Vector2f(tilesize, tilesize), type, true, "pink");
        }
        else
        {
            tiles.emplace_back(sf::Vector2f(offset.x + (toFloat(i % length) * tilesize), offset.y + (std::floor(toFloat(i) / length) * tilesize)), sf::Vector2f(tilesize, tilesize), type);
        }
    }

    for (auto& tile : tiles)
    {
        sf::Color color;

        switch (tile.getType())
        {
            case 0:
                color = sf::Color::Green;
                break;
            case 1:
                color = sf::Color::Blue;
                break;
            case 2:
                color = sf::Color::Red;
                break;
            default:
                color = sf::Color(255, 60, 220);
                break;
        }

        sf::Vertex tl;
        sf::Vertex tr;
        sf::Vertex bl;
        sf::Vertex br;

        tl.position = {tile.getPosition().x, tile.getPosition().y};
        tr.position = {tile.getPosition().x + tile.getSize().x, tile.getPosition().y};
        bl.position = {tile.getPosition().x, tile.getPosition().y + tile.getSize().y};
        br.position = {tile.getPosition().x + tile.getSize().x, tile.getPosition().y + tile.getSize().y};

        tl.color = color;
        tr.color = color;
        bl.color = color;
        br.color = color;

        vertices.push_back(tl);
        vertices.push_back(tr);
        vertices.push_back(bl);
        vertices.push_back(bl);
        vertices.push_back(tr);
        vertices.push_back(br);
    }
}

std::vector<Tile>* TileLayer::getTiles() { return &tiles; }

void TileLayer::tick()
{

}

void TileLayer::draw()
{
    window->getWindow().draw(&vertices[0], vertices.size(), sf::PrimitiveType::Triangles);

    // for (int i = 0; i < tiles.size(); i++)
    // {
    //     if (tiles[i].hasCollider())
    //     {
    //         sf::RectangleShape rect(tiles[i].getRect().size);

    //         rect.setFillColor(sf::Color::Transparent);
    //         rect.setOutlineColor(sf::Color::Blue);
    //         rect.setOutlineThickness(3.f);
    //         rect.setPosition(tiles[i].getRect().position);

    //         window->draw(rect);
    //     }
    // }
}