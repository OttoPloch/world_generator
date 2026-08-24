#pragma once

enum class RectType
{
    ACTIVE, // collides with all.
    PASSIVE, // Other rect types do not collide with this type, meaning this type is pushed by other types. Does not collide with other PASSIVE rects.
    STATIC, // does not move from collision resolutions.

    COUNT
};