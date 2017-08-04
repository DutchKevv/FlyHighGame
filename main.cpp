#define ENGINE_TYPE 0;

#include <engine/engine.h>
#include "src/renderObjects/game_world.h"

int main() {

    Engine engine(ENGINE_TYPE_GAME);

    engine.renderer->initSpriteHandler();

    World *gameWorld = new GameWorld();
    engine.renderer->attachWorld(gameWorld);

    engine.renderer->startLoop();
}