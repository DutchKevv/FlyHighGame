//
// Created by Kewin Brandsma on 01/08/2017.
//

#pragma once

#include <engine/baseRenderObj.h>

class Floor : public BaseRenderObj {
public:
    Floor();

    int init();

    int update();

    int draw();

    int renderScene(Shader &shader, bool isShadowRender);

    int destroy();
private:
};
