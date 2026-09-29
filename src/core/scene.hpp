#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "../graphics/asset_manager.hpp"
#include "../entities/entity_layer.hpp"
#include "../world/chunk/chunk_layer.hpp"
#include "../entities/components/entity_component.hpp"
#include "../entities/components/movement_component.hpp"
#include "../entities/actions/action.hpp"
#include "../ui/ui_layer.hpp"

#include <vector>

class Game;

class Scene
{
public:
    Scene(Game* p_game);

    void tick();

    void update(float dt);

    void UIUpdate(float dt);

    void chunkLoadUpdate();

    void draw();

    void sceneInput(std::string control);

    bool processActionRequest(Entity* actor, Action* action);

    Camera* getCamera();

    void toggleFocus();

    EntityLayer* getEntityLayer();

    UILayer* getUILayer();

    ChunkLayer* getChunkLayer();
    
    sf::Vector2i getWorldChunkOrigin();
    void adjustWorldChunkOrigin(sf::Vector2i amount);
    bool debugMode;
    int debugLevel;
private:
    Game* m_game;
    Window* window;
    AssetManager* assetManager;    

    ChunkLayer chunkLayer;
    EntityLayer entityLayer;
    UILayer uiLayer;

    Camera camera;

    sf::Vector2i worldChunkOrigin;

    int debugChunkLayerView;

    // for monitoring performance
    std::unordered_map<std::string, float> updateBlame;
    std::unordered_map<std::string, float> drawBlame;
    sf::Clock debugClock;
};