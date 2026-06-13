#include "ui_layer.hpp"
#include "../core/game.hpp"
#include "animations/ui_animation.hpp"
#include "components/button_component.hpp"
#include "components/image_component.hpp"
#include "ui_element.hpp"
#include "components/text_component.hpp"
#include "components/background_component.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <memory>

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;
    this->assetManager = game->getAssetManager();
    this->camera = camera;

    UIView.setSize(toV2F(game->getWindow()->getSize()));

    // auto e = elements.emplace_back(std::make_unique<UIElement>(game, "test", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_RIGHT))).get();
    // e->addComponent<BackgroundComponent>(game, e, UIPosition({0, 0}, UIOrigin::BOTTOM_RIGHT), "bg", 0, sf::Vector2f(400, 100), sf::Color(30, 30, 30, 180));
    // e->addComponent<TextComponent>(game, e, UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT), "text 1", 1, "Hello, World!", assetManager->getFont("sfml_font"), 30);

    // auto e2 = elements.emplace_back(std::make_unique<UIElement>(game, "test 2", UIPosition({200, 400}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT, true))).get();
    // e2->addComponent<BackgroundComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER), "bg 2", 0, sf::Vector2f(52, 37), sf::Color(15, 15, 15));
    // e2->addComponent<BackgroundComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER), "bg 1", 0, sf::Vector2f(50, 35), sf::Color(255, 0, 0));
    // e2->addComponent<TextComponent>(game, e2, UIPosition({0, 0}, UIOrigin::CENTER, UIAnchor::CENTER), "text", 1, "This is a\nUI element", assetManager->getFont("sfml_font"), 10);

    // auto e3 = elements.emplace_back(std::make_unique<UIElement>(game, "speed buttons", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_MIDDLE))).get();
    // e3->addComponent<BackgroundComponent>(game, e3, UIPosition({0, 0}, UIOrigin::TOP_MIDDLE), "bg", 0, sf::Vector2f(300, 120), sf::Color(30, 30, 30, 180));
    // e3->addComponent<ButtonComponent>(game, e3, UIPosition({10, 10}), "slower button", 1, game->getAssetManager()->getTexture("blue_button", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("button", "ui/"), sf::Vector2f(3, 3));
    // e3->addComponent<ButtonComponent>(game, e3, UIPosition({-10, 10}, UIOrigin::TOP_RIGHT, UIAnchor::TOP_RIGHT), "faster button", 1, game->getAssetManager()->getTexture("green_button", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("button", "ui/"), sf::Vector2f(3, 3));
    // e3->addComponent<TextComponent>(game, e3, UIPosition({0, 20}, UIOrigin::TOP_MIDDLE, UIAnchor::TOP_MIDDLE), "note", 1, "<< slower\nfaster >>", game->getAssetManager()->getFont("sfml_font"), 20);
    // e3->addComponent<TextComponent>(game, e3, UIPosition({0, -20}, UIOrigin::BOTTOM_MIDDLE, UIAnchor::BOTTOM_MIDDLE), "speed display", 1, "", game->getAssetManager()->getFont("sfml_font"), 20);

    // auto e4 = elements.emplace_back(std::make_unique<UIElement>(game, "test image", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_RIGHT))).get();
    // e4->addComponent<ImageComponent>(game, e4, UIPosition({0, 0}, UIOrigin::TOP_RIGHT), "image bee", 0, game->getAssetManager()->getTexture("dr bee"), sf::Vector2f(100, 100), false);
    // e4->addComponent<ImageComponent>(game, e4, UIPosition({0, 0}, UIOrigin::TOP_RIGHT, UIAnchor::BOTTOM_LEFT), "image", 1, game->getAssetManager()->getTexture("shaq_time_out"), sf::Vector2f(100, 100), false, game->getAssetManager()->getAnimation("dot_left"));



    auto inputNote = createElement(std::make_unique<UIElement>(game, "__inputNote", UIPosition({0, 10}, UIOrigin::TOP_LEFT, UIAnchor::TOP_MIDDLE)));
    // inputNote->addComponent<BackgroundComponent>(game, inputNote, UIPosition({0, 0}, UIOrigin::TOP_MIDDLE), "bg", 0, sf::Vector2f(500, 500), 10, game->getAssetManager()->getTexture("white_border", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("background_8px", "ui/"), true); // TEMP
    inputNote->addComponent<TextComponent>(game, inputNote, UIPosition({0, 0}, UIOrigin::TOP_MIDDLE, UIAnchor::TOP_MIDDLE), "note text", 1, "Press B on controller to navigate the UI,\nor move the right stick to control the cursor", game->getAssetManager()->getFont("White Storm"), 32);
    // inputNote->addComponent<ImageComponent>(game, inputNote, UIPosition({0, 0}, UIOrigin::TOP_MIDDLE, UIAnchor::TOP_MIDDLE), "debug img", 2, game->getAssetManager()->getTexture("pixel"), sf::Vector2f(10, 10), false);

    // auto e5 = elements.emplace_back(std::make_unique<UIElement>(game, "debug text display", UIPosition({0, 0}))).get();
    // e5->addComponent<TextComponent>(game, e5, UIPosition({10, 10}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_LEFT), "fps text", 0, "FPS: ", game->getAssetManager()->getFont("sfml_font"), 32);
    // e5->addComponent<TextComponent>(game, e5, UIPosition({10, 10}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_LEFT), "mouse chunk pos text", 1, "Mouse Chunk Pos: ", game->getAssetManager()->getFont("sfml_font"), 32);
    // e5->addComponent<TextComponent>(game, e5, UIPosition({10, 10}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_LEFT), "mouse tile type text", 2, "Mouse Tile Type: ", game->getAssetManager()->getFont("sfml_font"), 32);
    
    auto e6 = elements.emplace_back(std::make_unique<UIElement>(game, "useless button", UIPosition({0, 0}, UIOrigin::BOTTOM_LEFT, UIAnchor::BOTTOM_LEFT))).get();
    e6->addComponent<ButtonComponent>(game, e6, UIPosition({0, 0}, UIOrigin::BOTTOM_LEFT, UIAnchor::BOTTOM_LEFT), "button", 0, game->getAssetManager()->getTexture("default_button", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("button", "ui/"), sf::Vector2f(100, 100), false);
    // e6->addComponent<ButtonComponent>(game, e6, UIPosition({10, 0}, UIOrigin::BOTTOM_LEFT, UIAnchor::BOTTOM_RIGHT), "button2", 1, game->getAssetManager()->getTexture("blue_button", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("button", "ui/"), sf::Vector2f(100, 100), false);
    
    // auto e6 = elements.emplace_back(std::make_unique<UIElement>(game, "t3st", UIPosition({0, 0}))).get();
    // e6->addComponent<BackgroundComponent>(game, e6, UIPosition({0, 0}), "bg", 0, sf::Vector2f(300, 200), 40, game->getAssetManager()->getTexture("ui_default", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("background_8px", "ui/"));

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

UIElement* UILayer::createElement(std::unique_ptr<UIElement> element)
{
    if (element)
    {
        return elements.emplace_back(std::move(element)).get();
    }

    return nullptr;
}

sf::View UILayer::getUIView() { return UIView; }

bool UILayer::checkUICollision()
{
    for (auto& e : elements)
    {
        sf::Vector2f mousePos;
        
        if (e->position.worldPosition) mousePos = game->getInput()->getCursorCoords();
        else mousePos = game->getInput()->getCursorWindowPos();

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
}

void UILayer::draw(bool debug)
{
    if (debug) updateVisuals();

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
        e->draw(debug);
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
        if (e->name != "CONTROLLER_INDICATOR") e->draw(debug);
    }

    if (debug)
    {
        game->getWindow()->getWindow().draw(debugScreenComponentBoundingBoxes.data(), debugScreenComponentBoundingBoxes.size(), sf::PrimitiveType::Lines);
        game->getWindow()->getWindow().draw(debugScreenElementBoundingBoxes.data(), debugScreenElementBoundingBoxes.size(), sf::PrimitiveType::Lines);
    }

    game->getWindow()->setView(camera->getView());
}

UIElement* UILayer::getNearestElement(sf::Vector2f direction, UIElement* origin)
{
    if (direction == sf::Vector2f(0, 0)) return origin;

    sf::FloatRect bounds;
    if (origin) bounds = origin->getGlobalBounds();
    else bounds = {{0, 0}, {0, 0}};

    sf::Vector2f center = {bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2};

    UIElement* nearestElement = nullptr;
    float nearestDistance = MAXFLOAT;

    for (auto& e : elements)
    {
        if (e->position.worldPosition || e.get() == origin || e->name.substr(0, 2) == "__") continue;

        sf::FloatRect eBounds = e->getGlobalBounds();
        sf::Vector2f eCenter = {eBounds.position.x + eBounds.size.x / 2, eBounds.position.y + eBounds.size.y / 2};

        float dist = getDistance(center, eCenter);

        // if true, this element is guarenteed to not be the closest no matter the direction.
        if (dist >= nearestDistance) continue;

        // making sure this element is in the correct direction and preventing movement to an element with no diff in that direction
        sf::Vector2f diff = eCenter - center;
        if (diff.x >= 0 && direction.x < 0) continue;
        if (diff.x <= 0 && direction.x > 0) continue;
        if (diff.y >= 0 && direction.y < 0) continue;
        if (diff.y <= 0 && direction.y > 0) continue;

        nearestElement = e.get();
        nearestDistance = dist;
    }

    return nearestElement;
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