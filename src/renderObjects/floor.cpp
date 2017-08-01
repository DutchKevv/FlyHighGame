//
// Created by Kewin Brandsma on 01/08/2017.
//

#include "floor.h"

#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/resourceManager.h>

static unsigned int VBO, VAO;

static float vertices[] = {
        500.0f, -0.0f,  500.0f,  500.0f, 0.0f,
        -500.0f, -0.0f,  500.0f,  0.0f, 0.0f,
        -500.0f, -0.0f, -500.0f,  0.0f, 500.0f,

        500.0f, -0.0f,  500.0f,  500.0f, 0.0f,
        -500.0f, -0.0f, -500.0f,  0.0f, 500.0f,
        500.0f, -0.0f, -500.0f,  1.0f, 1.0f
};

Floor::Floor() : BaseRenderObj() {

}

int Floor::init(Context &context) {
    BaseRenderObj::init(context);

    /* FLOOR */
    ResourceManager::LoadShader("assets/shaders/triangle.v.glsl", "assets/shaders/triangle.f.glsl", NULL, "triangle");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    ResourceManager::LoadTexture("assets/textures/grass.jpg", GL_FALSE, "grass");

    return 0;
}

int Floor::update() {
    return 0;
}

int Floor::draw() {
    BaseRenderObj::draw();

    Shader shader = ResourceManager::GetShader("triangle");
    ResourceManager::GetTexture("grass").Bind();

    shader.Use();

    glBindVertexArray(VAO);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("grass").ID);

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    // create transformations
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 trans;

    // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::lookAt(context.camera->Position, context.camera->Position + context.camera->Front, context.camera->Up);
//    view = glm::mat4(glm::mat3(context.camera->GetViewMatrix()));
    projection = glm::perspective(glm::radians(45.0f), (float)context.windowW / (float)context.windowH, 0.1f, 100.0f);

    // camera/view transformation
    shader.SetMatrix4("projection", projection);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);

    return 0;
};

int Floor::destroy() {
    // Cleanup VBO
//    shader->destroy();
    return 0;
};