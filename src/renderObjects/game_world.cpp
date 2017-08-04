//
// Created by Kewin Brandsma on 02/08/2017.
//

#include "game_world.h"
#include <engine/model.h>
#include <engine/context.h>
#include <engine/baseRenderObj.h>
#include <engine/camera.h>
#include "src/renderObjects/skybox.h"
#include "src/renderObjects/hud.h"
#include "src/renderObjects/player.h"
#include <algorithm>
#include <vector>


using namespace std;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int space = 200;
const unsigned int trees = 1000;
const unsigned int rocks = 1000;
const unsigned int dayCycleMax = 5000;
unsigned int depthMap;
unsigned int depthMapFBO;
unsigned int dayCycle = 3500;

// lighting info
// -------------
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

Model *rockModel;
Model *planeModel;
Model *blenderModel;
Model *treeModel;
Model *cubeModel;


vector<glm::vec4> treePositions;
vector<glm::vec4> rockPositions;

// meshes
unsigned int planeVAO;


GameWorld::GameWorld() : World() {

}

int GameWorld::init() {

    // load shader
    // -----------------------
    ResourceManager::LoadShader("assets/shaders/shadowMappingDepth.v.glsl", "assets/shaders/shadowMappingDepth.f.glsl", nullptr, "shadowMappingDepth");
    Shader depthQuadShader = ResourceManager::LoadShader("assets/shaders/depthQuad.v.glsl", "assets/shaders/depthQuad.f.glsl", nullptr, "depthQuad");
    Shader worldShader = ResourceManager::LoadShader("assets/shaders/shadowMapping.v.glsl", "assets/shaders/shadowMapping.f.glsl", nullptr, "world");

    // load textures
    // -----------------------
    ResourceManager::LoadTexture("assets/textures/grass.jpg", GL_FALSE, "grass");
    ResourceManager::LoadTexture("assets/textures/boulder.jpg", GL_FALSE, "rock");

    // load renderObjects
    // -----------------------
    SkyBox *skybox = new SkyBox();
    Hud *hud = new Hud();
    Player *player = new Player();
    Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Camera, HUD, SkyBox, Player
    this->attachCamera(camera);
    this->attachPlayer(player);
    this->attachSkybox(skybox);
    this->attachHud(hud);

    // load models
    // -----------------------
    rockModel = new Model("assets/models/stone/stone.obj");
    planeModel = new Model("assets/models/plane-ground/flyhigh.obj");
    blenderModel = new Model("assets/models/blenderman/BLENDERMAN!.obj");
    treeModel = new Model("assets/models/tree-low-poly/lowtree.obj");
    cubeModel = new Model("assets/models/cube/cube.obj");

    // load random positions for models
    float halfSpace = space / 2;
    for (int i = 0; i < trees; i++) {
        treePositions.push_back(
                glm::vec4((rand() % space) - halfSpace, 0.0f, (rand() % space) - halfSpace, rand() % 100));
    }

    for (int i = 0; i < rocks; i++) {
        rockPositions.push_back(
                glm::vec4((rand() % space) - halfSpace, 2.0f, (rand() % space) - halfSpace, rand() % 100));
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
            // positions            // normals         // texcoords
            250.0f, -0.0f, 250.0f, 0.0f, 1.0f, 0.0f, 250.0f, 0.0f,
            -250.0f, -0.0f, 250.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -250.0f, -0.0f, -250.0f, 0.0f, 1.0f, 0.0f, 0.0f, 250.0f,

            250.0f, -0.0f, 250.0f, 0.0f, 1.0f, 0.0f, 250.0f, 0.0f,
            -250.0f, -0.0f, -250.0f, 0.0f, 1.0f, 0.0f, 0.0f, 250.0f,
            250.0f, -0.0f, -250.0f, 0.0f, 1.0f, 0.0f, 250.0f, 250.0f
    };

    // configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    worldShader.Use();
    worldShader.SetInteger("diffuseTexture", 0);
    worldShader.SetInteger("shadowMap", 1);

    //    worldShader.SetVector3f("light.direction", -20.2f, -10.0f, -10.3f);
//    worldShader.SetVector3f("viewPos", context->camera->Position);
//
//    worldShader.SetVector3f("light.ambient", 0.7, 0.7, 0.7);
//    worldShader.SetVector3f("light.diffuse", 0.5, 0.5, 0.5);
//    worldShader.SetVector3f("light.specular", 0.7, 0.7, 0.7);
    worldShader.SetFloat("material.shininess", 0.5f);

    depthQuadShader.Use();
    depthQuadShader.SetInteger("depthMap", 0);

    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glBindVertexArray(0);

    return 0;
}

