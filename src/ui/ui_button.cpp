#include "ui_button.hpp"
#include "../core/game.hpp"

UIButton::UIButton() : UIElement() {}

UIButton::UIButton(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, UIElement* parent) : UIElement(game, uiLayer, name, ID, posSet, position, size, parent)
{
    init(buttonTextures);
}

void UIButton::init(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, UIElement* parent)
{
    baseInit(game, uiLayer, name, ID, posSet, position, size, parent);

    init(buttonTextures);
}

void UIButton::init(std::array<sf::Texture*, 3> buttonTextures)
{
    this->buttonTextures = buttonTextures;

    button = std::make_unique<sf::Sprite>(*buttonTextures[0]);
    
    button->setPosition({left(), top()});
    button->setScale({size.x / button->getTextureRect().size.x, size.y / button->getTextureRect().size.y});

    active = false;
}

bool UIButton::hover()
{
    if (uiLayer->interactiveUIManager.isControllerUIActive())
    {
        if (uiLayer->interactiveUIManager.getSelectedElement())
        {
            return (uiLayer->interactiveUIManager.getSelectedElement()->getID() == ID);
        }

        return false;
    }
    else
    {
        return mouseRectCollide(game, {left(), top()}, size);
    }
}

bool UIButton::pressed()
{
    if (uiLayer->interactiveUIManager.isControllerUIActive())
    {
        return (hover() && game->getInput()->getControl("INTERACT"));
    }
    else
    {
        return (hover() && game->getInput()->getKey("LEFTCLICK"));
    }
}

bool UIButton::clicked()
{
    if (uiLayer->interactiveUIManager.isControllerUIActive())
    {
        // controller input goes through sceneInput()
        return false;
    }
    else
    {
        if (game->getInput()->getKey("LEFTCLICK")) std::cout << game->getInput()->getKeyPressedLastFrame("LEFTCLICK") << '\n';

        return (hover() && game->getInput()->getKey("LEFTCLICK") && !game->getInput()->getKeyPressedLastFrame("LEFTCLICK"));
    }
}

void UIButton::activate()
{
    active = true;
}

bool UIButton::getActive() { return active; }

UIButton* UIButton::getAsButton() { return this; }

void UIButton::switchTexture(int index)
{
    button->setTexture(*buttonTextures[index], true);
    button->setScale({size.x / button->getTextureRect().size.x, size.y / button->getTextureRect().size.y});
}

void UIButton::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;

    button->setPosition({left(), top()});
}

void UIButton::update()
{
    active = false;
    
    if (hover() && pressed())
    {
        switchTexture(2);
        
        if (clicked())
        {
            activate();
        }
    }
    else if (hover())
    {
        switchTexture(1);
    }
    else
    {
        switchTexture(0);
    }
}

void UIButton::draw()
{
    game->getWindow()->draw(*button);
}