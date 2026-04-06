#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    int renderDistance = 5;

    ui_scale = 1.f;
    ui_animation_speed = 0.3f;
    tile_size = 200.f;
    chunk_size = 16;
    chunk_render_distance = renderDistance;
    chunk_load_distance = renderDistance * 2;
    generation_foliage_scale = 4.f;
}