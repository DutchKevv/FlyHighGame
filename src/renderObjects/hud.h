//
// Created by Kewin Brandsma on 01/08/2017.
//

#pragma once

#include <engine/context.h>

class Hud : public BaseRenderObj {
public:
    Hud();

    int init(Context &context);

    int update();

    int draw();

    int destroy();
private:

};