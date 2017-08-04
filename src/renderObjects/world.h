//
// Created by Kewin Brandsma on 02/08/2017.
//

#pragma conce


#include <engine/baseRenderObj.h>

class World : public BaseRenderObj {
public:
    World();

    int init();

    int update();

    int draw();

    int renderScene(Shader &shader, bool isShadowRender);

    int destroy();
private:
};
