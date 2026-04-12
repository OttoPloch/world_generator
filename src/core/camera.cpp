#include "camera.hpp"
#include "game.hpp"
#include <SFML/System/Vector2.hpp>

Camera::Camera() {}

void Camera::init(Game* game, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size, Entity* focus)
{
    this->game = game;

    window = game->getWindow();

    gamerules = game->getGamerules();

    baseSize = size;
    
    this->size = baseSize;

    this->focus = focus;

    defaultZoom = game->getGamerules()->camera_defaultZoom;
    zoomFactor = defaultZoom;

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

float Camera::getZoomFactor() { return zoomFactor; }

void Camera::tick()
{
    lastCenter = center;

    if (focus != nullptr)
    {
        sf::Vector2<double> targetPosition = focus->getPosition();

        velocity.x = (targetPosition.x - center.x);
        velocity.y = (targetPosition.y - center.y);

        center.x += velocity.x;
        center.y += velocity.y;
    }
    else
    {
        velocity.x = (gamerules->camera_freecamMoveSpeedBase * zoomFactor) * game->getInput()->getMovement().x;
        velocity.y = (gamerules->camera_freecamMoveSpeedBase * zoomFactor) * game->getInput()->getMovement().y;

        center.x += velocity.x;
        center.y += velocity.y;
    }

    center.x = std::roundf(center.x);
    center.y = std::roundf(center.y);

    view.setCenter(center);
}

sf::View Camera::getInterpolatedView(float alpha)
{
    sf::Vector2f renderPos = center * alpha + lastCenter * (1.f - alpha);

    return sf::View(renderPos, view.getSize());
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

void Camera::zoom(int amount)
{
    if (amount == 0) return;
    if (zoomFactor <= gamerules->camera_minZoomFactor && amount < 0) return;
    if (zoomFactor >= gamerules->camera_maxZoomFactor && amount > 0) return;
    
    if (amount > 0)
    {
        if (zoomFactor >= 1.f)
        {
            zoomFactor *= gamerules->camera_bigZoomFactor;
        }
        else if (zoomFactor < 1.f)
        {
            zoomFactor += gamerules->camera_smallZoomAmount;
        }
    }
    else if (amount < 0)
    {
        if (zoomFactor > 1.f)
        {
            zoomFactor /= gamerules->camera_bigZoomFactor;
        }
        else if (zoomFactor <= 1.f)
        {
            zoomFactor -= gamerules->camera_smallZoomAmount;
        }
    }

    size = baseSize * zoomFactor;

    view.setSize(size);
}

void Camera::resetZoom()
{
    zoomFactor = defaultZoom;

    size = baseSize * zoomFactor;

    view.setSize(size);
}

void Camera::setBaseSize(sf::Vector2f newSize)
{
    baseSize = newSize;

    size = {
        baseSize.x * zoomFactor,
        baseSize.y * zoomFactor
    };

    view.setSize(size);
}

void Camera::setFocus(Entity* newFocus)
{
    focus = newFocus;
}

void Camera::removeFocus() { focus = nullptr; }

Entity* Camera::getFocus() { return focus; }