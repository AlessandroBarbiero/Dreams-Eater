#pragma once

#include "Component.hpp"

class RoomComponent : public Component {
public:
    explicit RoomComponent(GameObject *gameObject);
    virtual ~RoomComponent();

    void setRoomSize(glm::vec2);
    glm::vec2 getRoomSize();
    glm::vec2 getRoomSizeInPixels();

    void buildWalls();
    void buildFloor();

private:
    std::vector<std::shared_ptr<GameObject>> roomObjects;
    glm::vec2 roomSize;

    std::shared_ptr<GameObject> RoomComponent::spawnWall(sre::Sprite spriteWall, glm::vec2 pos);
    std::shared_ptr<GameObject> RoomComponent::spawnFloor(sre::Sprite spriteFloor, glm::vec2 pos);
};

