#include "game.hpp"

Game::Game() {}

void Game::init()
{
    window.create({1200, 1200}, "INFINITE", false, 165, sf::Color(10, 10, 12));

    eventHandler.init(window, camera);

    camera.init(window, true, {0, 0}, toV2F(window.getSize()));

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
    rect.setPosition(toV2F(window.getSize().x / 2, window.getSize().y / 2));

    outline.setSize({480, 480});
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(5.f);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOrigin({240.f, 240.f});
    outline.setPosition(toV2F(window.getSize().x / 2, window.getSize().y / 2));

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
    camera.update(dt);
    window.setView(camera.getView());
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