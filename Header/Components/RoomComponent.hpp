#pragma once

#include "Component.hpp"
#include "RoomBuilder.hpp"
#include "Level.hpp"

class RoomComponent : public Component {
public:
    explicit RoomComponent(GameObject *gameObject);
    virtual ~RoomComponent();

    void update(float deltaTime) override;

    void setRoomSize(glm::vec2);
    glm::vec2 getRoomSize();
    glm::vec2 getRoomSizeInPixels();

    void buildWalls();
    void buildFloor();

    void unlockDoor(DoorPosition doorPosition);
    void lockDoor(DoorPosition doorPosition);
    void unlockDoor(GameObject* doorPosition);
    void lockDoor(GameObject* doorPosition);

    RoomType roomType;

    std::map<DoorPosition, glm::vec2> doorEntrances;
    bool doorsLocked = true;

    void onGui() override;

private:
    std::vector<std::shared_ptr<GameObject>> roomObjects;
    glm::vec2 roomSize;

    std::shared_ptr<GameObject> spawnWall(sre::Sprite spriteWall, glm::vec2 pos);
    std::shared_ptr<GameObject> spawnFloor(sre::Sprite spriteFloor, glm::vec2 pos, float scale);
    std::shared_ptr<GameObject> spawnDoor(sre::Sprite spriteDoor, glm::vec2 pos, Door door);

    void DoorsToPositions(std::vector<Door> doors, int(&skips)[4][2]); // Pairs are ((Top, Bottom),(Left, Right))
    std::vector<Door> doors;
    std::map<DoorPosition, GameObject*> doorMap;

    TileSetWalls tileSetWalls;
    TileSetFloor tileSetFloor;

    const static std::map<TileSetWalls, std::string> TileSetWallsToString;
    const static std::map<TileSetFloor, std::string> TileSetFloorToString;

    Level* level;

    friend class LevelGuiComponent;
    friend class RoomBuilder;
    friend class Level;
    friend class DreamGame;
};

