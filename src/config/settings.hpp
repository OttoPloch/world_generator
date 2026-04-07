#pragma once

#include "../core/common.hpp"

#include <map>

class Settings
{
public:
    Settings();

    void init();

    float ui_scale;
    float ui_animation_speed;
    float tile_size;
    int chunk_size;
    float chunk_render_distance;
    float chunk_load_distance;
    float generation_foliage_scale;
};