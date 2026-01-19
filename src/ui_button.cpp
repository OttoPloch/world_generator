#include "ui_button.hpp"
#include "game.hpp"

UIButton::UIButton() : UIElement() {}

UIButton::UIButton(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, std::string parentName) : UIElement()
{
    baseInit(game, name, ID, posSet, position, size, parentName);

    init(buttonTextures);
}

void UIButton::init(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, std::string parentName)
{
    baseInit(game, name, ID, posSet, position, size, parentName);

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
        return (uiLayer->interactiveUIManager.getSelectedElementID() == ID);
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
        return (hover() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
    }
}

bool UIButton::clicked()
{
    if (uiLayer->interactiveUIManager.isControllerUIActive())
    {
        return false;
    }
    else
    {
        return (hover() && game->getInput()->leftClick());
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