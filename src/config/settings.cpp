#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    int renderDistance = 10;
    int loadDistanceExtra = 0;

    ui_animation_speed = 0.15f;
    tile_size = 10.f;
    chunk_size = 16;
    chunk_render_distance = renderDistance;
    chunk_load_distance = renderDistance + loadDistanceExtra;
    generation_foliage_scale = .2f;
    maxTileZ = 2;
    worldSeed = -1;
}