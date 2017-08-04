//
// Created by Kewin Brandsma on 02/08/2017.
//

#pragma once

#include <engine/baseRenderObj.h>

class Player : public BaseRenderObj {
public:
    const float minSpeed = -0.5f;
    const float maxSpeed = 1.0f;
    const float airborneThreshold = 0.1f;
    bool isAirborne = false;

    enum Camera_Movement {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        A,
        W,
        S,
        D
    };

    float yM = 0.0f;

    Player();

    int init();

    int update();

    int draw();

    int processKeyboard(Camera_Movement direction, float deltaTime);

    int renderScene(Shader &shader, bool isShaderRender);
    int destroy();
private:
};
