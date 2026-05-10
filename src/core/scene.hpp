#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "../graphics/asset_manager.hpp"
#include "../entities/entity_layer.hpp"
#include "../ui/ui_layer.hpp"
#include "../ui/ui_animation.hpp"
#include "../world/chunk/chunk_layer.hpp"
#include "../entities/components/entity_component.hpp"
#include "../entities/components/movement_component.hpp"
#include "../entities/actions/action.hpp"

#include <vector>

class Game;

class Scene
{
public:
    Scene();

    void init(Game* game);

    void tick();

    void update(float dt);

    void UIUpdate(float dt);

    void chunkLoadUpdate();

    void draw();

    void sceneInput(std::string control, bool justPressed = false);

    bool processActionRequest(Entity* actor, Action* action);

    Camera* getCamera();

    void toggleFocus();

    EntityLayer* getEntityLayer();

    UILayer* getUILayer();

    ChunkLayer* getChunkLayer();
private:
    Game* game;
    Window* window;
    AssetManager* assetManager;    

    EntityLayer entityLayer;
    UILayer uiLayer;
    ChunkLayer chunkLayer;

    Camera camera;

    std::vector<std::pair<Entity*, std::unique_ptr<Action>>> actions;

    bool debugView;
    int debugChunkLayerView;
};