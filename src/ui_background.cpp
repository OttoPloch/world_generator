#include "ui_background.hpp"
#include "game.hpp"

UIBackground::UIBackground() : UIElement() {}

UIBackground::UIBackground(Game* game, sf::Color color, TileSet* tileSet, UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex) : UIElement(uiLayer, posSet, position, size, parentIndex)
{
    init(game, color, tileSet);
}

void UIBackground::init(Game* game, sf::Color color, TileSet* tileSet)
{
    this->game = game;

    this->color = color;

    this->tileSet = tileSet;

    borderStates.texture = tileSet->getTexture();
    
    createBorderVertices();
}

void UIBackground::createBorderVertices()
{
    float BORDERSIZE = 90.f;
    float HALFBORDERSIZE = BORDERSIZE / 2.f;

    sf::Vertex tltl;
    tltl.position = {left() - HALFBORDERSIZE, top() - HALFBORDERSIZE};
    tltl.texCoords = {tileSet->getCoordsFor("tl").x, tileSet->getCoordsFor("tl").y};
    sf::Vertex tltr;
    tltr.position = {left() + HALFBORDERSIZE, top() - HALFBORDERSIZE};
    tltr.texCoords = {tileSet->getCoordsFor("tl").x + 16.f, tileSet->getCoordsFor("tl").y};
    sf::Vertex tlbl;
    tlbl.position = {left() - HALFBORDERSIZE, top() + HALFBORDERSIZE};
    tlbl.texCoords = {tileSet->getCoordsFor("tl").x, tileSet->getCoordsFor("tl").y + 16.f};
    sf::Vertex tlbr;
    tlbr.position = {left() + HALFBORDERSIZE, top() + HALFBORDERSIZE};
    tlbr.texCoords = {tileSet->getCoordsFor("tl").x + 16.f, tileSet->getCoordsFor("tl").y + 16.f};
    borderVertices.push_back(tltl);
    borderVertices.push_back(tltr);
    borderVertices.push_back(tlbl);
    borderVertices.push_back(tlbl);
    borderVertices.push_back(tlbr);
    borderVertices.push_back(tltr);

    sf::Vertex trtl;
    trtl.position = {right() - HALFBORDERSIZE, top() - HALFBORDERSIZE};
    trtl.texCoords = {tileSet->getCoordsFor("tr").x, tileSet->getCoordsFor("tr").y};
    sf::Vertex trtr;
    trtr.position = {right() + HALFBORDERSIZE, top() - HALFBORDERSIZE};
    trtr.texCoords = {tileSet->getCoordsFor("tr").x + 16.f, tileSet->getCoordsFor("tr").y};
    sf::Vertex trbl;
    trbl.position = {right() - HALFBORDERSIZE, top() + HALFBORDERSIZE};
    trbl.texCoords = {tileSet->getCoordsFor("tr").x, tileSet->getCoordsFor("tr").y + 16.f};
    sf::Vertex trbr;
    trbr.position = {right() + HALFBORDERSIZE, top() + HALFBORDERSIZE};
    trbr.texCoords = {tileSet->getCoordsFor("tr").x + 16.f, tileSet->getCoordsFor("tr").y + 16.f};
    borderVertices.push_back(trtl);
    borderVertices.push_back(trtr);
    borderVertices.push_back(trbl);
    borderVertices.push_back(trbl);
    borderVertices.push_back(trbr);
    borderVertices.push_back(trtr);
    
    sf::Vertex bltl;
    bltl.position = {left() - HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    bltl.texCoords = {tileSet->getCoordsFor("bl").x, tileSet->getCoordsFor("bl").y};
    sf::Vertex bltr;
    bltr.position = {left() + HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    bltr.texCoords = {tileSet->getCoordsFor("bl").x + 16.f, tileSet->getCoordsFor("bl").y};
    sf::Vertex blbl;
    blbl.position = {left() - HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    blbl.texCoords = {tileSet->getCoordsFor("bl").x, tileSet->getCoordsFor("bl").y + 16.f};
    sf::Vertex blbr;
    blbr.position = {left() + HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    blbr.texCoords = {tileSet->getCoordsFor("bl").x + 16.f, tileSet->getCoordsFor("bl").y + 16.f};
    borderVertices.push_back(bltl);
    borderVertices.push_back(bltr);
    borderVertices.push_back(blbl);
    borderVertices.push_back(blbl);
    borderVertices.push_back(blbr);
    borderVertices.push_back(bltr);

    sf::Vertex brtl;
    brtl.position = {right() - HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    brtl.texCoords = {tileSet->getCoordsFor("br").x, tileSet->getCoordsFor("br").y};
    sf::Vertex brtr;
    brtr.position = {right() + HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    brtr.texCoords = {tileSet->getCoordsFor("br").x + 16.f, tileSet->getCoordsFor("br").y};
    sf::Vertex brbl;
    brbl.position = {right() - HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    brbl.texCoords = {tileSet->getCoordsFor("br").x, tileSet->getCoordsFor("br").y + 16.f};
    sf::Vertex brbr;
    brbr.position = {right() + HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    brbr.texCoords = {tileSet->getCoordsFor("br").x + 16.f, tileSet->getCoordsFor("br").y + 16.f};
    borderVertices.push_back(brtl);
    borderVertices.push_back(brtr);
    borderVertices.push_back(brbl);
    borderVertices.push_back(brbl);
    borderVertices.push_back(brbr);
    borderVertices.push_back(brtr);

    std::vector<std::pair<std::string, sf::Vector2f>> edges;

    int xMin = 1;
    int xMax = toInt(size.x / BORDERSIZE);

    int yMin = 1;
    int yMax = toInt(size.y / BORDERSIZE);

    for (int x = xMin; x <= xMax; x++)
    {
        edges.push_back({
                "t", {
                    left() + toFloat(x) * BORDERSIZE,
                    top()
                }});   

        edges.push_back({
                "b", {
                    right() - toFloat(x) * BORDERSIZE,
                    bottom()
                }});
    }

    for (int y = yMin; y <= yMax; y++)
    {
        edges.push_back({
                "l", {
                    left(),
                    bottom() - toFloat(y) * BORDERSIZE
                }});

        edges.push_back({
                "r", {
                    right(),
                    top() + toFloat(y) * BORDERSIZE
                }});
    }

    for (int i = 0; i < edges.size(); i++)
    {
        sf::Vector2f coords = edges[i].second;
        sf::Vector2f texCoords = tileSet->getCoordsFor(edges[i].first);

        sf::Vertex tl;
        tl.position = {coords.x - HALFBORDERSIZE, coords.y - HALFBORDERSIZE};
        tl.texCoords = {texCoords.x, texCoords.y};
        sf::Vertex tr;
        tr.position = {coords.x + HALFBORDERSIZE, coords.y - HALFBORDERSIZE};
        tr.texCoords = {texCoords.x + 16.f, texCoords.y};
        sf::Vertex bl;
        bl.position = {coords.x - HALFBORDERSIZE, coords.y + HALFBORDERSIZE};
        bl.texCoords = {texCoords.x, texCoords.y + 16.f};
        sf::Vertex br;
        br.position = {coords.x + HALFBORDERSIZE, coords.y + HALFBORDERSIZE};
        br.texCoords = {texCoords.x + 16.f, texCoords.y + 16.f};
        
        borderVertices.push_back(tl);
        borderVertices.push_back(tr);
        borderVertices.push_back(bl);
        borderVertices.push_back(bl);
        borderVertices.push_back(br);
        borderVertices.push_back(tr);
    }
}

void UIBackground::draw()
{
    game->getWindow()->getWindow().draw(&borderVertices[0], borderVertices.size(), sf::PrimitiveType::Triangles, borderStates);
}

sf::Color UIBackground::getColor() { return color; }