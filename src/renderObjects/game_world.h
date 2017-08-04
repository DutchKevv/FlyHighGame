//
// Created by Kewin Brandsma on 02/08/2017.
//

#pragma conce


#include <engine/baseRenderObj.h>
#include <engine/world.h>

class GameWorld : public World {
public:
    GameWorld();

    int init();

    int update();

    int draw();

    int renderScene(Shader &shader, bool isShadowRender);

    int renderShadowScene();

    int destroy();
private:
};