int GameWorld::update() {

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

    return 0;
}

int GameWorld::draw() {
    //    lightPos = glm::vec3(player->position.x, player->position.y + 1.0f, player->position.z);

    Shader worldShader = ResourceManager::GetShader("world");
    Shader depthQuadShader = ResourceManager::GetShader("depthQuad");
    Shader depthShader = ResourceManager::GetShader("shadowMappingDepth");

    lightPos.x = sin(glfwGetTime()) * 3.0f;
    lightPos.z = cos(glfwGetTime()) * 2.0f;
    lightPos.y = 2.0 + cos(glfwGetTime()) * 1.0f;

    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
//    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // render scene from light's point of view
    depthShader.Use();
    depthShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);
    this->renderShadowScene();

    // 2. render scene as normal using the generated depth/shadow map
    // --------------------------------------------------------------
    glViewport(0, 0, context->windowW * 2, context->windowH * 2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 projection = glm::perspective(context->camera->Zoom, (float) context->windowW / (float) context->windowH, 0.1f, 100.0f);
    glm::mat4 view = context->camera->GetViewMatrix();

    worldShader.Use();
    worldShader.SetMatrix4("projection", projection);
    worldShader.SetMatrix4("view", view);

    // set light uniforms
    worldShader.SetVector3f("viewPos", context->camera->Position);
    worldShader.SetVector3f("lightPos", lightPos);
    worldShader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

    // Set texture and depth texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("rock").ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // Render scene as normal
    this->renderScene(worldShader, false);

//    depthQuadShader.Use();
//    depthQuadShader.SetFloat("near_plane", near_plane);
//    depthQuadShader.SetFloat("far_plane", far_plane);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, depthMap);

    if (this->skybox)
        this->skybox->renderScene(worldShader, false);

    if (this->hud)
        this->hud->renderScene(worldShader, false);

//    if (this->player)
//        this->player->renderScene(worldShader, false);

    return 0;
}

int GameWorld::renderScene(Shader &shader, bool isShadowRender) {

    for (BaseRenderObj *obj : this->renderObjects) {
        obj->renderScene(shader, isShadowRender);
    }

    /*
     *
     * Floor
     *
     */

    //  glCullFace(GL_FRONT);
    glm::mat4 model;
    shader.SetMatrix4("model", model);
    glBindVertexArray(planeVAO);
    ResourceManager::GetTexture("grass").Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //    glCullFace(GL_BACK);

    /*
     *
     * Trees
     *
     */
    for (unsigned int i = 0; i < trees; i++) {

        glm::mat4 model;
        glm::vec4 random = treePositions[i];
        model = glm::translate(model, glm::vec3(random.x, random.y, random.z));
        model = glm::scale(model, glm::vec3(random.w / 100));

        shader.SetMatrix4("model", model);

        treeModel->Draw(&shader);
    }

    /*
     *
     * Rocks
     *
     */
    for (unsigned int i = 0; i < rocks; i++) {
        glm::mat4 model;
        glm::vec4 random = rockPositions[i];

        model = glm::translate(model, glm::vec3(random.x, random.y, random.z));
        model = glm::scale(model, glm::vec3(random.w / 100));

        shader.SetMatrix4("model", model);

        rockModel->Draw(&shader);
    }

    return 0;
}

int GameWorld::renderShadowScene() {
// Shader config
    Shader depthShader = ResourceManager::GetShader("shadowMappingDepth");

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("rock").ID);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    this->renderScene(depthShader, true);

    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int GameWorld::destroy() {
    return 0;
}