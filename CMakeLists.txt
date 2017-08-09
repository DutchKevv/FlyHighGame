cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_BUILD_TYPE_INIT "Release")

include(/Users/kewinbrandsma/Projects/Engine/CMakeLists.txt)

project(FlyHighGame)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(SOURCE_FILES
        main.cpp
        src/level.cpp src/level.h
        src/renderObjects/skybox.cpp src/renderObjects/skybox.h
        src/renderObjects/floor.cpp src/renderObjects/floor.h
        src/renderObjects/hud.cpp src/renderObjects/hud.h
        src/renderObjects/player.cpp src/renderObjects/player.h
        src/renderObjects/game_world.cpp src/renderObjects/game_world.h
    )

add_executable(FlyHighGame ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} Engine)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-o dist/engine.js -s USE_GLFW=3 -s TOTAL_MEMORY=167772160 -s USE_FREETYPE=1 -s OUTLINING_LIMIT=2000 -s DISABLE_EXCEPTION_CATCHING=0 -s NO_EXIT_RUNTIME=1 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --bind -s WASM=1 -O2 -s LEGACY_GL_EMULATION=0  -s GL_UNSAFE_OPTS=0 --pre-js pre-module.js --post-js post-module.js -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -s INVOKE_RUN=0  -std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -s GL_DEBUG=1 -s OFFSCREENCANVAS_SUPPORT=0 --use-preload-plugins --preload-file assets --preload-file assets")
else ()
    target_link_libraries(${CMAKE_PROJECT_NAME} assimp ${OPENGL_gl_LIBRARY} glew glfw ${GLM_LIBRARY})
endif ()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
