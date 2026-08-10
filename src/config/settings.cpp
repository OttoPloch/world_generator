#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    int renderDistance = 1;
    int loadDistanceExtra = 1;

    ui_animation_speed = 0.15f;
    tile_size = 10.f;
    chunk_size = 16;
    chunk_render_distance = renderDistance;
    chunk_load_distance = renderDistance + loadDistanceExtra;
    generation_decoration_scale = -1.f;
    generation_decoration_frequency = chunk_size * 6;
    maxTileZ = 2;
    entityTileZEquivalent = maxTileZ - 1;
    worldSeed = 117;
    input_controllerCursorSensitivity = 20;
    input_triggerMinPressValue = 0;
    input_UISelectorMoveCooldown = 0.15f;
    worldOriginThreshold = chunk_size * tile_size * 2;
}
