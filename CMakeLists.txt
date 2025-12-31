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
src/main.cpp
src/game.cpp
src/window.cpp
src/event_handler.cpp
src/camera.cpp
src/input.cpp
src/conversions.cpp
src/entity.cpp
src/sprite.cpp
src/asset_manager.cpp
src/attribute.cpp
src/motion_attribute.cpp
src/game_position.cpp
src/scene.cpp
src/collision_attribute.cpp
src/collision_rect.cpp
src/utils.cpp
src/animation.cpp
src/entity_states.cpp
src/animation_set.cpp
src/gamerules.cpp
src/gamerule.cpp
src/entity_layer.cpp
src/setting.cpp
src/settings.cpp
src/ui_layer.cpp
src/ui_element.cpp
)

target_compile_features(app PRIVATE cxx_std_17)
target_link_libraries(app PRIVATE SFML::Graphics)
