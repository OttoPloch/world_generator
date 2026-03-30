#pragma once

template<typename Enum>
constexpr int enumSize()
{
    return static_cast<int>(Enum::COUNT);
}

enum class AnimState
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    IDLE,

    COUNT
};