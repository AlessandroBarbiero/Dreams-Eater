#include "DreamGame.hpp"
#include "RoomBuilder.hpp"
#include "RoomComponent.hpp"
#include "PhysicsComponent.hpp"

std::shared_ptr<GameObject> RoomBuilder::createRoom(RoomSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto roomObject = game->currentScene->createGameObject();
    roomObject->name = settings.name;
    //roomObject->setPosition(settings.position);
    roomObject->setPosition({0,0});
    auto roomPhys = roomObject->addComponent<PhysicsComponent>();
    auto room = roomObject->addComponent<RoomComponent>();
    room->setRoomSize(settings.size);

    room->roomType = settings.roomType;

    room->tileSetWalls = settings.tileSetWalls;
    room->tileSetFloor = settings.tileSetFloor;

    room->doors = settings.doors;
    room->level = game->level.get();

    room->buildFloor();
    room->buildWalls();

    return roomObject;
}