#include "settings.hpp"

Settings::Settings()
{
    init();
}

void Settings::init()
{
    settings = {
        {"ui_scale", setting(1.f, 0, false, "")}
    };
}