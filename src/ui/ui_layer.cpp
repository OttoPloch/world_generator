#include "ui_layer.hpp"
#include "../core/game.hpp"
#include "ui_element.hpp"
#include "components/text_component.hpp"
#include "components/background_component.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <algorithm>

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;
    this->assetManager = game->getAssetManager();
    this->camera = camera;

    UIView.setSize(camera->getSize());

    IDCounter = 0;

    int currID;

    auto e = elements.emplace_back(std::make_unique<UIElement>(game, "test", UIPosition({0, 0}), 1)).get();
    auto e2 = elements.emplace_back(std::make_unique<UIElement>(game, "test 2", UIPosition({400, 400}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT, true), 0)).get();

    e->addComponent<BackgroundComponent>(game, e, UIPosition({0, 0}), "bg", sf::Vector2f(400, 100), sf::Color(30, 30, 30, 180));
    e2->addComponent<BackgroundComponent>(game, e2, UIPosition({0, 0}), "bg", sf::Vector2f(30, 30), sf::Color(255, 0, 0));

    e->addComponent<TextComponent>(game, e, UIPosition({0, 0}), "text", "Hello, World!", assetManager->getFont("sfml_font"), 30);
    e2->addComponent<TextComponent>(game, e2, UIPosition({0, 0}), "text", "(0_0)", assetManager->getFont("sfml_font"), 10);
    
    // std::array<sf::Texture*, 3> buttonTextures = {assetManager->getTexture("button_up", "images/ui/"), assetManager->getTexture("button_hover", "images/ui/"), assetManager->getTexture("button_down", "images/ui/")};
    // std::array<sf::Texture*, 3> blueButtonTextures = {assetManager->getTexture("blue_button_up", "images/ui/"), assetManager->getTexture("blue_button_hover", "images/ui/"), assetManager->getTexture("blue_button_down", "images/ui/")};
    // std::array<sf::Texture*, 3> redButtonTextures = {assetManager->getTexture("red_button_up", "images/ui/"), assetManager->getTexture("red_button_hover", "images/ui/"), assetManager->getTexture("red_button_down", "images/ui/")};
    // std::array<sf::Texture*, 3> greenButtonTextures = {assetManager->getTexture("green_button_up", "images/ui/"), assetManager->getTexture("green_button_hover", "images/ui/"), assetManager->getTexture("green_button_down", "images/ui/")};
    
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "fps display", currID, 0, toV2F(20, 20), assetManager->getFont("sfml_font"), "FPS: ###", 30, sf::Color::White);
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "chunk pos display", currID, 0, toV2F(20, 60), assetManager->getFont("sfml_font"), "Chunk Position: ###", 30, sf::Color::White);
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "mouse chunk pos display", currID, 0, toV2F(20, 100), assetManager->getFont("sfml_font"), "Mouse Chunk Position: ###", 30, sf::Color::White);
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "mouse tile type display", currID, 0, toV2F(20, 140), assetManager->getFont("sfml_font"), "Mouse Top Tile Type: ###", 30, sf::Color::White);
    
    // currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "win 1", currID, 0, toV2F(-280, 300), toV2F(220, 240), sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll", "images/ui/"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "win 1 title", currID, 5, toV2F(0, 0), assetManager->getFont("White Storm"), "Speed adjust", 30, sf::Color::Black, getElement("win 1"));
    
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "faster button", currID, 3, toV2F(-30, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "faster button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go faster!", 20, sf::Color::Black, getElement("faster button"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "slower button", currID, 2, toV2F(30, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "slower button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go slower!", 20, sf::Color::Black, getElement("slower button"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "reset button", currID, 7, toV2F(-20, -25), toV2F(30, 30), blueButtonTextures, getElement("win 1"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "reset button text", currID, 5, toV2F(0, -30), assetManager->getFont("White Storm"), "Reset", 20, sf::Color::Black, getElement("reset button"));
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "speed display", currID, 0, toV2F(15, 40), assetManager->getFont("White Storm"), "Speed: ###", 30, sf::Color::Black, getElement("win 1"));

    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button 2", currID, 0, toV2F(-75, 200), toV2F(50, 50), blueButtonTextures);
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button", currID, 0, toV2F(25, 200), toV2F(50, 50), blueButtonTextures);
    
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "action set attack button", currID, 2, toV2F(25, -25), toV2F(100, 100), redButtonTextures);
    // currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "action set heal button", currID, 2, toV2F(175, -25), toV2F(100, 100), greenButtonTextures);
    // currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "action set text", currID, 2, toV2F(25, -150), assetManager->getFont("White Storm"), "Set Main Action 'attack!' or 'heal!'", 25, sf::Color::Black);
    

    // if (auto e = game->getScene()->getEntityLayer()->getEntity(0)->getComponent<MovementComponent>())
    // {
    //     getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
    // }

    // currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "CONTROLLER_INDICATOR", getNewID(), 0, toV2F(0, 0), toV2F(50, 50), sf::Color::Transparent, assetManager->getTileSet("16px"), assetManager->getTexture("ui_select", "images/ui/"), 36.f);
    // interactiveUIManager.init(game, &elements, getElement("CONTROLLER_INDICATOR"));

    for (auto& e : elements)
    {
        auto bb = e->getGlobalBounds();

        std::cout << e->name << "; " << bb.position.x << ", " << bb.position.y << "; " << bb.size.x << ", " << bb.size.y << '\n';

        std::array<sf::Vertex, 8> verts = VertexGroup::createLineVerts(bb.position, bb.size, sf::Color::Green);

        if (e->position.worldPosition)
        {
            debugWorldElementBoundingBoxes.insert(debugWorldElementBoundingBoxes.end(), verts.begin(), verts.end());
        }
        else
        {
            debugScreenElementBoundingBoxes.insert(debugScreenElementBoundingBoxes.end(), verts.begin(), verts.end());
        }
    }

    updateVisuals();
}

UIElement* UILayer::getElement(std::string name)
{
    for (auto& e : elements)
    {
        if (e->name == name)
        {
            return e.get();
        }
    }

    return nullptr;
}

int UILayer::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

bool UILayer::checkUICollision()
{
    return false;
}

void UILayer::updateVisuals()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());
    UIView.setCenter({viewSize.x / 2.f, viewSize.y / 2.f});
    UIView.setSize(viewSize);

    if (elements.size() > 0)
    {
        for (auto& e : elements)
        {
            e->updateVisuals();
        }
    }
}

