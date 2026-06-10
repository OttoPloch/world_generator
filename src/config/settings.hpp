#pragma once

#include "../core/common.hpp"

#include <map>

class Settings
{
public:
    Settings();

    void init();

    float ui_animation_speed;
    float tile_size;
    int chunk_size;
    float chunk_render_distance;
    float chunk_load_distance;
    float generation_decoration_scale; // negative = match tile pixel size
    int generation_decoration_frequency;
    int maxTileZ;
    int worldSeed; // -1 = random seed
    float input_controllerCursorSensitivity;
    float input_triggerMinPressValue; // -100 is completely up, +100 is all the way down in SFML
    float input_UISelectorMoveCooldown;
};