#define ENGINE_TYPE 0;

#include <engine/engine.h>
#include <engine/context.h>
#include <engine/renderer.h>
#include <engine/baseRenderObj.h>
#include <engine/camera.h>
#include "src/renderObjects/skybox.h"
#include "src/renderObjects/floor.h"
#include "src/renderObjects/hud.h"
#include "src/renderObjects/player.h"
#include "src/renderObjects/world.h"

int main() {

    Engine engine(ENGINE_TYPE_GAME);


    World *world = new World();
    SkyBox *skybox = new SkyBox();
    Floor *floor = new Floor();
    Hud *hud = new Hud();
    Player *player = new Player();

    Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    engine.renderer->initShadowMap();
    engine.renderer->initSpriteHandler();

    // Camera, HUD, SkyBox, Player
    engine.renderer->attachCamera(camera);
    engine.renderer->attachPlayer(player);
//    engine.renderer->attachSkybox(skybox);
    engine.renderer->attachHud(hud);

    /// Objects

    engine.renderer->attachRenderObj(world);
//    engine.renderer->attachRenderObj(floor);

    engine.renderer->startLoop();
}