void UILayer::UIUpdate(float dt)
{
    if (elements.size() > 0)
    {
        for (auto& e : elements)
        {
            //e->baseUpdate(dt);
        }
    }

    // if (interactiveUIManager.isControllerUIActive())
    // {
    //     interactiveUIManager.updateIndicatorPosition();
    // }
}

void UILayer::draw(bool debug)
{
    std::vector<UIElement*> visibleWorldElements;
    std::vector<UIElement*> visibleScreenElements;

    for (auto& e : elements)
    {
        if (e->position.worldPosition && isOnScreen(game, e->getGlobalBounds(), true))
        {
            visibleWorldElements.push_back(e.get());
        }
        else if (!e->position.worldPosition && isOnScreen(game, e->getGlobalBounds(), false))
        {
            visibleScreenElements.push_back(e.get());
        }
    }



    // WORLD ELEMENTS

    std::sort(visibleWorldElements.begin(), visibleWorldElements.end(), [](UIElement* a, UIElement* b) {
        if (a->z != b->z)
        {
            return a->z < b->z;
        }
        else
        {
            sf::FloatRect aGB = a->getGlobalBounds();
            sf::FloatRect bGB = b->getGlobalBounds();

            return aGB.position.y + aGB.size.y < bGB.position.y + bGB.size.y;
        }
    });

    for (auto e : visibleWorldElements)
    {
        e->draw();
    }

    if (debug)
    {
        game->getWindow()->getWindow().draw(debugWorldElementBoundingBoxes.data(), debugWorldElementBoundingBoxes.size(), sf::PrimitiveType::Lines);
    }


    // SCREEN ELEMENTS

    game->getWindow()->setView(UIView);
    
    std::sort(visibleScreenElements.begin(), visibleScreenElements.end(), [](UIElement* a, UIElement* b){
        if (a->z != b->z)
        {
            return a->z < b->z;
        }
        else
        {
            sf::FloatRect aGB = a->getGlobalBounds();
            sf::FloatRect bGB = b->getGlobalBounds();

            return aGB.position.y + aGB.size.y < bGB.position.y + bGB.size.y;
        }
    });
    
    for (auto e : visibleScreenElements)
    {
        if (e->name != "CONTROLLER_INDICATOR") e->draw();
    }

    // interactiveUIManager.draw();

    if (debug)
    {
        game->getWindow()->getWindow().draw(debugScreenElementBoundingBoxes.data(), debugScreenElementBoundingBoxes.size(), sf::PrimitiveType::Lines);
    }

    game->getWindow()->setView(camera->getView());
}