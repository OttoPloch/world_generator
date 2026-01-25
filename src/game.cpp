#include "game.hpp"

Game::Game() {}

void Game::init()
{
    window.create({800, 800}, "INFINITE", false, 60, sf::Color(10, 10, 12));

    input.init(this);

    scene.init(this);

    eventHandler.init(&window, scene.getCamera(), this, &scene);

    paused = false;

    ticksPerSecond = 60;

    lastWindowSize = window.getSize();

    run();
}

void Game::exit()
{
    window.exit();
}

void Game::processInput(std::string control)
{
    if (control == "EXIT")
    {
        exit();
    }
    else if (control == "PAUSE")
    {
        paused = !paused;
    }
    else if (control == "STEP")
    {
        if (paused)
        {
            tick();
            update(dt);
        }
    }
    else
    {
        scene.sceneInput(control);
    }
}

Window* Game::getWindow() { return &window; }

Gamerules* Game::getGamerules() { return &gamerules; }

AssetManager* Game::getAssetManager() { return &assetManager; }

Input* Game::getInput() { return &input; }

Scene* Game::getScene() { return &scene; }

void Game::run()
{
    float ticksToProcess = 0.f;
    
    float averageTps = 0.f;
    int tpsCount = 0;

    float lastTimeCount = 0.f;
    int ticksLastSecond = 0;

    while (window.getWindow().isOpen())
    {
        dt = dtClock.restart().asSeconds();

        input.update();

        eventHandler.processEvents();

        float fps = 1.f / dt;

        if (!paused)
        {
            ticksToProcess += (dt * 1000) / (1000 / ticksPerSecond);

            while (ticksToProcess >= 1.f)
            {
                float dtick = tickClock.restart().asSeconds();

                averageTps *= tpsCount;
                averageTps += (1.f / dtick);
                tpsCount++;
                averageTps /= tpsCount;
                
                if (std::fmod(gameClock.getElapsedTime().asSeconds(), 1.f) < lastTimeCount)
                {
                    // std::cout << "dt: " << dt << "; dtick: " << dtick << '\n';
                    // std::cout << "fps: " << fps << "; tps: " << 1.f / dtick << '\n';
                    // std::cout << "average tps: " << averageTps << '\n';
                    // std::cout << "ticks last second: " << ticksLastSecond << '\n';
                    // std::cout << "time: " << gameClock.getElapsedTime().asSeconds() << '\n';
                    // std::cout << "//////////////////////////////////\n";

                    ticksLastSecond = 0;
                }
                
                lastTimeCount = std::fmod(gameClock.getElapsedTime().asSeconds(), 1.f);

                tick();

                ticksLastSecond++;

                ticksToProcess -= 1.f;
            }

            update(dt);
        }

        UIUpdate();

        if (lastWindowSize != window.getSize())
        {
            std::cout << lastWindowSize.x << ", " << lastWindowSize.y << " old new: " << window.getSize().x << ", " << window.getSize().y << '\n';

            eventHandler.windowResized();

            lastWindowSize = window.getSize();
        }

        draw();
    }

    exit();
}

void Game::tick()
{
    scene.tick();
}

void Game::update(float dt)
{
    scene.update(dt);
}

void Game::UIUpdate()
{
    scene.UIUpdate();
}

void Game::draw()
{
    window.clear();

    scene.draw();

    window.display();
}