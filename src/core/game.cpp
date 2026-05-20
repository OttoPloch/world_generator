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

void Game::processInput(std::string control, bool justPressed)
{
    if (control == "EXIT")
    {
        exit();
    }
    else if (control == "PAUSE" && justPressed)
    {
        paused = !paused;
    }
    else if (control == "STEP" && justPressed)
    {
        if (paused)
        {
            tick();
            update();
        }
    }
    else
    {
        scene.sceneInput(control, justPressed);
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
        runBlame.clear();
        debugClock.restart();

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
            //std::cout << "FPS (smoothed): " << smoothFps << '\n';

            // std::cout << "dt: " << dt << "; dtick: " << dtick << '\n';
            // std::cout << "average dt: " << averageDt << '\n';
            // std::cout << "fps: " << fps << "; tps: " << 1.f / dtick << '\n';
            // std::cout << "ticks last second: " << ticksLastSecond << '\n';
            // std::cout << "time: " << gameClock.getElapsedTime().asSeconds() << '\n';
            // std::cout << "///////////////////////////////////\n";

            ticksLastSecond = 0;
            secondsSinceStatPrint = 0;
        }
        
        runBlame["STAT TRACKING"] = debugClock.restart().asSeconds();

        input.update();
        eventHandler.processEvents();

        runBlame["INPUT/EVENTS"] = debugClock.restart().asSeconds();

        if (lastWindowSize != window.getSize())
        {
            eventHandler.windowResized();
            lastWindowSize = window.getSize();
        }

        runBlame["WINDOW RESIZE"] = debugClock.restart().asSeconds();

        if (!paused)
        {
            ticksToProcess += dt;
            
            debugClock.restart();

            while (ticksToProcess >= secondsPerTick)
            {
                tick();

                ticksLastSecond++;
                ticksToProcess -= secondsPerTick;
            }

            runBlame["TICK"] = debugClock.restart().asSeconds();

            update();

            runBlame["UPDATE"] = debugClock.restart().asSeconds();
        }
        else
        {
            runBlame["TICK"] = 0;
            runBlame["UPDATE"] = 0;
        }

        scene.UIUpdate(dt);
        scene.chunkLoadUpdate();
        // scene.getUILayer()->getElement("fps display")->getAsText()->setValue(std::to_string(toInt(std::round(smoothFps))));

        runBlame["UI/CHUNK LOAD UPDATE"] = debugClock.restart().asSeconds();

        draw();

        runBlame["DRAW"] = debugClock.restart().asSeconds();

        input.shiftPressedThisFrame();

        if (scene.debugMode) printBlameStats(runBlame, "GAME_RUN");
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

    assetManager.updateGlobalAnimations(dt);
}

void Game::draw()
{
    debugClock.restart();

    window.clear();
    drawBlame["DRAW_CLEAR"] = debugClock.restart().asSeconds();
    
    scene.draw();
    drawBlame["DRAW_SCENE"] = debugClock.restart().asSeconds();
    
    window.display();
    drawBlame["DRAW_DISPLAY"] = debugClock.restart().asSeconds();

    if (scene.debugMode) printBlameStats(drawBlame, "GAME_DRAW");
}