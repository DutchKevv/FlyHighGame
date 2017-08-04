//
// Created by Kewin Brandsma on 01/08/2017.
//

#pragma once

class Hud : public BaseRenderObj {
public:
    Hud();

    int init();

    int update();

    int draw();

    int renderScene(Shader &shader, bool isShadowRender);

    int destroy();
private:

};