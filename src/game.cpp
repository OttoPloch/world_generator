#include "game.hpp"

Game::Game() {}

void Game::init()
{
    window.create({800, 800}, "INFINITE", false, 165, sf::Color(10, 10, 12));

    scene.init(&window, &assetManager);

    eventHandler.init(window, *scene.getCamera(), *this);

    paused = false;

    ticksPerSecond = 30;

    run();
}

void Game::run()
{
    float ticksToProcess = 0.f;
    
    float averageTps = 0.f;
    int tpsCount = 0;

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
                float dtick = tickClock.restart().asSeconds();

                averageTps *= tpsCount;
                averageTps += (1.f / dtick);
                tpsCount++;
                averageTps /= tpsCount;
                
                std::cout << "dt: " << dt << "; dtick: " << dtick << '\n';
                std::cout << "fps: " << fps << "; tps: " << 1.f / dtick << '\n';
                std::cout << "average tps: " << averageTps << '\n';

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
    scene.tick();
}

void Game::update(float dt)
{
    scene.update(dt);

    window.setView(scene.getCamera()->getView());
}

void Game::draw()
{
    window.clear();

    scene.draw(&window.getWindow());

    window.display();
}

void Game::exit()
{
    window.exit();
}

void Game::keyPress(sf::Keyboard::Key key)
{
    switch(key)
    {
        case sf::Keyboard::Key::Escape:
            exit();
            break;
        case sf::Keyboard::Key::Enter:
            scene.getCamera()->resetZoom();    
            break;
        case sf::Keyboard::Key::Tab:
            paused = !paused;
            break;
        case sf::Keyboard::Key::Right:
            if (paused) tick();
            break;       
        case sf::Keyboard::Key::F1:
            //(scene.getCamera()->getFocus() != nullptr) ? scene.getCamera()->removeFocus() : scene.getCamera()->setFocus(&thing2);
            break;
        default:
            break;
    }
}