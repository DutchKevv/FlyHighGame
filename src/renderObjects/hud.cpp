//
// Created by Kewin Brandsma on 01/08/2017.
//

#include <engine/shader.h>
#include <engine/baseRenderObj.h>
#include <engine/texture.h>
#include <engine/resourceManager.h>
#include "hud.h"

unsigned int VAO, VBO;

static float vertices[] = {
        -1.5f, -1.5f, 0.0f, 0.0f,
        1.5f, -1.5f, 1.0f, 0.0f,
        1.5f,  1.5f, 1.0f, 1.0f,

        1.5f,  1.5f, 1.0f, 1.0f,

        -1.5f,  1.5f,  0.0f, 1.0f,
        -1.5f, -1.5f, 0.0f, 0.0f
};

Hud::Hud() : BaseRenderObj() {}

int Hud::init(Context &context) {
    BaseRenderObj::init(context);

    Shader shader = ResourceManager::LoadShader("assets/shaders/hud.v.glsl", "assets/shaders/hud.f.glsl", NULL, "hud");

    ResourceManager::LoadTexture("assets/textures/boulder.jpg", GL_FALSE, "hud");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(shader.getLayoutPos("coord"));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(shader.getLayoutPos("textureCoord"));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}


int Hud::update() {
    BaseRenderObj::update();

    return 0;
}

int Hud::draw() {
    BaseRenderObj::draw();

    ResourceManager::GetShader("hud").Use();

    int w = context.windowW * 2;
    int h = context.windowH * 0.3;

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    glViewport(0, 0, w, h);
    glScissor(0, 0, w, h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("hud").ID);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glViewport(0, 0, context.windowW * 2, context.windowH * 2);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);

    return 0;
}

int Hud::destroy() {
    BaseRenderObj::destroy();
    return 0;
}