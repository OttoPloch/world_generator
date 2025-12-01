#include "camera.hpp"

#include "window.hpp"

const int FREECAM_MOVE_SPEED_BASE = 10;
const float FREECAM_FRICTION = 8.f;

const float CAMERA_BIG_ZOOM_FACTOR = 1.5f;
const float CAMERA_SMALL_ZOOM_AMOUNT = 0.1f;
const float CAMERA_MIN_ZOOM_FACTOR = 0.3f;
const float CAMERA_MAX_ZOOM_FACTOR = pow(CAMERA_BIG_ZOOM_FACTOR, 3);

const float FOCUS_FOLLOW_DELAY = 10.f;

Camera::Camera() {}

void Camera::init(Window* window, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size, Entity* focus)
{
    this->window = window;

    baseSize = size;
    
    this->size = baseSize;

    this->focus = focus;

    zoomFactor = 1.f;

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

void Camera::update(float dt)
{
    if (focus != nullptr)
    {
        sf::Vector2f targetPosition;
        
        (focus->getSprite() != nullptr) ? targetPosition = focus->getSprite()->getSpritePosition() : targetPosition = focus->getPosition();

        velocity.x = (targetPosition.x - center.x) / FOCUS_FOLLOW_DELAY;
        velocity.y = (targetPosition.y - center.y) / FOCUS_FOLLOW_DELAY;
    }
    else
    {
        if (getMovement().x == 0)
        {
            velocity.x *= 1 - (dt * FREECAM_FRICTION);
        }
        else
        {
            velocity.x = (FREECAM_MOVE_SPEED_BASE * zoomFactor) * getMovement().x;
        }
    
        if (getMovement().y == 0)
        {
            velocity.y *= 1 - (dt * FREECAM_FRICTION);
        }
        else
        {
            velocity.y = (FREECAM_MOVE_SPEED_BASE * zoomFactor) * getMovement().y;
        }
    }

    // the 100 is just to make things go faster, not intended to be changeable
    center.x += velocity.x * 100 * dt;
    center.y += velocity.y * 100 * dt;

    view.setCenter(center);
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
    if (zoomFactor <= CAMERA_MIN_ZOOM_FACTOR && amount < 0) return;
    if (zoomFactor >= CAMERA_MAX_ZOOM_FACTOR && amount > 0) return;
    
    if (amount > 0)
    {
        if (zoomFactor >= 1.f)
        {
            zoomFactor *= CAMERA_BIG_ZOOM_FACTOR;
        }
        else if (zoomFactor < 1.f)
        {
            zoomFactor += CAMERA_SMALL_ZOOM_AMOUNT;
        }
    }
    else if (amount < 0)
    {
        if (zoomFactor > 1.f)
        {
            zoomFactor /= CAMERA_BIG_ZOOM_FACTOR;
        }
        else if (zoomFactor <= 1.f)
        {
            zoomFactor -= CAMERA_SMALL_ZOOM_AMOUNT;
        }
    }

    size = {
        baseSize.x * zoomFactor,
        baseSize.y * zoomFactor,
    };

    view.setSize(size);
}

void Camera::resetZoom()
{
    zoomFactor = 1.f;

    size = baseSize;

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