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
    
    background.setFillColor(color);

    updateSize();
}

void UIBackground::createBorderVertices()
{
    float BORDERSIZE = 98.f;
    float HALFBORDERSIZE = BORDERSIZE / 2.f;

    float TEXTURESIZE = tileSet->getTileSize();

    borderVertices.clear();

    sf::Vertex tltl;
    tltl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    tltl.texCoords = {tileSet->getCoordsFor("tl").x, tileSet->getCoordsFor("tl").y};
    sf::Vertex tltr;
    tltr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    tltr.texCoords = {tileSet->getCoordsFor("tl").x + TEXTURESIZE, tileSet->getCoordsFor("tl").y};
    sf::Vertex tlbl;
    tlbl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    tlbl.texCoords = {tileSet->getCoordsFor("tl").x, tileSet->getCoordsFor("tl").y + TEXTURESIZE};
    sf::Vertex tlbr;
    tlbr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    tlbr.texCoords = {tileSet->getCoordsFor("tl").x + TEXTURESIZE, tileSet->getCoordsFor("tl").y + TEXTURESIZE};
    borderVertices.push_back(tltl);
    borderVertices.push_back(tltr);
    borderVertices.push_back(tlbl);
    borderVertices.push_back(tlbl);
    borderVertices.push_back(tlbr);
    borderVertices.push_back(tltr);

    sf::Vertex ttl;
    ttl.position = {std::floor(left() + HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    ttl.texCoords = {tileSet->getCoordsFor("t").x, tileSet->getCoordsFor("t").y};
    sf::Vertex ttr;
    ttr.position = {std::floor(right() - HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    ttr.texCoords = {tileSet->getCoordsFor("t").x + TEXTURESIZE, tileSet->getCoordsFor("t").y};
    sf::Vertex tbl;
    tbl.position = {std::floor(left() + HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    tbl.texCoords = {tileSet->getCoordsFor("t").x, tileSet->getCoordsFor("t").y + TEXTURESIZE};
    sf::Vertex tbr;
    tbr.position = {std::floor(right() - HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    tbr.texCoords = {tileSet->getCoordsFor("t").x + TEXTURESIZE, tileSet->getCoordsFor("t").y + TEXTURESIZE};
    borderVertices.push_back(ttl);
    borderVertices.push_back(ttr);
    borderVertices.push_back(tbl);
    borderVertices.push_back(tbl);
    borderVertices.push_back(tbr);
    borderVertices.push_back(ttr);

    sf::Vertex trtl;
    trtl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    trtl.texCoords = {tileSet->getCoordsFor("tr").x, tileSet->getCoordsFor("tr").y};
    sf::Vertex trtr;
    trtr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(top() - HALFBORDERSIZE)};
    trtr.texCoords = {tileSet->getCoordsFor("tr").x + TEXTURESIZE, tileSet->getCoordsFor("tr").y};
    sf::Vertex trbl;
    trbl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    trbl.texCoords = {tileSet->getCoordsFor("tr").x, tileSet->getCoordsFor("tr").y + TEXTURESIZE};
    sf::Vertex trbr;
    trbr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    trbr.texCoords = {tileSet->getCoordsFor("tr").x + TEXTURESIZE, tileSet->getCoordsFor("tr").y + TEXTURESIZE};
    borderVertices.push_back(trtl);
    borderVertices.push_back(trtr);
    borderVertices.push_back(trbl);
    borderVertices.push_back(trbl);
    borderVertices.push_back(trbr);
    borderVertices.push_back(trtr);

    sf::Vertex ltl;
    ltl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    ltl.texCoords = {tileSet->getCoordsFor("l").x, tileSet->getCoordsFor("l").y};
    sf::Vertex ltr;
    ltr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    ltr.texCoords = {tileSet->getCoordsFor("l").x + TEXTURESIZE, tileSet->getCoordsFor("l").y};
    sf::Vertex lbl;
    lbl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    lbl.texCoords = {tileSet->getCoordsFor("l").x, tileSet->getCoordsFor("l").y + TEXTURESIZE};
    sf::Vertex lbr;
    lbr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    lbr.texCoords = {tileSet->getCoordsFor("l").x + TEXTURESIZE, tileSet->getCoordsFor("l").y + TEXTURESIZE};
    borderVertices.push_back(ltl);
    borderVertices.push_back(ltr);
    borderVertices.push_back(lbl);
    borderVertices.push_back(lbl);
    borderVertices.push_back(lbr);
    borderVertices.push_back(ltr);

    sf::Vertex rtl;
    rtl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    rtl.texCoords = {tileSet->getCoordsFor("r").x, tileSet->getCoordsFor("r").y};
    sf::Vertex rtr;
    rtr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(top() + HALFBORDERSIZE)};
    rtr.texCoords = {tileSet->getCoordsFor("r").x + TEXTURESIZE, tileSet->getCoordsFor("r").y};
    sf::Vertex rbl;
    rbl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    rbl.texCoords = {tileSet->getCoordsFor("r").x, tileSet->getCoordsFor("r").y + TEXTURESIZE};
    sf::Vertex rbr;
    rbr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    rbr.texCoords = {tileSet->getCoordsFor("r").x + TEXTURESIZE, tileSet->getCoordsFor("r").y + TEXTURESIZE};
    borderVertices.push_back(rtl);
    borderVertices.push_back(rtr);
    borderVertices.push_back(rbl);
    borderVertices.push_back(rbl);
    borderVertices.push_back(rbr);
    borderVertices.push_back(rtr);
    
    sf::Vertex bltl;
    bltl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    bltl.texCoords = {tileSet->getCoordsFor("bl").x, tileSet->getCoordsFor("bl").y};
    sf::Vertex bltr;
    bltr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    bltr.texCoords = {tileSet->getCoordsFor("bl").x + TEXTURESIZE, tileSet->getCoordsFor("bl").y};
    sf::Vertex blbl;
    blbl.position = {std::floor(left() - HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    blbl.texCoords = {tileSet->getCoordsFor("bl").x, tileSet->getCoordsFor("bl").y + TEXTURESIZE};
    sf::Vertex blbr;
    blbr.position = {std::floor(left() + HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    blbr.texCoords = {tileSet->getCoordsFor("bl").x + TEXTURESIZE, tileSet->getCoordsFor("bl").y + TEXTURESIZE};
    borderVertices.push_back(bltl);
    borderVertices.push_back(bltr);
    borderVertices.push_back(blbl);
    borderVertices.push_back(blbl);
    borderVertices.push_back(blbr);
    borderVertices.push_back(bltr);

    sf::Vertex btl;
    btl.position = {std::floor(left() + HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    btl.texCoords = {tileSet->getCoordsFor("b").x, tileSet->getCoordsFor("b").y};
    sf::Vertex btr;
    btr.position = {std::floor(right() - HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    btr.texCoords = {tileSet->getCoordsFor("b").x + TEXTURESIZE, tileSet->getCoordsFor("b").y};
    sf::Vertex bbl;
    bbl.position = {std::floor(left() + HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    bbl.texCoords = {tileSet->getCoordsFor("b").x, tileSet->getCoordsFor("b").y + TEXTURESIZE};
    sf::Vertex bbr;
    bbr.position = {std::floor(right() - HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    bbr.texCoords = {tileSet->getCoordsFor("b").x + TEXTURESIZE, tileSet->getCoordsFor("b").y + TEXTURESIZE};
    borderVertices.push_back(btl);
    borderVertices.push_back(btr);
    borderVertices.push_back(bbl);
    borderVertices.push_back(bbl);
    borderVertices.push_back(bbr);
    borderVertices.push_back(btr);

    sf::Vertex brtl;
    brtl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    brtl.texCoords = {tileSet->getCoordsFor("br").x, tileSet->getCoordsFor("br").y};
    sf::Vertex brtr;
    brtr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(bottom() - HALFBORDERSIZE)};
    brtr.texCoords = {tileSet->getCoordsFor("br").x + TEXTURESIZE, tileSet->getCoordsFor("br").y};
    sf::Vertex brbl;
    brbl.position = {std::floor(right() - HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    brbl.texCoords = {tileSet->getCoordsFor("br").x, tileSet->getCoordsFor("br").y + TEXTURESIZE};
    sf::Vertex brbr;
    brbr.position = {std::floor(right() + HALFBORDERSIZE), std::floor(bottom() + HALFBORDERSIZE)};
    brbr.texCoords = {tileSet->getCoordsFor("br").x + TEXTURESIZE, tileSet->getCoordsFor("br").y + 16.f};
    borderVertices.push_back(brtl);
    borderVertices.push_back(brtr);
    borderVertices.push_back(brbl);
    borderVertices.push_back(brbl);
    borderVertices.push_back(brbr);
    borderVertices.push_back(brtr);
}

sf::Color UIBackground::getColor() { return color; }

void UIBackground::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;
    
    background.setPosition({left(), top()});
    background.setSize(newSize);
    
    createBorderVertices();
}

void UIBackground::draw()
{
    game->getWindow()->draw(background);

    game->getWindow()->getWindow().draw(&borderVertices[0], borderVertices.size(), sf::PrimitiveType::Triangles, borderStates);
}