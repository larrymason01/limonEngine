//
// Created by engin on 28.07.2018.
//

#include "MenuPlayer.h"
#include "../../Options.h"
#include "../../GUI/GUIRenderable.h"

void MenuPlayer::move(moveDirections direction __attribute((unused))) {
    //menu player can't move
}

void MenuPlayer::rotate(float xPosition, float yPosition, float xChange __attribute__((unused)), float yChange __attribute__((unused))) {
    glm::vec2 cursorPosition((options->getScreenWidth()/2.0f)  + xPosition * options->getScreenWidth() /2.0f,
                             (options->getScreenHeight()/2.0f) - yPosition * options->getScreenHeight()/2.0f);//y is negative, because sdl reports opposite of OpenGL

    cursor->setTranslate(cursorPosition);
}

void MenuPlayer::getWhereCameraLooks(glm::vec3 &fromPosition, glm::vec3 &toPosition) const {
    fromPosition = this->getPosition();

    // Many thanks to http://antongerdelan.net/opengl/raycasting.html
    glm::vec2 cursorPosition = cursor->getTranslate();
    /* to normalized device coordinates */
    float normalizedDeviceCoordinateX = (2.0f * cursorPosition.x) / options->getScreenWidth() - 1.0f;
    float normalizedDeviceCoordinateY = (2.0f * cursorPosition.y) / options->getScreenHeight() - 1.0f;
    /* homogeneous clip coordinates */
    glm::vec4 clipSpaceRay = glm::vec4(normalizedDeviceCoordinateX, normalizedDeviceCoordinateY, -1.0, 1.0);
    /* eye coordinates */
    float aspect = float(options->getScreenHeight()) / float(options->getScreenWidth());
    glm::mat4 perspectiveProjectionMatrix = glm::perspective(options->PI/3.0f, 1.0f / aspect, 0.1f, 1000.0f);
    glm::vec4 cameraSpaceRay = glm::inverse(perspectiveProjectionMatrix) * clipSpaceRay;
    cameraSpaceRay = glm::vec4(cameraSpaceRay.x, cameraSpaceRay.y, -1.0, 0.0);

    /* world coordinate */
    glm::mat4 cameraTransformMatrix = glm::lookAt(position, center + position, up);
    glm::vec4 worldSpaceRay = glm::inverse(cameraTransformMatrix) * cameraSpaceRay;
    // switch to vec3 and normalize
    toPosition = glm::normalize(glm::vec3(worldSpaceRay.x, worldSpaceRay.y, worldSpaceRay.z));
}

MenuPlayer::MenuPlayer(Options *options, GUIRenderable* cursor):
        Player(cursor),
        options(options),
        dirty(true),
        position(),
        center(glm::vec3(0,0,-1)),
        up(glm::vec3(0,1,0)),
        right(glm::vec3(-1,0,0)),
        view(glm::quat(0,0,0,-1)) {
    worldSettings.debugMode = DEBUG_DISABLED;
    worldSettings.audioPlaying = false;
    worldSettings.worldSimulation = false;
    worldSettings.editorShown = false;
    worldSettings.cursorFree = true;
    worldSettings.resetAnimations = false;
    worldSettings.menuInteraction = true;
}