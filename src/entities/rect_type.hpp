#pragma once

enum class RectType
{
    ACTIVE, // collides with all
    PASSIVE, // only collides with active and static. Other rect types do not collide with this, meaning this type is pushed by other types.
    STATIC, // does not move from collision resolutions

    COUNT
};