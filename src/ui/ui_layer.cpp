#include "ui_layer.hpp"
#include "../core/game.hpp"
#include "components/button_component.hpp"
#include "components/image_component.hpp"
#include "ui_element.hpp"
#include "components/text_component.hpp"
#include "components/background_component.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <algorithm>
#include <filesystem>

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;
    this->assetManager = game->getAssetManager();
    this->camera = camera;

    UIView.setSize(toV2F(game->getWindow()->getSize()));

    std::map<ButtonState, sf::Texture*> blueButtonTextures = {
        {ButtonState::UP, game->getAssetManager()->getTexture("blue_button_up", "images/ui/")},
        {ButtonState::HOVER, game->getAssetManager()->getTexture("blue_button_hover", "images/ui/")},
        {ButtonState::DOWN, game->getAssetManager()->getTexture("blue_button_down", "images/ui/")}
    };

    std::map<ButtonState, sf::Texture*> greenButtonTextures = {
        {ButtonState::UP, game->getAssetManager()->getTexture("green_button_up", "images/ui/")},
        {ButtonState::HOVER, game->getAssetManager()->getTexture("green_button_hover", "images/ui/")},
        {ButtonState::DOWN, game->getAssetManager()->getTexture("green_button_down", "images/ui/")}
    };

    auto e = elements.emplace_back(std::make_unique<UIElement>(game, "test", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_RIGHT))).get();
    e->addComponent<BackgroundComponent>(game, e, UIPosition({0, 0}, UIOrigin::BOTTOM_RIGHT), "bg", 0, sf::Vector2f(400, 100), sf::Color(30, 30, 30, 180));
    e->addComponent<TextComponent>(game, e, UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT), "text 1", 1, "Hello, World!", assetManager->getFont("sfml_font"), 30);

    auto e2 = elements.emplace_back(std::make_unique<UIElement>(game, "test 2", UIPosition({200, 400}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT, true))).get();
    e2->addComponent<BackgroundComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER), "bg 2", 0, sf::Vector2f(52, 37), sf::Color(15, 15, 15));
    e2->addComponent<BackgroundComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER), "bg 1", 0, sf::Vector2f(50, 35), sf::Color(255, 0, 0));
    e2->addComponent<TextComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER, UIAnchor::CENTER), "text", 1, "This is a\nUI element", assetManager->getFont("sfml_font"), 10);

    auto e3 = elements.emplace_back(std::make_unique<UIElement>(game, "speed buttons", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_MIDDLE))).get();
    e3->addComponent<BackgroundComponent>(game, e3, UIPosition({0, 0}, UIOrigin::TOP_MIDDLE), "bg", 0, sf::Vector2f(300, 120), sf::Color(30, 30, 30, 180));
    e3->addComponent<ButtonComponent>(game, e3, UIPosition({10, 10}), "slower button", 1, blueButtonTextures, sf::Vector2f(3, 3));
    e3->addComponent<ButtonComponent>(game, e3, UIPosition({-10, 10}, UIOrigin::TOP_RIGHT, UIAnchor::TOP_RIGHT), "faster button", 1, greenButtonTextures, sf::Vector2f(3, 3));
    e3->addComponent<TextComponent>(game, e3, UIPosition({0, 20}, UIOrigin::TOP_MIDDLE, UIAnchor::TOP_MIDDLE), "note", 1, "<< slower\nfaster >>", game->getAssetManager()->getFont("sfml_font"), 20);
    e3->addComponent<TextComponent>(game, e3, UIPosition({0, -20}, UIOrigin::BOTTOM_MIDDLE, UIAnchor::BOTTOM_MIDDLE), "speed display", 1, "", game->getAssetManager()->getFont("sfml_font"), 20);

    auto e4 = elements.emplace_back(std::make_unique<UIElement>(game, "test image", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_RIGHT))).get();
    e4->addComponent<ImageComponent>(game, e4, UIPosition({0, 0}, UIOrigin::TOP_RIGHT), "image bee", 0, game->getAssetManager()->getTexture("dr bee"), sf::Vector2f(100, 100), false);
    e4->addComponent<ImageComponent>(game, e4, UIPosition({0, 0}, UIOrigin::TOP_RIGHT, UIAnchor::BOTTOM_LEFT), "image", 1, game->getAssetManager()->getTexture("shaq_time_out"), sf::Vector2f(100, 100), false, game->getAssetManager()->getAnimation("dot_left"));

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

    setDebugVertices();
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

sf::View UILayer::getUIView() { return UIView; }

bool UILayer::checkUICollision()
{
    for (auto& e : elements)
    {
        sf::Vector2f mousePos;
        
        if (e->position.worldPosition) mousePos = game->getInput()->getMouseCoords();
        else mousePos = game->getInput()->getMouseWindowPos();

        std::vector<sf::FloatRect> componentBounds = e->getAllComponentBounds();
        for (auto b : componentBounds)
        {
            if (pointRectCollide(mousePos, b)) return true;
        }
    }

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

    setDebugVertices();
}

void UILayer::tick()
{

}

void UILayer::UIUpdate(float dt)
{
    for (auto& e : elements)
    {
        e->update(dt);
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
        game->getWindow()->getWindow().draw(debugWorldComponentBoundingBoxes.data(), debugWorldComponentBoundingBoxes.size(), sf::PrimitiveType::Lines);
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
        game->getWindow()->getWindow().draw(debugScreenComponentBoundingBoxes.data(), debugScreenComponentBoundingBoxes.size(), sf::PrimitiveType::Lines);
        game->getWindow()->getWindow().draw(debugScreenElementBoundingBoxes.data(), debugScreenElementBoundingBoxes.size(), sf::PrimitiveType::Lines);
    }

    game->getWindow()->setView(camera->getView());
}

void UILayer::setDebugVertices()
{
    debugWorldComponentBoundingBoxes.clear();
    debugWorldElementBoundingBoxes.clear();
    debugScreenComponentBoundingBoxes.clear();
    debugScreenElementBoundingBoxes.clear();

    for (auto& e : elements)
    {
        auto bb = e->getGlobalBounds();

        std::array<sf::Vertex, 8> verts = VertexGroup::createLineVerts(bb.position, bb.size, sf::Color::Green);

        if (e->position.worldPosition)
        {
            debugWorldElementBoundingBoxes.insert(debugWorldElementBoundingBoxes.end(), verts.begin(), verts.end());
            
            for (auto& c : e->components)
            {
                bb = c->getGlobalBounds();
    
                verts = VertexGroup::createLineVerts(bb.position, bb.size, sf::Color::Blue);

                debugWorldComponentBoundingBoxes.insert(debugWorldComponentBoundingBoxes.end(), verts.begin(), verts.end());
            }
        }
        else
        {
            debugScreenElementBoundingBoxes.insert(debugScreenElementBoundingBoxes.end(), verts.begin(), verts.end());
            
            for (auto& c : e->components)
            {
                bb = c->getGlobalBounds();
                
                verts = VertexGroup::createLineVerts(bb.position, bb.size, sf::Color::Blue);
                
                debugScreenComponentBoundingBoxes.insert(debugScreenComponentBoundingBoxes.end(), verts.begin(), verts.end());
            }
        }

    }
}