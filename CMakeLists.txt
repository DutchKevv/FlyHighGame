cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(FlyHighGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE_INIT "Release")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

message(${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_C_COMPILER "emcc")
endif ()

include(/Users/kewinbrandsma/Projects/Engine/CMakeLists.txt)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(SOURCE_FILES
        main.cpp
        src/level.cpp src/level.h
        src/renderObjects/skybox.cpp src/renderObjects/skybox.h
        src/renderObjects/floor.cpp src/renderObjects/floor.h src/renderObjects/hud.cpp src/renderObjects/hud.h src/renderObjects/player.cpp src/renderObjects/player.h src/renderObjects/game_world.cpp src/renderObjects/game_world.h)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    include_directories(/emscripten/ /emscripten/emscripten/incoming/system/include)
else()
    include_directories()
endif ()

add_executable(FlyHighGame ${SOURCE_FILES})
set_target_properties(FlyHighGame PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(FlyHighGame Engine)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
