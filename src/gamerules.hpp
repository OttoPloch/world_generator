#pragma once

#include "common.hpp"
#include "gamerule.hpp"

#include <map>

class Gamerules
{
public:
    Gamerules();

    void init();

    float camera_freecamMoveSpeedBase;
    float camera_bigZoomFactor;
    float camera_smallZoomAmount;
    float camera_minZoomFactor;
    float camera_maxZoomFactor;
    float motion_friction;
    float motion_velocityCutoff;

    float player_moveSpeed;
    float player_acceleration;
};