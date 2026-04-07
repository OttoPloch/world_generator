#include "gamerules.hpp"

Gamerules::Gamerules()
{
    init();
}

void Gamerules::init()
{
    float bigZoomFactor = 1.5f;

    camera_defaultZoom = 3.f;
    camera_freecamMoveSpeedBase = 15.f;
    camera_bigZoomFactor = bigZoomFactor;
    camera_smallZoomAmount = 0.1f;
    camera_minZoomFactor = 0.1f;
    camera_maxZoomFactor = pow(bigZoomFactor, 8);
    motion_friction = 0.8f;
    motion_velocityCutoff = 0.01f;
}