#pragma once

#include "action.hpp"

struct MineAction : public Action
{
    MineAction(float mineSpeed, std::string name, float rangeMultiplier);

    void start(Game* game) override;

    bool update(float dt, Game* game) override;

    void completeAction(Entity* actor, sf::Vector2f position = {0, 0}) override;

    float mineSpeed;

    int mineZ;
};