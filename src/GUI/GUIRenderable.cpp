//
// Created by engin on 24.03.2016.
//

#include "GUIRenderable.h"
#include "GUILayer.h"

GUIRenderable::GUIRenderable(GLHelper *glHelper, uint32_t id) : Renderable(glHelper), worldID(id) {
    vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));

    faces.push_back(glm::mediump_uvec3(0, 1, 2));//front
    faces.push_back(glm::mediump_uvec3(0, 2, 3));

    uint_fast32_t vbo;
    glHelper->bufferVertexData(vertices, faces, vao, vbo, 2, ebo);
    bufferObjects.push_back(vbo);

    textureCoordinates.push_back(glm::vec2(0.0f, 1.0f));
    textureCoordinates.push_back(glm::vec2(1.0f, 1.0f));
    textureCoordinates.push_back(glm::vec2(1.0f, 0.0f));
    textureCoordinates.push_back(glm::vec2(0.0f, 0.0f));
    glHelper->bufferVertexTextureCoordinates(textureCoordinates, vao, vbo, 3);

    renderProgram = new GLSLProgram(glHelper, "./Data/Shaders/GUI/vertex.glsl", "./Data/Shaders/GUI/fragment.glsl", false);
    name = "Gui_Element_" + std::to_string(worldID);
}

void GUIRenderable::renderDebug(BulletDebugDrawer* debugDrawer) {
    float up    = transformation.getTranslate().y + transformation.getScale().y;
    float down  = transformation.getTranslate().y - transformation.getScale().y;

    float right = transformation.getTranslate().x + transformation.getScale().x;
    float left  = transformation.getTranslate().x - transformation.getScale().x;

    //now build 4 lines;
    debugDrawer->drawLine(glm::vec3(left, up, 0.0f), glm::vec3(left, down, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f), false);
    debugDrawer->drawLine(glm::vec3(right, up, 0.0f), glm::vec3(right, down, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f), false);
    debugDrawer->drawLine(glm::vec3(left, up, 0.0f), glm::vec3(right, up, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f), false);
    debugDrawer->drawLine(glm::vec3(left, down, 0.0f), glm::vec3(right, down, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f), false);
}

void GUIRenderable::addedToLayer(GUILayer *layer) {
    parentLayers.push_back(layer);
}

GUIRenderable::~GUIRenderable() {
    for (size_t i = 0; i < parentLayers.size(); ++i) {
        parentLayers[i]->removeGuiElement(this->getWorldID());
    }
}
