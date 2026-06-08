#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    int renderDistance = 5;
    int loadDistanceExtra = 5;

    ui_animation_speed = 0.15f;
    tile_size = 10.f;
    chunk_size = 16;
    chunk_render_distance = renderDistance;
    chunk_load_distance = renderDistance + loadDistanceExtra;
    generation_decoration_scale = -1.f;
    generation_decoration_frequency = chunk_size * 6;
    maxTileZ = 2;
    worldSeed = 117;
    input_controllerCursorSensitivity = 2;
    input_triggerMinPressValue = 0;
}