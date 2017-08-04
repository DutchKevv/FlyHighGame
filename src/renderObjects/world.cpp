//
// Created by Kewin Brandsma on 02/08/2017.
//

#include "world.h"
#include <engine/model.h>
#include <engine/context.h>
#include <algorithm>
#include <vector>

using namespace std;
Model *rockModel;
Model *planeModel;
Model *blenderModel;
Model *treeModel;
Model *cubeModel;

const unsigned int space = 200;
const unsigned int trees = 1000;
const unsigned int rocks = 1000;

//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//unsigned int depthMapFBO;
//unsigned int depthMap;

glm::mat4 model_matrix[trees];
vector<glm::vec4> treePositions;
vector<glm::vec4> rockPositions;


int dayCycleMax = 5000;
int dayCycle = 3500;

// lighting info
// -------------

static float vertices[] = {
        500.0f, -0.0f, 500.0f, 500.0f, 0.0f,
        -500.0f, -0.0f, 500.0f, 0.0f, 0.0f,
        -500.0f, -0.0f, -500.0f, 0.0f, 500.0f,

        500.0f, -0.0f, 500.0f, 500.0f, 0.0f,
        -500.0f, -0.0f, -500.0f, 0.0f, 500.0f,
        500.0f, -0.0f, -500.0f, 1.0f, 1.0f
};

// meshes
unsigned int planeVAO;


World::World() : BaseRenderObj() {

}

int World::init() {

    Shader worldShader = ResourceManager::GetShader("world").Use();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
            // positions            // normals         // texcoords
            250.0f, -0.0f,  250.0f,  0.0f, 1.0f, 0.0f,  250.0f,  0.0f,
            -250.0f, -0.0f,  250.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -250.0f, -0.0f, -250.0f,  0.0f, 1.0f, 0.0f,   0.0f, 250.0f,

            250.0f, -0.0f,  250.0f,  0.0f, 1.0f, 0.0f,  250.0f,  0.0f,
            -250.0f, -0.0f, -250.0f,  0.0f, 1.0f, 0.0f,   0.0f, 250.0f,
            250.0f, -0.0f, -250.0f,  0.0f, 1.0f, 0.0f,  250.0f, 250.0f
    };

    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);


    // Shader config
    worldShader.Use();
    worldShader.SetVector3f("light.direction", -20.2f, -10.0f, -10.3f);
    worldShader.SetVector3f("viewPos", context->camera->Position);

    worldShader.SetVector3f("light.ambient", 0.7, 0.7, 0.7);
    worldShader.SetVector3f("light.diffuse", 0.5, 0.5, 0.5);
    worldShader.SetVector3f("light.specular", 0.7, 0.7, 0.7);

    worldShader.SetFloat("material.shininess", 32.0f);

    rockModel = new Model("assets/models/stone/stone.obj");
    planeModel = new Model("assets/models/plane-ground/flyhigh.obj");
    blenderModel = new Model("assets/models/blenderman/BLENDERMAN!.obj");
    treeModel = new Model("assets/models/tree-low-poly/lowtree.obj");
    cubeModel = new Model("assets/models/cube/cube.obj");

    float halfSpace = space / 2;
    for (int i = 0; i < trees; i++) {
        treePositions.push_back(glm::vec4((rand() % space) - halfSpace, 0.0f, (rand() % space) - halfSpace, rand() % 100));
    }

    for (int i = 0; i < rocks; i++) {
        rockPositions.push_back(glm::vec4((rand() % space) - halfSpace, 2.0f, (rand() % space) - halfSpace, rand() % 100));
    }

    return 0;
}

int World::update() {

    float dayPercentage = dayCycle / dayCycleMax;
    float lightRange = dayPercentage * 2;
    bool pm = false;

    // Past top light, gets only darker (12:00)
    if (dayPercentage >= 0.5) {
        pm = true;
        float diff = ((dayPercentage - 0.5) / 0.5);
        lightRange = 1 - diff;
    }

    if (lightRange < 0.15) {
        lightRange = 0.15;
    } else if (lightRange > 0.7) {
        lightRange = 0.8;
    }

    Shader worldShader = ResourceManager::GetShader("world");
    // be sure to activate shader when setting uniforms/drawing objects
    worldShader.Use();
    worldShader.SetVector3f("light.direction", -0.2f, -1.0f, -0.3f);
    worldShader.SetVector3f("viewPos", context->camera->Position);

    worldShader.SetVector3f("light.ambient", lightRange, lightRange, lightRange);
    worldShader.SetVector3f("light.diffuse", lightRange, lightRange, lightRange);
    worldShader.SetVector3f("light.specular", lightRange, lightRange, lightRange);

    worldShader.SetFloat("material.shininess", 32.0f);

    return 0;
}

int World::draw() {
    return 0;
}

int World::renderScene(Shader &shader, bool isShadowRender) {

    // floor
//    glCullFace(GL_FRONT);
    glm::mat4 model;
    shader.SetMatrix4("model", model);
    glBindVertexArray(planeVAO);
    ResourceManager::GetTexture("grass").Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glCullFace(GL_BACK);

//    // Mountains
//    for (unsigned int i = 0; i < 10; i++) {
//        glm::mat4 model;
//        glm::mat4 view;
//        glm::mat4 projection;
//
//        model = glm::translate(model, mountainPositions[i]);
////        model = glm::rotate(model, i * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//        model = glm::scale(model, glm::vec3(10.0f));
//        view = glm::lookAt(context->camera->Position, context->camera->Position + context->camera->Front,
//                           context->camera->Up);
//        projection = glm::perspective(glm::radians(45.0f), (float) context->windowW / (float) context->windowH, 0.1f,
//                                      100.0f);
//
//        shader.SetMatrix4("model", model);
//        shader.SetMatrix4("view", view);
//        shader.SetMatrix4("projection", projection);
//
//        rockModel->Draw(&shader);
//    }

//    glm::mat4 view = glm::lookAt(context->camera->Position, context->camera->Position + context->camera->Front,
//                       context->camera->Up);
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) context->windowW / (float) context->windowH, 0.1f,
//                                  100.0f);

//    shader.SetMatrix4("view", view);
//    shader.SetMatrix4("projection", projection);

//    ResourceManager::GetTexture("rock").Bind();

    for (unsigned int i = 0; i < trees; i++) {

        glm::mat4 model;
        glm::vec4 random = treePositions[i];
        model = glm::translate(model, glm::vec3(random.x, random.y, random.z));
        model = glm::scale(model, glm::vec3(random.w / 100));

        shader.SetMatrix4("model", model);

        treeModel->Draw(&shader);
    }

    for (unsigned int i = 0; i < rocks; i++) {
        glm::mat4 model;
        glm::vec4 random = rockPositions[i];

        model = glm::translate(model, glm::vec3(random.x, random.y, random.z));
        model = glm::scale(model, glm::vec3(random.w / 100));

        shader.SetMatrix4("model", model);

        cubeModel->Draw(&shader);
    }
}

int World::destroy() {
    return 0;
}