#include "ui_layer.hpp"
#include "../core/game.hpp"
#include "../entities/components/movement_component.hpp"
#include "ui_background.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    IDCounter = 0;

    int currID;
    
    std::array<sf::Texture*, 3> buttonTextures = {assetManager->getTexture("button_up", "images/ui/"), assetManager->getTexture("button_hover", "images/ui/"), assetManager->getTexture("button_down", "images/ui/")};
    std::array<sf::Texture*, 3> blueButtonTextures = {assetManager->getTexture("blue_button_up", "images/ui/"), assetManager->getTexture("blue_button_hover", "images/ui/"), assetManager->getTexture("blue_button_down", "images/ui/")};
    
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "fps display", currID, 0, toV2F(20, 20), assetManager->getFont("sfml_font"), "FPS: ###", 30, sf::Color::White);
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "chunk pos display", currID, 0, toV2F(20, 60), assetManager->getFont("sfml_font"), "Chunk Position: ###", 30, sf::Color::White);
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "mouse chunk pos display", currID, 0, toV2F(20, 100), assetManager->getFont("sfml_font"), "Mouse Chunk Position: ###", 30, sf::Color::White);
    
    currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "win 1", currID, 0, toV2F(-280, 250), toV2F(220, 240), sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll", "images/ui/"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "win 1 title", currID, 5, toV2F(0, 0), assetManager->getFont("White Storm"), "Speed adjust", 30, sf::Color::Black, getElement("win 1"));
    
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "faster button", currID, 3, toV2F(-30, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "faster button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go faster!", 20, sf::Color::Black, getElement("faster button"));
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "slower button", currID, 2, toV2F(30, -10), toV2F(50, 50), buttonTextures, getElement("win 1"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "slower button text", currID, 5, toV2F(0, -45), assetManager->getFont("White Storm"), "Click me\nto go slower!", 20, sf::Color::Black, getElement("slower button"));
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "reset button", currID, 7, toV2F(-20, -25), toV2F(30, 30), blueButtonTextures, getElement("win 1"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "reset button text", currID, 5, toV2F(0, -30), assetManager->getFont("White Storm"), "Reset", 20, sf::Color::Black, getElement("reset button"));
    currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "speed display", currID, 0, toV2F(15, 40), assetManager->getFont("White Storm"), "Speed: ###", 30, sf::Color::Black, getElement("win 1"));

    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button 2", currID, 0, toV2F(-75, 140), toV2F(50, 50), blueButtonTextures);
    currID = getNewID(); elements[currID] = std::make_unique<UIButton>(game, this, "animation button", currID, 0, toV2F(25, 140), toV2F(50, 50), blueButtonTextures);
    
    // currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "many win parent", currID, 0, toV2F(80, 80), toV2F(50, 50), sf::Color(0, 0, 0, 0), assetManager->getTileSet("16px"), assetManager->getTexture("ui_tech", "images/ui/"));
    // for (int i = 0; i < 100; i++)
    // {
    //     UIElement* parent = getElement("many win parent");

    //     if (i > 0) parent = getElement("ma " + std::to_string(i - 1));

    //     currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "ma " + std::to_string(i), currID, 0, toV2F(20, 20), toV2F(100, 150), sf::Color(220, 220, 220), assetManager->getTileSet("16px"), assetManager->getTexture("ui_gray", "images/ui/"), 48, parent);
    //     currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "ma " + std::to_string(i) + " text 1", currID, 0, toV2F(0, 0), assetManager->getFont("White Storm"), "Performance test", 20, sf::Color::Black, getElement("ma " + std::to_string(i)));
    //     currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "ma " + std::to_string(i) + " text 2", currID, 0, toV2F(0, 30), assetManager->getFont("White Storm"), "Performance test", 20, sf::Color::Black, getElement("ma " + std::to_string(i)));
    //     currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "ma " + std::to_string(i) + " text 3", currID, 0, toV2F(0, 60), assetManager->getFont("White Storm"), "Performance test", 20, sf::Color::Black, getElement("ma " + std::to_string(i)));
    //     currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "ma " + std::to_string(i) + " text 4", currID, 0, toV2F(0, 90), assetManager->getFont("White Storm"), "Performance test", 20, sf::Color::Black, getElement("ma " + std::to_string(i)));
    //     currID = getNewID(); elements[currID] = std::make_unique<UIText>(game, this, "ma " + std::to_string(i) + " text 5", currID, 0, toV2F(0, 120), assetManager->getFont("White Storm"), "Performance test", 20, sf::Color::Black, getElement("ma " + std::to_string(i)));
    // }

    if (auto e = game->getScene()->getEntityLayer()->getEntity(0)->getComponent<MovementComponent>())
    {
        getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
    }

    currID = getNewID(); elements[currID] = std::make_unique<UIBackground>(game, this, "controller indicator", getNewID(), 0, toV2F(0, 0), toV2F(50, 50), sf::Color::Transparent, assetManager->getTileSet("16px"), assetManager->getTexture("ui_select", "images/ui/"), 36.f);
    interactiveUIManager.init(game, &elements, getElement("controller indicator"));

    reset();
}

UIElement* UILayer::getElement(std::string name)
{
    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            if (i.second->getName() == name)
            {
                return elements[i.first].get();
            }
        }
    }

    return nullptr;
}

int UILayer::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

sf::Vector2u UILayer::getScreenSize()
{
    return game->getWindow()->getSize();
}

sf::Vector2f UILayer::getViewSize()
{
    return game->getWindow()->getWindow().getView().getSize();
}

void UILayer::reset()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);

    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            i.second->updateSize();
        }
    }
}

void UILayer::UIUpdate(float dt)
{
    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            i.second->baseUpdate(dt);
        }
    }

    if (interactiveUIManager.isControllerUIActive())
    {
        interactiveUIManager.updateIndicatorPosition();
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (elements.size() > 0)
    {
        for (auto& i : elements)
        {
            sf::FloatRect bb = i.second->getBoundingBox();

            if (isOnScreen(game, bb.position, bb.size, false))
            {
                if (i.second->getName() != "controller indicator")
                {
                    i.second->draw();
                }
            }
        }
    }

    interactiveUIManager.draw();

    game->getWindow()->setView(camera->getView());
}