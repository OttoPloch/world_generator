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
src/input_handler.cpp
)

target_compile_features(app PRIVATE cxx_std_17)
target_link_libraries(app PRIVATE SFML::Graphics)
