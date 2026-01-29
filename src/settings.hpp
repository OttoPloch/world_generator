#pragma once

#include "common.hpp"
#include "setting.hpp"

#include <map>

class Settings
{
public:
    Settings();

    void init();

    setting getSetting(std::string option);
private:
    std::map<std::string, setting> settings;
};