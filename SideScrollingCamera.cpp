//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SideScrollingCamera.hpp"
#include "DreamGame.hpp"

using namespace glm;

SideScrollingCamera::SideScrollingCamera(GameObject *gameObject)
        : Component(gameObject)
{
}

sre::Camera &SideScrollingCamera::getCamera() {
    return camera;
}

void SideScrollingCamera::update(float deltaTime) {
    if (followObject != nullptr){
        auto position = followObject->getPosition();

        position.x += offset.x;
        position.y = offset.y;
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
