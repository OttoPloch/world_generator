#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    settings = {
        {"ui_scale", setting(1.f, 0, false, "")},
        {"ui_animation_speed", setting(0.3f, 0, false, "")}
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