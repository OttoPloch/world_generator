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

    background.setPosition({left(), top()});
    background.setSize(size);

    createBorderVertices();
}

void UIBackground::createBorderVertices()
{
    float BORDERSIZE = 90.f;
    float HALFBORDERSIZE = BORDERSIZE / 2.f;

    borderVertices.clear();

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

    sf::Vertex ttl;
    ttl.position = {left() + HALFBORDERSIZE, top() - HALFBORDERSIZE};
    ttl.texCoords = {tileSet->getCoordsFor("t").x, tileSet->getCoordsFor("t").y};
    sf::Vertex ttr;
    ttr.position = {right() - HALFBORDERSIZE, top() - HALFBORDERSIZE};
    ttr.texCoords = {tileSet->getCoordsFor("t").x + 16.f, tileSet->getCoordsFor("t").y};
    sf::Vertex tbl;
    tbl.position = {left() + HALFBORDERSIZE, top() + HALFBORDERSIZE};
    tbl.texCoords = {tileSet->getCoordsFor("t").x, tileSet->getCoordsFor("t").y + 16.f};
    sf::Vertex tbr;
    tbr.position = {right() - HALFBORDERSIZE, top() + HALFBORDERSIZE};
    tbr.texCoords = {tileSet->getCoordsFor("t").x + 16.f, tileSet->getCoordsFor("t").y + 16.f};
    borderVertices.push_back(ttl);
    borderVertices.push_back(ttr);
    borderVertices.push_back(tbl);
    borderVertices.push_back(tbl);
    borderVertices.push_back(tbr);
    borderVertices.push_back(ttr);

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

    sf::Vertex ltl;
    ltl.position = {left() - HALFBORDERSIZE, top() + HALFBORDERSIZE};
    ltl.texCoords = {tileSet->getCoordsFor("l").x, tileSet->getCoordsFor("l").y};
    sf::Vertex ltr;
    ltr.position = {left() + HALFBORDERSIZE, top() + HALFBORDERSIZE};
    ltr.texCoords = {tileSet->getCoordsFor("l").x + 16.f, tileSet->getCoordsFor("l").y};
    sf::Vertex lbl;
    lbl.position = {left() - HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    lbl.texCoords = {tileSet->getCoordsFor("l").x, tileSet->getCoordsFor("l").y + 16.f};
    sf::Vertex lbr;
    lbr.position = {left() + HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    lbr.texCoords = {tileSet->getCoordsFor("l").x + 16.f, tileSet->getCoordsFor("l").y + 16.f};
    borderVertices.push_back(ltl);
    borderVertices.push_back(ltr);
    borderVertices.push_back(lbl);
    borderVertices.push_back(lbl);
    borderVertices.push_back(lbr);
    borderVertices.push_back(ltr);

    sf::Vertex rtl;
    rtl.position = {right() - HALFBORDERSIZE, top() + HALFBORDERSIZE};
    rtl.texCoords = {tileSet->getCoordsFor("r").x, tileSet->getCoordsFor("r").y};
    sf::Vertex rtr;
    rtr.position = {right() + HALFBORDERSIZE, top() + HALFBORDERSIZE};
    rtr.texCoords = {tileSet->getCoordsFor("r").x + 16.f, tileSet->getCoordsFor("r").y};
    sf::Vertex rbl;
    rbl.position = {right() - HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    rbl.texCoords = {tileSet->getCoordsFor("r").x, tileSet->getCoordsFor("r").y + 16.f};
    sf::Vertex rbr;
    rbr.position = {right() + HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    rbr.texCoords = {tileSet->getCoordsFor("r").x + 16.f, tileSet->getCoordsFor("r").y + 16.f};
    borderVertices.push_back(rtl);
    borderVertices.push_back(rtr);
    borderVertices.push_back(rbl);
    borderVertices.push_back(rbl);
    borderVertices.push_back(rbr);
    borderVertices.push_back(rtr);
    
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

    sf::Vertex btl;
    btl.position = {left() + HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    btl.texCoords = {tileSet->getCoordsFor("b").x, tileSet->getCoordsFor("b").y};
    sf::Vertex btr;
    btr.position = {right() - HALFBORDERSIZE, bottom() - HALFBORDERSIZE};
    btr.texCoords = {tileSet->getCoordsFor("b").x + 16.f, tileSet->getCoordsFor("b").y};
    sf::Vertex bbl;
    bbl.position = {left() + HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    bbl.texCoords = {tileSet->getCoordsFor("b").x, tileSet->getCoordsFor("b").y + 16.f};
    sf::Vertex bbr;
    bbr.position = {right() - HALFBORDERSIZE, bottom() + HALFBORDERSIZE};
    bbr.texCoords = {tileSet->getCoordsFor("b").x + 16.f, tileSet->getCoordsFor("b").y + 16.f};
    borderVertices.push_back(btl);
    borderVertices.push_back(btr);
    borderVertices.push_back(bbl);
    borderVertices.push_back(bbl);
    borderVertices.push_back(bbr);
    borderVertices.push_back(btr);

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
}

sf::Color UIBackground::getColor() { return color; }

void UIBackground::draw()
{
    game->getWindow()->draw(background);

    game->getWindow()->getWindow().draw(&borderVertices[0], borderVertices.size(), sf::PrimitiveType::Triangles, borderStates);
}

void UIBackground::resize(sf::Vector2f newSize, int posSet)
{
    if (posSet != -1) this->posSet = posSet;

    background.setPosition({left(), top()});
    background.setSize(newSize);
    
    size = newSize;

    createBorderVertices();
}