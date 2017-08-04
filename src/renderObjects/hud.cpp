//
// Created by Kewin Brandsma on 01/08/2017.
//

#include <engine/shader.h>
#include <engine/baseRenderObj.h>
#include <engine/texture.h>
#include <engine/resourceManager.h>
#include <engine/renderer.h>
#include <engine/context.h>
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

int Hud::init() {
    BaseRenderObj::init();

    Shader shader = ResourceManager::LoadShader("assets/shaders/hud.v.glsl", "assets/shaders/hud.f.glsl", NULL, "hud");
    ResourceManager::LoadTexture("assets/textures/compass.png", GL_TRUE, "hud");

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
//
//    float h = context->windowH * 0.2;
//
//    glDisable(GL_DEPTH_TEST);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_BLEND);
//
//    context->renderer->sprite->draw(ResourceManager::GetTexture("hud"), glm::vec2(0, context->windowH - h), glm::vec2(h, h), 0.0f);
//
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_BLEND);
    return 0;
}

int Hud::renderScene(Shader &shader, bool isShadowRender) {
    BaseRenderObj::renderScene(shader, isShadowRender);

    float h = context->windowH * 0.2;

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Texture2D text = ResourceManager::GetTexture("hud");

    context->renderer->sprite->draw(text, glm::vec2(0, context->windowH - h), glm::vec2(h, h), 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    return 0;
}

int Hud::destroy() {
    BaseRenderObj::destroy();
    return 0;
}