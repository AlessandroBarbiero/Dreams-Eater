//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SideScrollingCamera.hpp"
#include "DreamGame.hpp"

using namespace glm;

SideScrollingCamera::SideScrollingCamera(GameObject *gameObject)
        : Component(gameObject)
{
    camera.setOrthographicProjection(DreamGame::windowSize.y / 2, -1, 1);
}

sre::Camera &SideScrollingCamera::getCamera() {
    return camera;
}

void SideScrollingCamera::update(float deltaTime) {
    if (followObject != nullptr){
        auto position = followObject->getPosition();

        position.x += offset.x;
        position.y += offset.y;
        gameObject->setPosition(position);
        vec3 eye (position, 0);
        vec3 at (position, -1);
        vec3 up (0, 1, 0);
        camera.lookAt(eye, at, up);
    }
}

void SideScrollingCamera::setFollowObject(std::shared_ptr<GameObject> followObject, glm::vec2 offset) {
    this->followObject = followObject;
    this->offset = offset;
}

//from SRE examples
glm::vec2 SideScrollingCamera::getWindowCoordinates(glm::vec3 worldpos) {
    using namespace glm;
    vec4 w(worldpos, 1.0f);
    auto inst = sre::Renderer::instance;
    auto viewport = static_cast<glm::vec2>(inst->getDrawableSize());
    vec4 clipSpace = camera.getProjectionTransform(viewport) * camera.getViewTransform() * w;
    vec4 ndc = clipSpace / clipSpace.w;
    ndc.y *= -1;
    vec4 winCoords = ndc * 0.5f + 0.5f;
    return vec2(winCoords) * vec2(inst->getWindowSize());
}
