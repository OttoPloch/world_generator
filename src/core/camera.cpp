#include "camera.hpp"
#include "game.hpp"
#include <SFML/System/Vector2.hpp>
#include "../entities/components/components.hpp"

Camera::Camera() {}

void Camera::init(Game* game, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size, Entity* focus)
{
    this->game = game;
    window = game->getWindow();

    baseSize = size;
    defaultZoom = game->getSettings()->camera_defaultZoom;
    zoomSpeed = game->getSettings()->camera_zoomSpeed;
    zoomAmount = defaultZoom;
    updateZoom();

    this->focus = focus;

    velocity = {0, 0};

    if (setTopLeftPos) setTopLeft(position);
    else setCenter(position);

    view.setCenter(center);
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

sf::Vector2f Camera::getCenter() { return center; }

sf::Vector2f Camera::getTopLeft()
{
    return {center.x - size.x / 2, center.y - size.y / 2};
}

sf::Vector2f Camera::getSize() { return size; }

void Camera::tick()
{
    updatePosition();
}

void Camera::update(float dt)
{
    sf::Vector2i worldChunkOriginChange(0, 0);
    float threshold = game->getSettings()->worldOriginThreshold;
    float chunkLength = game->getSettings()->tile_size * game->getSettings()->chunk_size;
    if (center.x >= threshold)
    {
        worldChunkOriginChange.x += static_cast<int>(center.x / chunkLength);
        center.x = std::fmod(center.x, threshold);
    }
    if (center.x < -threshold)
    {
        worldChunkOriginChange.x += static_cast<int>(center.x / chunkLength);
        center.x = std::fmod(center.x, threshold);
    }
    if (center.y >= threshold)
    {
        worldChunkOriginChange.y += static_cast<int>(center.y / chunkLength);
        center.y = std::fmod(center.y, threshold);
    }
    if (center.y < -threshold)
    {
        worldChunkOriginChange.y += static_cast<int>(center.y / chunkLength);
        center.y = std::fmod(center.y, threshold);
    }

    if (worldChunkOriginChange != sf::Vector2i(0, 0))
    {
        game->getScene()->adjustWorldChunkOrigin(worldChunkOriginChange);

        updatePosition();
    }
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

    zoomAmount += zoomSpeed * amount;

    updateZoom();
}

void Camera::resetZoom()
{
    zoomAmount = defaultZoom;

    updateZoom();
}

void Camera::windowResized(sf::Vector2u oldSize, sf::Vector2u newSize)
{
    zoomAmount *= static_cast<float>(newSize.y) / static_cast<float>(oldSize.y);

    setBaseSize(sf::Vector2f(newSize));
}

void Camera::setBaseSize(sf::Vector2f newSize)
{
    baseSize = newSize;

    updateZoom();
}

void Camera::setFocus(Entity* newFocus)
{
    focus = newFocus;
}

void Camera::removeFocus() { focus = nullptr; }

Entity* Camera::getFocus() { return focus; }

void Camera::updatePosition()
{
    if (focus != nullptr)
    {
        center = focus->getComponent<PositionComponent>()->position.getPosition();
    }
    else
    {
        velocity.x = (game->getSettings()->camera_freecamMoveSpeedBase * (size.x / baseSize.x)) * game->getInput()->getMovement().x;
        velocity.y = (game->getSettings()->camera_freecamMoveSpeedBase * (size.x / baseSize.x)) * game->getInput()->getMovement().y;

        center.x += velocity.x;
        center.y += velocity.y;
    }

    view.setCenter(center);
}

void Camera::updateZoom()
{
    size = baseSize + sf::Vector2f(zoomAmount * game->getWindow()->getAspectRatio(), zoomAmount);

    float minSizeFraction = game->getSettings()->camera_minSizeFraction;
    float maxSizeFraction = game->getSettings()->camera_maxSizeFraction;

    if (size.x < baseSize.x * minSizeFraction || size.y < baseSize.y * minSizeFraction)
    {
        size = baseSize * minSizeFraction;

        zoomAmount = size.y - baseSize.y;
    }
    else if (size.x > baseSize.x * maxSizeFraction || size.y > baseSize.y * maxSizeFraction)
    {
        size = baseSize * maxSizeFraction;

        zoomAmount = size.y - baseSize.y;
    }

    view.setSize(size);
}