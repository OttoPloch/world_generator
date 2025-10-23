#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "INFINITE");
    window.setFramerateLimit(165);

    sf::CircleShape circle(250.f);
    circle.setFillColor(sf::Color::Green);
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }

        window.clear(sf::Color(10, 10, 10));

        window.draw(circle);

        window.display();
    }
}
