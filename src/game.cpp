#include "game.hpp"

Game::Game() {}

void Game::init()
{
    window.create({1200, 1200}, "INFINITE", false, 165, sf::Color(10, 10, 12));

    eventHandler.init(window);

    paused = false;

    ticksPerSecond = 20;

    run();
}

void Game::run()
{
    float ticksToProcess = 0.f;
    
    rect.setSize({100.f, 100.f});
    rect.setFillColor(sf::Color::Red);
    rect.setOrigin({50.f, 50.f});
    rect.setPosition({static_cast<float>(window.getWindow().getSize().x) / 2.f, static_cast<float>(window.getWindow().getSize().y) / 2.f});

    outline.setSize({480, 480});
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(5.f);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOrigin({240.f, 240.f});
    outline.setPosition({static_cast<float>(window.getWindow().getSize().x) / 2.f, static_cast<float>(window.getWindow().getSize().y) / 2.f});

    while (window.getWindow().isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        eventHandler.processEvents();

        float fps = 1.f / dt;

        if (!paused)
        {
            ticksToProcess += (dt * 1000) / (1000 / ticksPerSecond);

            while (ticksToProcess >= 1.f)
            {
                tick();

                ticksToProcess -= 1.f;
            }
        }

        update(dt);

        draw();
    }

    exit();
}

void Game::tick()
{

}

void Game::update(float dt)
{
    window.update(dt);
}

void Game::draw()
{
    window.clear();

    window.draw(rect);
    window.draw(outline);

    window.display();
}

void Game::exit()
{
    window.exit();
}