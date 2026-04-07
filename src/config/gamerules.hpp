#pragma once

#include "../core/common.hpp"

#include <map>

class Gamerules
{
public:
    Gamerules();

    void init();

    float camera_defaultZoom;
    float camera_freecamMoveSpeedBase;
    float camera_bigZoomFactor;
    float camera_smallZoomAmount;
    float camera_minZoomFactor;
    float camera_maxZoomFactor;
    float motion_friction;
    float motion_velocityCutoff;
};