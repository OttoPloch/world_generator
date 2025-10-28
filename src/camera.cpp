#include "camera.hpp"

#include "window.hpp"

const int FREECAM_MOVE_SPEED_BASE = 10;

const float CAMERA_BIG_ZOOM_FACTOR = 1.5f;
const float CAMERA_SMALL_ZOOM_AMOUNT = 0.1f;
const float CAMERA_MIN_ZOOM_FACTOR = 0.5f;
const float CAMERA_MAX_ZOOM_FACTOR = pow(CAMERA_BIG_ZOOM_FACTOR, 3);

Camera::Camera() {}

void Camera::init(Window& window, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size)
{
    this->window = &window;

    baseSize = size;
    
    this->size = baseSize;

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
    if (getMovement().x == 0)
    {
        velocity.x *= 1 - (dt * 8);
    }
    else
    {
        velocity.x = (FREECAM_MOVE_SPEED_BASE * zoomFactor) * getMovement().x;
    }

    if (getMovement().y == 0)
    {
        velocity.y *= 1 - (dt * 8);
    }
    else
    {
        velocity.y = (FREECAM_MOVE_SPEED_BASE * zoomFactor) * getMovement().y;
    }

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
    
    std::cout << "amount is " << amount << " which is ";

    if (amount > 0)
    {
        std::cout << "> 0, so ";

        if (zoomFactor >= 1.f)
        {
            std::cout << "since zoomFactor is " << zoomFactor << ", which is >= 1, multiply by 2\n";

            zoomFactor *= CAMERA_BIG_ZOOM_FACTOR;
        }
        else if (zoomFactor < 1.f)
        {
            std::cout << "since zoomFactor is " << zoomFactor << ", which is < 1, add .1 to it\n";

            zoomFactor += CAMERA_SMALL_ZOOM_AMOUNT;
        }
    }
    else if (amount < 0)
    {
        std::cout << "< 0, so ";

        if (zoomFactor > 1.f)
        {
            std::cout << "since zoomFactor is " << zoomFactor << ", which is > 1, divide by 2\n";

            zoomFactor /= CAMERA_BIG_ZOOM_FACTOR;
        }
        else if (zoomFactor <= 1.f)
        {
            std::cout << "since zoomFactor is " << zoomFactor << ", which is <= 1, subtract by .1\n";
            
            zoomFactor -= CAMERA_SMALL_ZOOM_AMOUNT;
        }
    }

    std::cout << "new zoom: " << zoomFactor << '\n';

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