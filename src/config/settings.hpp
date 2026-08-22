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
    int entityTileZEquivalent; // used for the cursor to detect whether the entity or tile should be
    // considered selected at the position of the cursor when both are present. If this value is 1,
    // for example, and a tile (with a z of 2 or more) and entity both collide with the cursor position,
    // the tile will be considered selected. If the tile z is <= this value, the entity takes priority.
    // TODO: this should also be accounted for in drawing, where all tiles (and decorations?) with a z
    // <= this value are drawn before entities, then all tiles with a z > this value are drawn after,
    // giving a better depth effect.
    int worldSeed; // -1 = random seed
    float input_controllerCursorSensitivity;
    float input_triggerMinPressValue; // -100 is completely up, +100 is all the way down in SFML
    float input_UISelectorMoveCooldown;
    float worldOriginThreshold;
    float motion_friction;
    float item_maxSpawnVelocity;
    float camera_defaultZoom;
    float camera_zoomSpeed;
    float camera_freecamMoveSpeedBase;
    float camera_minSizeFraction; // fraction of the camera's base size.
    float camera_maxSizeFraction; // fraction of the camera's base size.
};