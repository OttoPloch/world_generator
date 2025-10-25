#include "camera.hpp"

#include "window.hpp"

const int FREECAM_MOVE_SPEED = 10;

Camera::Camera() {}

void Camera::init(Window& window, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size)
{
    this->window = &window;

    this->size = size;

    velocity = {0, 0};

    if (setTopLeftPos)
    {
        setTopLeft(position);
    }
    else
    {
        setCenter(position);
    }

    view.setCenter(center);
    view.setSize(size);
}

void Camera::setCenter(sf::Vector2f center)
{
    this->center = center;
}

void Camera::setTopLeft(sf::Vector2f topLeft)
{
    center = {
        topLeft.x + size.x / 2,
        topLeft.y + size.y / 2
    };
}

sf::View Camera::getView()
{
    return view;
}

sf::Vector2f Camera::getCenter()
{
    return center;
}

sf::Vector2f Camera::getTopLeft()
{
    return {center.x - size.x / 2, center.y - size.y / 2};
}

void Camera::update(float dt)
{
    if (getMovement().x == 0)
    {
        velocity.x *= 1 - (dt * 8);
    }
    else
    {
        velocity.x = FREECAM_MOVE_SPEED * getMovement().x;
    }

    if (getMovement().y == 0)
    {
        velocity.y *= 1 - (dt * 8);
    }
    else
    {
        velocity.y = FREECAM_MOVE_SPEED * getMovement().y;
    }

    center.x += velocity.x * 100 * dt;
    center.y += velocity.y * 100 * dt;

    view.setCenter(center);
    view.setSize(toV2F(window->getSize()));
}

void Camera::setVelocity(sf::Vector2f newVelocity)
{
    velocity = newVelocity;
}

void Camera::setVelocity(char direction, float newVelocity)
{
    if (direction == 'x')
    {
        velocity.x = newVelocity;
    }
    else if (direction == 'y')
    {
        velocity.y = newVelocity;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

void Camera::changeVelocity(sf::Vector2f amount)
{
    velocity += amount;
}

void Camera::changeVelocity(char direction, float amount)
{
    if (direction == 'x')
    {
        velocity.x += amount;
    }
    else if (direction == 'y')
    {
        velocity.y += amount;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

void Camera::setSize(sf::Vector2f newSize)
{
    size = newSize;

    view.setSize(size);
}