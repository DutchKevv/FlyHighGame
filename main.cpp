#define ENGINE_TYPE 0;

#include <engine/engine.h>
#include <engine/context.h>
#include <engine/renderer.h>
#include <engine/baseRenderObj.h>
#include <engine/camera.h>
#include "src/renderObjects/skybox.h"
#include "src/renderObjects/floor.h"
#include "src/renderObjects/hud.h"

Engine *engine;

int main() {

    engine = new Engine();

    SkyBox *skybox = new SkyBox();
    Floor *floor = new Floor();
    Hud *hud = new Hud();

    Camera *camera = new Camera(glm::vec3(0.0f, 0.2f, 3.0f));

    engine->renderer->attachCamera(camera);
    engine->renderer->attachRenderObj(skybox);
    engine->renderer->attachRenderObj(floor);
//
//    // Hud should always go as last
    engine->renderer->attachRenderObj(hud);
    engine->renderer->startLoop();


}