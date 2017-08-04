//
// Created by Kewin Brandsma on 02/08/2017.
//

#include <math.h>
#include "player.h"
#include <engine/context.h>
#include <engine/model.h>

Model *playerModel;
Shader shader;

Player::Player() : BaseRenderObj() {

}

int Player::init() {
    BaseRenderObj::init();

    position = glm::vec3(0.0f, 1.0f, 0.0f);

    playerModel = new Model("assets/models/plane/FREOBJ.obj");

    return 0;
}

int Player::update() {
    BaseRenderObj::update();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (speed != 0) {
        if (speed > 0) {
            speed -= 0.001f * deltaTime;
            if (speed < 0)
                speed = 0;
        }
    }

    if (speed < airborneThreshold) {
        yM -= 0.001f;
    }

//    position.x += speed * sinf(xRadius);
//
//    if (yM != 0)
//        position.y += yM * cosf(xRadius);
//
//    position.z += speed * cosf(xRadius);
//
//    if (position.y <= 0.1f) {
//        yM = 0;
//        position.y = 0.1f;
//        isAirborne = false;
//    } else {
//        isAirborne = true;
//    }

    if (glfwGetKey(context->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(context->window, true);
    if (glfwGetKey(context->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_UP) == GLFW_PRESS)
        this->processKeyboard(UP, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->processKeyboard(DOWN, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_W) == GLFW_PRESS)
        this->processKeyboard(W, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_S) == GLFW_PRESS)
        this->processKeyboard(S, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_A) == GLFW_PRESS)
        this->processKeyboard(A, deltaTime);
    if (glfwGetKey(context->window, GLFW_KEY_D) == GLFW_PRESS)
        this->processKeyboard(D, deltaTime);

    return 0;
}

int Player::draw() {
    return 0;
}

int Player::renderScene(Shader &shader, bool isShadowRender) {
    BaseRenderObj::draw();

    glm::mat4 model;

    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.1f));

    // Normalize
    model = glm::rotate(model, -1.75f, glm::vec3(1.0f, 0.0f, 0.0f));

    // horizontal x
    model = glm::rotate(model, xRadius, glm::vec3(0.0f, 0.0f, 1.0f));

    // horizontal y
    float pointDir = yM * 2;
    model = glm::rotate(model, yRadius, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, -pointDir, glm::vec3(1.0f, 0.0f, 0.0f));

    shader.SetMatrix4("model", model);

    glm::vec3 cPosition = glm::vec3(position.x, position.y, position.z);
    context->camera->lookToObj(*this);

    playerModel->Draw(&shader);

    return 0;
}

int Player::destroy() {
    BaseRenderObj::destroy();
    return 0;
}

int Player::processKeyboard(Camera_Movement direction, float velocity) {

    switch (direction) {
        case UP:
            yM -= velocity * 0.2f;
            break;
        case DOWN:
            if (speed > airborneThreshold)
                yM += velocity * 0.2f;
            break;
        case LEFT:
            if (speed > 0)
                xRadius += velocity * 1.0f;
            break;
        case RIGHT:
            if (speed > 0)
                xRadius -= velocity * 1.0f;
            break;
        case W:
            if (speed < maxSpeed)
                speed += velocity * 0.05f;
            break;
        case S:
            if (speed > minSpeed)
                speed -= velocity * 0.05f;
            break;
        case A:
            if (isAirborne)
                yRadius += velocity * 1.0f;
            break;
        case D:
            if (isAirborne)
                yRadius -= velocity * 1.0f;
            break;
        default:
            break;
    }

    return 0;
}

