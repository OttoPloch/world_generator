#include "gamerules.hpp"

Gamerules::Gamerules()
{
    init();
}

void Gamerules::init()
{
    float bigZoomFactor = 1.5f;

    camera_defaultZoom = 1.f;
    camera_freecamMoveSpeedBase = 15.f;
    camera_bigZoomFactor = bigZoomFactor;
    camera_smallZoomAmount = 0.1f;
    camera_minZoomFactor = 0.1f;
    camera_maxZoomFactor = pow(bigZoomFactor, 4);
    motion_friction = 0.9f;
}