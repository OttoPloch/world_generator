#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    int renderDistance = 5;

    settings = {
        {"ui_scale", setting(1.f, 0, false, "")},
        {"ui_animation_speed", setting(0.3f, 0, false, "")},
        {"tile_size", setting(200.f, 0, false, "")},
        {"chunk_size", setting(0.f, 16, false, "")},
        {"chunk_render_distance", setting(0.f, renderDistance, false, "")}, // distance at which chunks get drawn
        {"chunk_load_distance", setting(0.f, renderDistance + 2, false, "")}, // distance at which chunks get loaded, chunks in between this and render distance are loaded but not ticked, updated, or drawn.
        {"generation_foliage_scale", setting(4.f, 0, false, "")}
    };
}

setting Settings::getSetting(std::string option)
{
    if (settings.find(option) != settings.end())
    {
        return settings[option];
    }

    std::cout << "ERROR finding setting " << option << ". Setting does not exist.\n";

    assert(false);
    
    return setting(0.f, 0, false, "");
}