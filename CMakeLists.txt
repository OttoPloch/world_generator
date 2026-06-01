cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(app
src/core/main.cpp
src/core/game.cpp
src/core/window.cpp
src/core/event_handler.cpp
src/core/camera.cpp
src/core/input.cpp
src/core/scene.cpp

src/utils/conversions.cpp
src/utils/utils.cpp
src/utils/game_position.cpp

src/config/settings.cpp
src/config/gamerules.cpp

src/entities/entity_layer.cpp
src/entities/entity.cpp

src/entities/components/entity_component.cpp
src/entities/components/position_component.cpp
src/entities/components/movement_component.cpp
src/entities/components/control_component.cpp
src/entities/components/state_component.cpp
src/entities/components/collision_component.cpp
src/entities/components/action_component.cpp
src/entities/components/sprite_component.cpp
src/entities/actions/action.cpp
src/entities/actions/mine_action.cpp
src/entities/template_manager.cpp
src/entities/collision_rect.cpp
src/entities/entity_systems/position_system.cpp
src/entities/entity_systems/collision_system.cpp
src/entities/entity_systems/render_system.cpp
src/entities/entity_systems/animation_system.cpp
src/entities/entity_systems/movement_system.cpp
src/entities/entity_systems/action_system.cpp

src/graphics/sprite.cpp
src/graphics/asset_manager.cpp
src/graphics/tile_set.cpp
src/graphics/texture_atlas.cpp
src/graphics/vertex_group.cpp
src/graphics/animation.cpp
src/graphics/global_animation.cpp
src/graphics/animation_set.cpp

src/ui/ui_layer.cpp
src/ui/ui_element.cpp
src/ui/ui_background.cpp
src/ui/ui_button.cpp
src/ui/ui_text.cpp
src/ui/ui_animation.cpp
src/ui/interactive_ui_manager.cpp
src/ui/ui_position.cpp
src/ui/components/ui_component.cpp
src/ui/components/background_component.cpp
src/ui/components/text_component.cpp
src/ui/components/button_component.cpp
src/ui/components/image_component.cpp

src/world/background_object.cpp

src/world/chunk/chunk_layer.cpp
src/world/chunk/chunk.cpp
src/world/chunk/chunk_generator.cpp

src/world/tile/tile.cpp
src/world/tile/tags/tile_tag.cpp
src/world/tile/tags/mineable_tag.cpp
src/world/tile/tile_template_manager.cpp
src/world/tile/tile_template.cpp
)

target_compile_features(app PRIVATE cxx_std_17)
target_link_libraries(app PRIVATE SFML::Graphics)