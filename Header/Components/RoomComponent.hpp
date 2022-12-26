#pragma once

#include "Component.hpp"
#include "RoomBuilder.hpp"
#include "Level.hpp"

class RoomComponent : public Component {
public:
    explicit RoomComponent(GameObject *gameObject);
    virtual ~RoomComponent();

    void setRoomSize(glm::vec2);
    glm::vec2 getRoomSize();
    glm::vec2 getRoomSizeInPixels();

    void buildWalls();
    void buildFloor();

    RoomType roomType;

    std::map<DoorPosition, glm::vec2> doorEntrances;

    void onGui() override;

private:
    std::vector<std::shared_ptr<GameObject>> roomObjects;
    glm::vec2 roomSize;

    std::shared_ptr<GameObject> spawnWall(sre::Sprite spriteWall, glm::vec2 pos);
    std::shared_ptr<GameObject> spawnFloor(sre::Sprite spriteFloor, glm::vec2 pos, float scale);
    std::shared_ptr<GameObject> spawnDoor(sre::Sprite spriteDoor, glm::vec2 pos, Door door);

    void DoorsToPositions(std::vector<Door> doors, int(&skips)[4][2]); // Pairs are ((Top, Bottom),(Left, Right))
    std::vector<Door> doors;

    TileSetWalls tileSetWalls;
    TileSetFloor tileSetFloor;

    const static std::map<TileSetWalls, std::string> TileSetWallsToString;
    const static std::map<TileSetFloor, std::string> TileSetFloorToString;

    Level* level;

    friend class RoomBuilder;
    friend class Level;
    friend class DreamGame;
};

