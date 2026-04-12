#include "game.hpp"
#include "../utils/utils.hpp"

Game::Game() {}

void Game::init()
{
    window.create({800, 800}, "INFINITE", false, 60, sf::Color(10, 10, 12));

    input.init(this);

    scene.init(this);

    eventHandler.init(&window, scene.getCamera(), this, &scene);

    paused = false;

    ticksPerSecond = 60;
    secondsPerTick = 1.f / ticksPerSecond;

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
            update();
        }
    }
    else
    {
        scene.sceneInput(control);
    }
}

Window* Game::getWindow() { return &window; }

Gamerules* Game::getGamerules() { return &gamerules; }

Settings* Game::getSettings() { return &settings; }

AssetManager* Game::getAssetManager() { return &assetManager; }

Input* Game::getInput() { return &input; }

Scene* Game::getScene() { return &scene; }

void Game::run()
{
    float ticksToProcess = 0.f;
    
    int ticksLastSecond = 0;
    float secondsToPrintStats = 1.f;
    float secondsSinceStatPrint = secondsToPrintStats;

    const int sampleSize = 60;
    float frameTimes[sampleSize];
    int currentFrame = 0;

    while (window.getWindow().isOpen())
    {
        dt = dtClock.restart().asSeconds();
        secondsSinceStatPrint += dt;
        frameTimes[currentFrame % sampleSize] = dt;
        currentFrame++;
        float averageDt = 0.f;
        for (float t : frameTimes) averageDt += t;
        averageDt /= sampleSize;
        float smoothFps = 1.f / averageDt;
        float fps = 1.f / dt;
        float dtick = tickClock.restart().asSeconds();

        if (secondsSinceStatPrint > secondsToPrintStats)
        {
            std::cout << "dt: " << dt << "; dtick: " << dtick << '\n';
            std::cout << "average dt: " << averageDt << '\n';
            std::cout << "fps: " << fps << "; tps: " << 1.f / dtick << '\n';
            std::cout << "ticks last second: " << ticksLastSecond << '\n';
            std::cout << "time: " << gameClock.getElapsedTime().asSeconds() << '\n';
            std::cout << "///////////////////////////////////\n";

            ticksLastSecond = 0;
            secondsSinceStatPrint = 0;
        }
        
        input.update();
        eventHandler.processEvents();

        if (lastWindowSize != window.getSize())
        {
            eventHandler.windowResized();
            lastWindowSize = window.getSize();
        }

        if (!paused)
        {
            ticksToProcess += dt;
            alpha = ticksToProcess / secondsPerTick;

            while (ticksToProcess >= secondsPerTick)
            {
                tick();

                ticksLastSecond++;
                ticksToProcess -= secondsPerTick;
            }

            update();
        }

        assetManager.updateGlobalAnimations(dt);
        scene.UIUpdate(dt);
        scene.chunkLoadUpdate();
        scene.getUILayer()->getElement("fps display")->getAsText()->setValue(std::to_string(toInt(std::round(smoothFps))));

        draw();
    }

    exit();
}

void Game::tick()
{
    scene.tick();
}

void Game::update()
{
    scene.update(dt);
}

void Game::draw()
{
    window.clear();

    scene.draw(alpha);

    window.display();
}