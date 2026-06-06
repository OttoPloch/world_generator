#include "gamerules.hpp"

Gamerules::Gamerules()
{
    init();
}

void Gamerules::init()
{
    camera_defaultZoom = .5f;
    camera_freecamMoveSpeedBase = 15.f;
    camera_bigZoomFactor = 1.5f;
    camera_smallZoomAmount = 0.1f;
    camera_minZoomFactor = 0.2f;
    camera_maxZoomFactor = pow(camera_bigZoomFactor, 4);
    motion_friction = 0.8f;
}