//
// Created by Kewin Brandsma on 01/08/2017.
//

#pragma once

#include <engine/baseRenderObj.h>

class Floor : public BaseRenderObj {
public:
    Floor();

    int init(Context &context);

    int update();

    int draw();

    int destroy();
private:
};
