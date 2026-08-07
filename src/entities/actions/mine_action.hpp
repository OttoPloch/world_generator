#pragma once

#include "action.hpp"

struct MineAction : public Action
{
    MineAction(Game* game, float mineSpeed, std::string name, float rangeMultiplier);

    bool start() override;

    bool update(float dt) override;

    void completeAction(Entity* actor, sf::Vector2f position = {0, 0}) override;
    
    // restartCooldownProgress does not matter for MineAction, because cooldown is set to 0 here
    // no matter what.
    void reset(bool restartCooldownProgress) override;

    float mineSpeed;

    int mineZ;
private:
    Tile* startTile;
};