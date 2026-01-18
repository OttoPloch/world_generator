#include "ui_layer.hpp"
#include "game.hpp"

UILayer::UILayer() {}

void UILayer::init(Game* game, Camera* camera)
{
    this->game = game;

    this->assetManager = game->getAssetManager();

    this->camera = camera;

    IDCounter = 0;

    reset();

    int currID;

    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "win 1", currID, 8, {0, -50}, {600, 300}, sf::Color(0, 0, 0, 195), assetManager->getTileSet("16px"), assetManager->getTexture("ui_default"));
    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "", currID, 4, {0, 0}, {300, 200}, sf::Color(0, 0, 0, 0), assetManager->getTileSet("32px filled"), assetManager->getTexture("ui_scroll"), 0);
    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "", currID, 0, {20, 20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, 0);
    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "", currID, 1, {-20, 20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, 0);
    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "", currID, 2, {20, -20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, 0);
    currID = getNewID(); bgElements[currID] = UIBackground(game, this, "", currID, 3, {-20, -20}, {50, 50}, sf::Color(0, 0, 255), nullptr, nullptr, 0);
    
    currID = getNewID(); textElements[currID] = UIText(game, this, "win 1 text", currID, 0, {0, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 50, sf::Color::White, 0);
    currID = getNewID(); textElements[currID] = UIText(game, this, "win 2 tl text", currID, 0, {20, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 30, sf::Color::Black, 1);
    currID = getNewID(); textElements[currID] = UIText(game, this, "win 2 center text", currID, 4, {0, 0}, assetManager->getFont("White Storm"), "Hello Boi!", 30, sf::Color::Black, 1);
    currID = getNewID(); textElements[currID] = UIText(game, this, "win 2 bl text", currID, 2, {20, -20}, assetManager->getFont("White Storm"), "Hello Boi!", 30, sf::Color::Black, 1);

    for (auto& i : bgElements)
    {
        namesToIDs[i.second.getName()] = i.first;
    }

    for (auto& i : textElements)
    {
        namesToIDs[i.second.getName()] = i.first;
    }
}

UIElement* UILayer::getElement(int ID)
{
    if (ID > -1)
    {
        if (bgElements.find(ID) != bgElements.end())
        {
            return &bgElements[ID];
        }

        if (textElements.find(ID) != textElements.end())
        {
            return &textElements[ID];
        }
        
        // if there are no entries with that ID, assume
        // it has been deleted and remove it from namesToIDs
        for (auto& i : namesToIDs)
        {
            if (i.second == ID)
            {
                namesToIDs.erase(i.first);
            }
        }
    }

    return nullptr;
}

UIElement* UILayer::getElement(std::string name)
{
    if (namesToIDs.find(name) != namesToIDs.end())
    {
        return getElement(namesToIDs[name]);
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

void UILayer::reset()
{
    sf::Vector2f viewSize = toV2F(game->getWindow()->getSize());

    UIView = sf::View({viewSize.x / 2.f, viewSize.y / 2.f}, viewSize);

    for (auto& i : bgElements)
    {
        i.second.updateSize();
    }

    for (auto& i : textElements)
    {
        i.second.updateSize();
    }
}

void UILayer::tick()
{
    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            if (i.first == namesToIDs["win 1"])
            {
                if (game->getInput()->getControl("INTERACT"))
                {
                    i.second.resize({i.second.getSize().x + 10.f, i.second.getSize().y + 2.f});
        
                    for (auto& j : bgElements) j.second.updateSize();
                    for (auto& j : textElements) j.second.updateSize();
                }
        
                if (game->getInput()->getControl("MENU"))
                {
                    i.second.resize({i.second.getSize().x - 10.f, i.second.getSize().y - 2.f});
        
                    for (auto& j : bgElements) j.second.updateSize();
                    for (auto& j : textElements) j.second.updateSize();
                }
            }
        }
    }
}

void UILayer::draw()
{
    game->getWindow()->setView(UIView);

    if (bgElements.size() > 0)
    {
        for (auto& i : bgElements)
        {
            i.second.draw();
    
            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    if (textElements.size() > 0)
    {
        for (auto& i : textElements)
        {
            i.second.draw();
    
            // sf::CircleShape circle(5.f);
            // circle.setFillColor(sf::Color::Red);
            // circle.setOrigin({5.f, 5.f});
            // circle.setPosition(i.second.getScreenCenter());
            // game->getWindow()->getWindow().draw(circle);
        }
    }

    game->getWindow()->setView(camera->getView());
}