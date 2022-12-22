#include "LevelBuilder.hpp"
#include "RoomBuilder.hpp"

Level LevelBuilder::createLevel(LevelSettings settings) {
    Level level = Level();

    level.name = settings.name;
    level.difficulty = settings.difficulty;
    level.rooms = settings.rooms;
    //level.player = player;
    level.startRoom = 0;


    int tileSet = rand() % 5;
    

    RoomSettings spawnRoomSettings;
    spawnRoomSettings.name = "Spawn";
    spawnRoomSettings.roomType = SpawnRoom;
    spawnRoomSettings.position = { 0,0 };

    spawnRoomSettings.tileSetFloor = static_cast<TileSetFloor>(tileSet);
    spawnRoomSettings.tileSetWalls = static_cast<TileSetWalls>(tileSet);
    
    // Room Size
    int roomSize = rand() % 100;
    if (roomSize < 50) {
        spawnRoomSettings.roomSize = Medium;
        spawnRoomSettings.size = { 7, 7 };
    }
    else if (roomSize < 70) {
        spawnRoomSettings.roomSize = Wide;
        spawnRoomSettings.size = { 14, 7 };
    }
    else if (roomSize < 90) {
        spawnRoomSettings.roomSize = Long;
        spawnRoomSettings.size = { 7, 14 };
    }
    else {
        spawnRoomSettings.roomSize = Large;
        spawnRoomSettings.size = { 14, 14 };
    }
    

    


    level.roomSettings.push_back(spawnRoomSettings);
    level.roomEntered.push_back(false);

    return level;
}