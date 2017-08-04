//
// Created by Kewin Brandsma on 31/07/2017.
//

#pragma once

#include <engine/baseRenderObj.h>

class SkyBox : public BaseRenderObj {
public:
    SkyBox();

    int init();

    int update();

    int draw();

    int renderScene(Shader &shader, bool isShaderRender);

    int destroy();

private:
};
