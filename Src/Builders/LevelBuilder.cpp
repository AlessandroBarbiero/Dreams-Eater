#include "LevelBuilder.hpp"
#include "RoomBuilder.hpp"
#include <deque>
#include <iostream>


std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
    return { lhs.first + rhs.first, lhs.second + rhs.second };
}

std::shared_ptr<Level> LevelBuilder::createLevel(LevelSettings settings) {
    std::shared_ptr<Level> level = std::make_shared<Level>();

    level->name = settings.name;
    level->difficulty = settings.difficulty;
    level->rooms = settings.rooms;
    //level.player = player;
    level->startRoom = 0;

    generateLayout(level);

    /*
    int tileSet = rand() % 5;
    
    RoomSettings spawnRoomSettings;
    spawnRoomSettings.name = "Spawn";
    spawnRoomSettings.roomType = SpawnRoom;
    spawnRoomSettings.position = { 0,0 };

    spawnRoomSettings.tileSetFloor = static_cast<TileSetFloor>(tileSet);
    spawnRoomSettings.tileSetWalls = static_cast<TileSetWalls>(tileSet);
    
    // Room Size
    int roomSize = rand() % 100;
    int maxDoors;
    if (roomSize < 50) {
        spawnRoomSettings.roomSize = Medium;
        spawnRoomSettings.size = { 7, 7 };
        maxDoors = 4;
    }
    else if (roomSize < 70) {
        spawnRoomSettings.roomSize = Wide;
        spawnRoomSettings.size = { 14, 7 };
        maxDoors = 6;
    }
    else if (roomSize < 90) {
        spawnRoomSettings.roomSize = Long;
        spawnRoomSettings.size = { 7, 14 };
        maxDoors = 6;
    }
    else {
        spawnRoomSettings.roomSize = Large;
        spawnRoomSettings.size = { 14, 14 };
        maxDoors = 8;
    }
    
    // Randomize doors
    int doors = rand() % maxDoors;

    if (doors > level.rooms) {
        doors = level.rooms;
    }

    // Prepare doors
    //DoorPosition possible[8];
    std::vector<DoorPosition> possible;
    switch (spawnRoomSettings.roomSize)
    {
    case Medium:
        possible = {Top, Bottom, Left, Right};
        break;
    case Wide:
        possible = { TopLeft, TopRight, BottomLeft, BottomRight, Left, Right };
        break;
    case Long:
        possible = { Top, Bottom, LeftTop, LeftBottom, RightTop, RightBottom };
        break;
    case Large:
        possible = { TopLeft, TopRight, BottomLeft, BottomRight, LeftTop, LeftBottom, RightTop, RightBottom };
        break;
    }

    for (int i = 0; i < doors; i++)
    {
        Door d;
        // Select position
        int pos = 0;
        if (spawnRoomSettings.roomSize == Medium) {
            int pos = rand() % 4;
            // 0,1,2,3 = Top,Bottom,Left,Right

        }
        else if (spawnRoomSettings.roomSize != Large) {
            int pos = rand() % 6;
        }
        else {
            int pos = rand() % 8;
        }
        d.position = possible[pos];
        spawnRoomSettings.doors.push_back(d);

        // Generate corresponding room
    }


    level.roomSettings.push_back(spawnRoomSettings);
    level.roomEntered.push_back(false);
    */
    




    return level;
}

void LevelBuilder::generateLayout(std::shared_ptr<Level> level) {



    std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> roomMap;

    roomMap[{ 0, 0 }] = std::make_shared<RoomSettings>();
    std::shared_ptr<RoomSettings> spawnRoomSettings = generateRoom(level, roomMap, level->rooms, 0, 0, { 0,0 }, true);

    level->roomSettings.push_back(spawnRoomSettings);
    level->roomEntered.push_back(false);
    level->roomObjects.push_back({});


    insertRoomInMap(roomMap, spawnRoomSettings);

    //std::vector<std::tuple<int, Door>> roomsToGenerate; // int is where the room was generated from. Door contains roomId and doorposition
    int nextRoomId = 1;
    int roomsLeft = level->rooms - 1;



    std::deque<std::pair<int, Door>> roomsToGenerate;
    for (Door &d : spawnRoomSettings->doors) {
        d.destinationRoom = nextRoomId;

        roomsToGenerate.push_back({ 0, d }); // From spawn, to nextRoomI
        // Insert room origin immediately
        glm::vec2 pos = calculatePosition(spawnRoomSettings, d);
        roomMap[{pos.x, pos.y}] = std::make_shared<RoomSettings>();
        roomMap[{pos.x, pos.y}]->id = nextRoomId;
        nextRoomId++;
        roomsLeft--;
    }

    if (roomsToGenerate.size() == 0 && roomsLeft > 0) {
        // Create a new room
        std::cout << "Need more rooms. Adding new door" << std::endl;
        for (int i = nextRoomId - 1; i >= 0; i--) {
            auto r = level->roomSettings[i];
            std::vector<std::pair<int, DoorPosition>> neighbours = getNeighbours(roomMap, r->positions, r->roomSize);
            std::map<DoorPosition, bool> chosen;
            for (std::pair<int, DoorPosition> n : neighbours) {
                chosen[n.second] = true;
            }
            auto newDoors = generateDoors(r, chosen, 1);
            if (newDoors.size() > 0) {
                for (Door& d : r->doors) {
                    // id = -1 means room doesn't exist yet
                    if (d.destinationRoom == -1) {
                        d.destinationRoom = nextRoomId;
                        roomsToGenerate.push_back({ r->id, d });
                        // Insert room origin immediately
                        glm::vec2 pos = calculatePosition(r, d);
                        roomMap[{pos.x, pos.y}] = std::make_shared<RoomSettings>();
                        roomMap[{pos.x, pos.y}]->id = nextRoomId;
                        nextRoomId++;
                        roomsLeft--;
                    }
                    std::cout << d.destinationRoom << std::endl;
                }
                break;
            }
        }
    }


    while (roomsToGenerate.size() > 0) {
        //int from, Door door
        auto x = roomsToGenerate.front();
        roomsToGenerate.pop_front();
        int from = x.first; 
        Door door = x.second;

        // Position
        std::shared_ptr<RoomSettings> fromRoom = level->roomSettings[from];

        glm::vec2 pos = calculatePosition(fromRoom, door);

        std::shared_ptr<RoomSettings> r = generateRoom(level, roomMap, roomsLeft, door.destinationRoom, from, pos);
        insertRoomInMap(roomMap, r);
        level->roomSettings.push_back(r);
        level->roomEntered.push_back(false);
        level->roomObjects.push_back({});

        for (Door &d : r->doors) {
            // id = -1 means room doesn't exist yet
            if (d.destinationRoom == -1) {
                d.destinationRoom = nextRoomId;
                roomsToGenerate.push_back({r->id, d});
                // Insert room origin immediately
                glm::vec2 pos = calculatePosition(r, d);
                roomMap[{pos.x, pos.y}] = std::make_shared<RoomSettings>();
                roomMap[{pos.x, pos.y}]->id = nextRoomId;
                nextRoomId++;
                roomsLeft--;
            }
            std::cout << d.destinationRoom << std::endl;
        }
        if (roomsToGenerate.size() == 0 && roomsLeft > 0) {
            // Create a new room
            std::cout << "Need more rooms. Adding new door" << std::endl;
            for (int i = nextRoomId - 1; i >= 0; i--) {
                auto r = level->roomSettings[i];
                std::vector<std::pair<int, DoorPosition>> neighbours = getNeighbours(roomMap, r->positions, r->roomSize);
                std::map<DoorPosition, bool> chosen;
                for (std::pair<int, DoorPosition> n : neighbours) {
                    chosen[n.second] = true;
                }
                auto newDoors = generateDoors(r, chosen, 1);
                if (newDoors.size() > 0) {
                    for (Door& d : r->doors) {
                        // id = -1 means room doesn't exist yet
                        if (d.destinationRoom == -1) {
                            d.destinationRoom = nextRoomId;
                            roomsToGenerate.push_back({ r->id, d });
                            // Insert room origin immediately
                            glm::vec2 pos = calculatePosition(r, d);
                            roomMap[{pos.x, pos.y}] = std::make_shared<RoomSettings>();
                            roomMap[{pos.x, pos.y}]->id = nextRoomId;
                            nextRoomId++;
                            roomsLeft--;
                        }
                        std::cout << d.destinationRoom << std::endl;
                    }
                    break;
                }
            }
        }
    }

    // Create backfacing doors after all have been generated?
    std::cout << "fixDoors" << std::endl;
    fixDoors(level);

    // Last room generated is the boss room
    level->roomSettings[level->rooms - 1]->roomType = BossRoom;

    std::cout << "Rooms generated: " << level->roomSettings.size() << std::endl;
    std::cout << "Rooms not generated: " << level->rooms - level->roomSettings.size() << std::endl;
}

std::shared_ptr<RoomSettings> LevelBuilder::generateRoom(std::shared_ptr<Level> level, std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> roomMap, int roomsLeft, int roomId, int roomEnteredFrom, glm::vec2 position, bool spawn) {
    std::cout << "Room {" << roomId << "}" << std::endl;
    
    int tileSet = rand() % 5;


    auto roomSettings = roomMap[{position.x, position.y}];
    //auto roomSettings = std::make_shared<RoomSettings>();
    //RoomSettings roomSettings;
    if (spawn) {
        roomSettings->name = "Spawn";
        roomSettings->roomType = SpawnRoom;
    }
    else {
        roomSettings->name = "room" + std::to_string(roomId);
        int roomType = rand() % 100;
        if (roomType < 1) {
            roomSettings->roomType = EnemyRoom;
        }
        else {
            roomSettings->roomType = PowerUpRoom;
        }
    }
    roomSettings->id = roomId;
    roomSettings->position = position;
    //roomSettings->positions[0] = position;

    roomSettings->tileSetFloor = static_cast<TileSetFloor>(tileSet);
    roomSettings->tileSetWalls = static_cast<TileSetWalls>(tileSet);


    // Room Size
    int roomSize = rand() % 100;
    int maxDoors;
    if (roomSize < 50 || spawn) {
        roomSettings->roomSize = Medium;
    }
    else if (roomSize < 70) {
        roomSettings->roomSize = Wide;
    }
    else if (roomSize < 90) {
        roomSettings->roomSize = Long;
    }
    else {
        roomSettings->roomSize = Large;
    }

    // Can't overlap other rooms.
    RoomSize finalRoomSize = expandRoom(roomMap, position, roomSettings->roomSize, roomSettings->positions);
    switch (finalRoomSize) {
    case Medium:
        roomSettings->roomSize = Medium;
        roomSettings->size = { 7, 7 };
        maxDoors = 4;
        break;
    case Wide:
        roomSettings->roomSize = Wide;
        roomSettings->size = { 14, 7 };
        maxDoors = 6;
        break;
    case Long:
        roomSettings->roomSize = Long;
        roomSettings->size = { 7, 14 };
        maxDoors = 6;
        break;
    case Large:
        roomSettings->roomSize = Large;
        roomSettings->size = { 14, 14 };
        maxDoors = 8;
        break;
    }

    // Check neighbours
    std::vector<std::pair<int, DoorPosition>> neighbours = getNeighbours(roomMap, roomSettings->positions, roomSettings->roomSize);
    std::map<DoorPosition, bool> chosen;
    for (std::pair<int, DoorPosition> n : neighbours) {
        int x = rand() % 100;
        // Door to neighbours.
        chosen[n.second] = true;
        if (x < 50) {
            Door newDoor;
            newDoor.destinationRoom = n.first;
            newDoor.position = n.second;
            roomSettings->doors.push_back(newDoor);
            // TODO: Make sure there is a corresponding door on the other side.
        }
    }

    maxDoors -= neighbours.size();

    // Randomize doors
    int doors;
    if (maxDoors >= 1) {
        doors = rand() % (maxDoors+1);
    }
    else {
        doors = 0;
    }

    if (doors > roomsLeft) {
        doors = roomsLeft;
    }


    auto generatedDoors = generateDoors(roomSettings, chosen, doors);
    

    return roomSettings;
}

std::vector<Door> LevelBuilder::generateDoors(std::shared_ptr<RoomSettings> roomSettings, std::map<DoorPosition, bool> &chosen, int doors) {
    // Prepare doors
    std::vector<Door> generatedDoors;
    DoorPosition possible[8];
    int possibleLast = 0;
    //std::vector<DoorPosition> possible;
    switch (roomSettings->roomSize)
    {
    case Medium:
        possibleLast = 4;
        possible[0] = Top;
        possible[1] = Bottom;
        possible[2] = Left;
        possible[3] = Right;
        //possible = { Top, Bottom, Left, Right };
        break;
    case Wide:
        possibleLast = 6;
        possible[0] = TopLeft;
        possible[1] = TopRight;
        possible[2] = BottomLeft;
        possible[3] = BottomRight;
        possible[4] = Left;
        possible[5] = Right;
        //possible = { TopLeft, TopRight, BottomLeft, BottomRight, Left, Right };
        break;
    case Long:
        possibleLast = 6;
        possible[0] = Top;
        possible[1] = Bottom;
        possible[2] = LeftTop;
        possible[3] = LeftBottom;
        possible[4] = RightTop;
        possible[5] = RightBottom;
        //possible = { Top, Bottom, LeftTop, LeftBottom, RightTop, RightBottom };
        break;
    case Large:
        possibleLast = 8;
        possible[0] = TopLeft;
        possible[1] = TopRight;
        possible[2] = BottomLeft;
        possible[3] = BottomRight;
        possible[4] = LeftTop;
        possible[5] = LeftBottom;
        possible[6] = RightTop;
        possible[7] = RightBottom;
        //possible = { TopLeft, TopRight, BottomLeft, BottomRight, LeftTop, LeftBottom, RightTop, RightBottom };
        break;
    }

    int generated = 0;
    //for (int i = 0; i < doors; i++)
    while (generated < doors)
    {
        Door d;
        // Select position
        int pos = rand() % possibleLast;
        // If not already chosen
        if (chosen.find(possible[pos]) == chosen.end()) {
            d.position = possible[pos];
            d.destinationRoom = -1; // Not set yet
            roomSettings->doors.push_back(d);
            generatedDoors.push_back(d);
            chosen[possible[pos]] = true; // Maybe not necessary
            generated += 1;
        }
        else {
            std::cout << "Room {" << roomSettings->id << "}: Position taken" << std::endl;
        }

        // Remove from possible positions
        possible[pos] = possible[possibleLast - 1];
        possibleLast--;
        if (possibleLast == -1) {
            std::cout << "No possible positions left: generated " << generated << " of " << doors << std::endl;
            break;
        }
    }
    return generatedDoors;
}

RoomSize LevelBuilder::expandRoom(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> roomMap, glm::vec2 position, RoomSize roomSize, glm::vec2 (&positions)[4]) {
    std::pair<int, int> pos = std::make_pair(position.x, position.y);
    auto up = roomMap.find(pos + std::make_pair(0, 1));
    auto down = roomMap.find(pos + std::make_pair(0, -1));
    auto left = roomMap.find(pos + std::make_pair(-1, 0));
    auto right = roomMap.find(pos + std::make_pair(1, 0));
    if (roomSize == Medium) {
        positions[0] = position;
    }
    if (roomSize == Wide) {
        if (left == roomMap.end() && right == roomMap.end()) {
            // Randomly expand to one side
            int temp = rand() % 2;
            if (temp == 0) {
                // Expand left
                positions[0] = position + glm::vec2(-1, 0);
                positions[1] = position;
            }
            else {
                // Expand Right
                positions[0] = position;
                positions[1] = position + glm::vec2(1, 0);
            }
        }
        else if (left == roomMap.end()) {
            // Expand left
            positions[0] = position + glm::vec2(-1, 0);
            positions[1] = position;
        }
        else if (right == roomMap.end()) {
            // Expand Right
            positions[0] = position;
            positions[1] = position + glm::vec2(1, 0);
        }
        else if (left != roomMap.end() && right != roomMap.end()) {
            // No room to expand
            roomSize = expandRoom(roomMap, position, Medium, positions);
        }
    }
    else if (roomSize == Long) {
        if (up == roomMap.end() && down == roomMap.end()) {
            // Randomly expand up or down
            int temp = rand() % 2;
            if (temp == 0) {
                // Expand up
                positions[0] = position + glm::vec2(0, 1);
                positions[1] = position;
            }
            else {
                // Expand down
                positions[0] = position;
                positions[1] = position + glm::vec2(0, -1);
            }
        }
        else if (up == roomMap.end()) {
            // Expand up
            positions[0] = position + glm::vec2(0, 1);
            positions[1] = position;
        }
        else if (down == roomMap.end()) {
            // Expand down
            positions[0] = position;
            positions[1] = position + glm::vec2(0, -1);
        }
        else if (up != roomMap.end() && down != roomMap.end()) {
            // No room to expand
            roomSize = expandRoom(roomMap, position, Medium, positions);
        }
    }
    else if (roomSize == Large) {
        int directions[4] = { 0, 1, 2, 3 }; // Up, Down, Left, Right
        int directionsLeft = 4;
        bool done = false;
        while (!done) {
            if (directionsLeft == 0) {
                // Cannot expand anywhere, reduce room size
                done = true;
                int temp = rand() % 2;
                if (temp == 0) {
                    roomSize = expandRoom(roomMap, position, Wide, positions);
                    if (roomSize == Medium) {
                        roomSize = expandRoom(roomMap, position, Long, positions);
                    }
                }
                else {
                    roomSize = expandRoom(roomMap, position, Long, positions);
                    if (roomSize == Medium) {
                        roomSize = expandRoom(roomMap, position, Wide, positions);
                    }
                }
                break;
            }
            int chosenDirection = rand() % directionsLeft;
            switch (directions[chosenDirection])
            {
            case 0:
                // Try upwards
                if (up == roomMap.end()) {
                    auto upLeft = roomMap.find(pos + std::make_pair(-1, 1));
                    auto upRight = roomMap.find(pos + std::make_pair(1, 1));
                    if (left == roomMap.end() && right == roomMap.end()) {
                        if (upLeft == roomMap.end() && upRight == roomMap.end()) {
                            int temp = rand() % 2;
                            if (temp == 0) {
                                // Expand left
                                positions[0] = position + glm::vec2(-1, 1);
                                positions[1] = position + glm::vec2(0, 1);
                                positions[2] = position + glm::vec2(-1, 0);
                                positions[3] = position;
                                done = true;
                            }
                            else {
                                // Expand right
                                positions[0] = position + glm::vec2(0, 1);
                                positions[1] = position + glm::vec2(1, 1);
                                positions[2] = position;
                                positions[3] = position + glm::vec2(1, 0);
                                done = true;
                            }
                        }
                        else if (upLeft == roomMap.end()) {
                            // Expand left
                            positions[0] = position + glm::vec2(-1, 1);
                            positions[1] = position + glm::vec2(0, 1);
                            positions[2] = position + glm::vec2(-1, 0);
                            positions[3] = position;
                            done = true;
                        }
                        else if (upRight == roomMap.end()) {
                            // Expand right
                            positions[0] = position + glm::vec2(0, 1);
                            positions[1] = position + glm::vec2(1, 1);
                            positions[2] = position;
                            positions[3] = position + glm::vec2(1, 0);
                            done = true;
                        }
                    }
                    else if (left == roomMap.end() && upLeft == roomMap.end()) {
                        // Expand left
                        positions[0] = position + glm::vec2(-1, 1);
                        positions[1] = position + glm::vec2(0, 1);
                        positions[2] = position + glm::vec2(-1, 0);
                        positions[3] = position;
                        done = true;
                    }
                    else if (right == roomMap.end() && upRight == roomMap.end()) {
                        // Expand right
                        positions[0] = position + glm::vec2(0, 1);
                        positions[1] = position + glm::vec2(1, 1);
                        positions[2] = position;
                        positions[3] = position + glm::vec2(1, 0);
                        done = true;
                    }
                }
                break;
            case 1:
                // Try downwards
                if (down == roomMap.end()) {
                    auto downLeft = roomMap.find(pos + std::make_pair(-1, -1));
                    auto downRight = roomMap.find(pos + std::make_pair(1, -1));
                    if (left == roomMap.end() && right == roomMap.end()) {
                        if (downLeft == roomMap.end() && downRight == roomMap.end()) {
                            int temp = rand() % 2;
                            if (temp == 0) {
                                // Expand left
                                positions[0] = position + glm::vec2(-1, 0);
                                positions[1] = position;
                                positions[2] = position + glm::vec2(-1, -1);
                                positions[3] = position + glm::vec2(0, -1);
                                done = true;
                            }
                            else {
                                // Expand right
                                positions[0] = position;
                                positions[1] = position + glm::vec2(1, 0);
                                positions[2] = position + glm::vec2(0, -1);
                                positions[3] = position + glm::vec2(1, -1);
                                done = true;
                            }
                        }
                        else if (downLeft == roomMap.end()) {
                            // Expand left
                            positions[0] = position + glm::vec2(-1, 0);
                            positions[1] = position;
                            positions[2] = position + glm::vec2(-1, -1);
                            positions[3] = position + glm::vec2(0, -1);
                            done = true;
                        }
                        else if (downRight == roomMap.end()) {
                            // Expand right
                            positions[0] = position;
                            positions[1] = position + glm::vec2(1, 0);
                            positions[2] = position + glm::vec2(0, -1);
                            positions[3] = position + glm::vec2(1, -1);
                            done = true;
                        }
                    }
                    else if (left == roomMap.end() && downLeft == roomMap.end()) {
                        // Expand left
                        positions[0] = position + glm::vec2(-1, 0);
                        positions[1] = position;
                        positions[2] = position + glm::vec2(-1, -1);
                        positions[3] = position + glm::vec2(0, -1);
                        done = true;
                    }
                    else if (right == roomMap.end() && downRight == roomMap.end()) {
                        // Expand right
                        positions[0] = position;
                        positions[1] = position + glm::vec2(1, 0);
                        positions[2] = position + glm::vec2(0, -1);
                        positions[3] = position + glm::vec2(1, -1);
                        done = true;
                    }
                }
                break;
            case 2:
                // Try left
                if (left == roomMap.end()) {
                    auto leftUp = roomMap.find(pos + std::make_pair(-1, 1));
                    auto leftDown = roomMap.find(pos + std::make_pair(-1, -1));
                    if (up == roomMap.end() && down == roomMap.end()) {
                        if (leftUp == roomMap.end() && leftDown == roomMap.end()) {
                            int temp = rand() % 2;
                            if (temp == 0) {
                                // Expand up
                                positions[0] = position + glm::vec2(-1, 1);
                                positions[1] = position + glm::vec2(0, 1);
                                positions[2] = position + glm::vec2(-1, 0);
                                positions[3] = position;
                                done = true;
                            }
                            else {
                                // Expand down
                                positions[0] = position + glm::vec2(-1, 0);
                                positions[1] = position;
                                positions[2] = position + glm::vec2(-1, -1);
                                positions[3] = position + glm::vec2(0, -1);
                                done = true;
                            }
                        }
                        else if (leftUp == roomMap.end()) {
                            // Expand up
                            positions[0] = position + glm::vec2(-1, 1);
                            positions[1] = position + glm::vec2(0, 1);
                            positions[2] = position + glm::vec2(-1, 0);
                            positions[3] = position;
                            done = true;
                        }
                        else if (leftDown == roomMap.end()) {
                            // Expand down
                            positions[0] = position + glm::vec2(-1, 0);
                            positions[1] = position;
                            positions[2] = position + glm::vec2(-1, -1);
                            positions[3] = position + glm::vec2(0, -1);
                            done = true;
                        }
                    }
                    else if (up == roomMap.end() && leftUp == roomMap.end()) {
                        // Expand up
                        positions[0] = position + glm::vec2(-1, 1);
                        positions[1] = position + glm::vec2(0, 1);
                        positions[2] = position + glm::vec2(-1, 0);
                        positions[3] = position;
                        done = true;
                    }
                    else if (down == roomMap.end() && leftDown == roomMap.end()) {
                        // Expand down
                        positions[0] = position + glm::vec2(-1, 0);
                        positions[1] = position;
                        positions[2] = position + glm::vec2(-1, -1);
                        positions[3] = position + glm::vec2(0, -1);
                        done = true;
                    }
                }
                break;
            case 3:
                // Try right
                if (right == roomMap.end()) {
                    auto rightUp = roomMap.find(pos + std::make_pair(-1, 1));
                    auto rightDown = roomMap.find(pos + std::make_pair(-1, -1));
                    if (up == roomMap.end() && down == roomMap.end()) {
                        if (rightUp == roomMap.end() && rightDown == roomMap.end()) {
                            int temp = rand() % 2;
                            if (temp == 0) {
                                // Expand up
                                positions[0] = position + glm::vec2(0, 1);
                                positions[1] = position + glm::vec2(1, 1);
                                positions[2] = position;
                                positions[3] = position + glm::vec2(1, 0);
                                done = true;
                            }
                            else {
                                // Expand down
                                positions[0] = position;
                                positions[1] = position + glm::vec2(1, 0);
                                positions[2] = position + glm::vec2(0, -1);
                                positions[3] = position + glm::vec2(1, -1);
                                done = true;
                            }
                        }
                        else if (rightUp == roomMap.end()) {
                            // Expand up
                            positions[0] = position + glm::vec2(0, 1);
                            positions[1] = position + glm::vec2(1, 1);
                            positions[2] = position;
                            positions[3] = position + glm::vec2(1, 0);
                            done = true;
                        }
                        else if (rightDown == roomMap.end()) {
                            // Expand down
                            positions[0] = position;
                            positions[1] = position + glm::vec2(1, 0);
                            positions[2] = position + glm::vec2(0, -1);
                            positions[3] = position + glm::vec2(1, -1);
                            done = true;
                        }
                    }
                    else if (up == roomMap.end() && rightUp == roomMap.end()) {
                        // Expand up
                        positions[0] = position + glm::vec2(0, 1);
                        positions[1] = position + glm::vec2(1, 1);
                        positions[2] = position;
                        positions[3] = position + glm::vec2(1, 0);
                        done = true;
                    }
                    else if (down == roomMap.end() && rightDown == roomMap.end()) {
                        // Expand down
                        positions[0] = position;
                        positions[1] = position + glm::vec2(1, 0);
                        positions[2] = position + glm::vec2(0, -1);
                        positions[3] = position + glm::vec2(1, -1);
                        done = true;
                    }
                }
                break;
            }
            directions[chosenDirection] = directions[directionsLeft -1];
            directionsLeft--;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        std::cout << "Room {" << position.x << "," << position.y << "} size: " << roomSize << " positions: (" << positions[i].x << ", " << positions[i].y << ")" << std::endl;
    }
    return roomSize;
}

std::vector<std::pair<int, DoorPosition>> LevelBuilder::getNeighbours(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> roomMap, glm::vec2(&positions)[4], RoomSize size) {
    std::map<std::pair<int, int>, RoomSettings>::iterator neighbour;
    std::vector<std::pair<int, DoorPosition>> neighbours;

    //std::pair<int, int> pos = std::make_pair(position.x, position.y);
    auto check = [&](std::pair<int, int> pos, std::pair<int, int> offset, DoorPosition d) {
        std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>>::iterator neighbour = roomMap.find(pos + offset);
        if (neighbour != roomMap.end()) {
            neighbours.push_back({neighbour->second->id, d});
        }
    };


    if (size == Medium) {
        std::pair<int, int> pos = std::make_pair(positions[0].x, positions[0].y);
        check(pos, { 0, 1 }, Top);
        check(pos, { 0, -1 }, Bottom);
        check(pos, { -1, 0 }, Left);
        check(pos, { 1, 0 }, Right);
    }
    else if (size == Wide) {
        std::pair<int, int> left = std::make_pair(positions[0].x, positions[0].y);
        std::pair<int, int> right = std::make_pair(positions[1].x, positions[1].y);
        check(left, { 0, 1 }, TopLeft);
        check(right, { 0, 1 }, TopRight);
        check(left, { 0, -1 }, BottomLeft);
        check(right, { 0, -1 }, BottomRight);
        check(left, { -1, 0 }, Left);
        check(right, { 1, 0 }, Right);
    }
    else if (size == Long) {
        std::pair<int, int> top = std::make_pair(positions[0].x, positions[0].y);
        std::pair<int, int> bottom = std::make_pair(positions[1].x, positions[1].y);
        check(top, { 0, 1 }, Top);
        check(bottom, { 0, -1 }, Bottom);
        check(top, { -1, 0 }, LeftTop);
        check(bottom, { -1, 0 }, LeftBottom);
        check(top, { 1, 0 }, RightTop);
        check(bottom, { 1, 0 }, RightBottom);
    }
    else {
        std::pair<int, int> topLeft = std::make_pair(positions[0].x, positions[0].y);
        std::pair<int, int> topRight = std::make_pair(positions[1].x, positions[1].y);
        std::pair<int, int> bottomLeft = std::make_pair(positions[2].x, positions[2].y);
        std::pair<int, int> bottomRight = std::make_pair(positions[3].x, positions[3].y);
        check(topLeft, { 0, 1 }, TopLeft);
        check(topRight, { 0, 1 }, TopRight);
        check(bottomLeft, { 0, -1 }, BottomLeft);
        check(bottomRight, { 0, -1 }, BottomRight);
        check(topLeft, { -1, 0 }, LeftTop);
        check(bottomLeft, { -1, 0 }, LeftBottom);
        check(topRight, { 1, 0 }, RightTop);
        check(bottomRight, { 1, 0 }, RightBottom);
    }

    return neighbours;
}

void LevelBuilder::insertRoomInMap(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> &map, std::shared_ptr<RoomSettings> room) {
    //glm::vec2 pos = room.position;
    int numberOfPositions = 0;
    switch (room->roomSize)
    {
    case Medium:
        numberOfPositions = 1;
        break;
    case Wide:
    case Long:
        numberOfPositions = 2;
        break;
    case Large:
        numberOfPositions = 4;
        break;
    }
    for (int i = 0; i < numberOfPositions; i++)
    {
        //std::cout << "Room {" << room->id << "} positions: (" << room->positions[i].x << ", " << room->positions[i].y << ")" << std::endl;
        map[{room->positions[i].x, room->positions[i].y}] = room;
    }
    /*
    map[pos] = room;
    switch (room.roomSize)
    {
    case Medium:
        break;
    case Wide:
        map[{pos.x + 1, pos.y}] = room;
        break;
    case Long:
        pos.y += 1;
        map[{pos.x, pos.y + 1}] = room;
        break;
    case Large:
        map[{pos.x + 1, pos.y}] = room;
        map[{pos.x, pos.y + 1}] = room;
        map[{pos.x + 1, pos.y + 1}] = room;
        break;
    }
    */
}

void LevelBuilder::fixDoors(std::shared_ptr<Level> level) {
    for each (auto room in level->roomSettings)
    {
        std::cout << "fixDoors Room {" << room->id << "}" << std::endl;
        for (auto &door : room->doors) {
            std::cout << "fixDoors Door {" << room->id << "}: Destination {" << door.destinationRoom << "}" << std::endl;
            if (door.destinationRoom == room->id) {
                std::cout << "What??" << std::endl;
            }
            auto destinationRoom = level->roomSettings[door.destinationRoom];
            glm::vec2 destinationPos = { 0,0 };
            switch (door.position)
            {
            case Top:
            case TopLeft:
                destinationPos = room->positions[0] + glm::vec2(0, 1);
                break;
            case TopRight:
                destinationPos = room->positions[1] + glm::vec2(0, 1);
                break;
            case Bottom:
                if (room->roomSize == Long) {
                    destinationPos = room->positions[1] + glm::vec2(0, -1);
                }
                else {
                    destinationPos = room->positions[0] + glm::vec2(0, -1);
                }
                break;
            case BottomLeft:
                if (room->roomSize == Wide) {
                    destinationPos = room->positions[0] + glm::vec2(0, -1);
                }
                else {
                    destinationPos = room->positions[2] + glm::vec2(0, -1);
                }
                break;
            case BottomRight:
                if (room->roomSize == Wide) {
                    destinationPos = room->positions[1] + glm::vec2(0, -1);
                }
                else {
                    destinationPos = room->positions[3] + glm::vec2(0, -1);
                }
                break;
            case Left:
            case LeftTop:
                destinationPos = room->positions[0] + glm::vec2(-1, 0);
                break;
            case LeftBottom:
                if (room->roomSize == Long) {
                    destinationPos = room->positions[1] + glm::vec2(-1, 0);
                }
                else {
                    destinationPos = room->positions[2] + glm::vec2(-1, 0);
                }
                break;
            case Right:
                if (room->roomSize == Wide) {
                    destinationPos = room->positions[1] + glm::vec2(1, 0);
                }
                else {
                    destinationPos = room->positions[0] + glm::vec2(1, 0);
                }
                break;
            case RightTop:
                if (room->roomSize == Long) {
                    destinationPos = room->positions[0] + glm::vec2(1, 0);
                }
                else {
                    destinationPos = room->positions[1] + glm::vec2(1, 0);
                }
                break;
            case RightBottom:
                if (room->roomSize == Long) {
                    destinationPos = room->positions[1] + glm::vec2(1, 0);
                }
                else {
                    destinationPos = room->positions[3] + glm::vec2(1, 0);
                }
                break;
            }
            Door newDoor;
            newDoor.destinationRoom = room->id;
            if (destinationRoom->roomSize == Medium) {
                switch (door.position)
                {
                case Top:
                case TopLeft:
                case TopRight:
                    newDoor.position = Bottom;
                    break;
                case Bottom:
                case BottomLeft:
                case BottomRight:
                    newDoor.position = Top;
                    break;
                case Left:
                case LeftTop:
                case LeftBottom:
                    newDoor.position = Right;
                    break;
                case Right:
                case RightTop:
                case RightBottom:
                    newDoor.position = Left;
                    break;
                }
            }
            else if (destinationRoom->roomSize == Long) {
                if (destinationPos == destinationRoom->positions[0]) {
                    // Top
                    switch (door.position)
                    {
                    case Bottom:
                    case BottomLeft:
                    case BottomRight:
                        newDoor.position = Top;
                        break;
                    case Left:
                    case LeftTop:
                    case LeftBottom:
                        newDoor.position = RightTop;
                        break;
                    case Right:
                    case RightTop:
                    case RightBottom:
                        newDoor.position = LeftTop;
                        break;
                    }
                }
                else {
                    // Bottom
                    switch (door.position)
                    {
                    case Top:
                    case TopLeft:
                    case TopRight:
                        newDoor.position = Bottom;
                        break;
                    case Left:
                    case LeftTop:
                    case LeftBottom:
                        newDoor.position = RightBottom;
                        break;
                    case Right:
                    case RightTop:
                    case RightBottom:
                        newDoor.position = LeftBottom;
                        break;
                    }
                }
            }
            else if (destinationRoom->roomSize == Wide) {
                if (destinationPos == destinationRoom->positions[0]) {
                    // Left
                    switch (door.position)
                    {
                    case Top:
                    case TopLeft:
                    case TopRight:
                        newDoor.position = BottomLeft;
                        break;
                    case Bottom:
                    case BottomLeft:
                    case BottomRight:
                        newDoor.position = TopLeft;
                        break;
                    case Right:
                    case RightTop:
                    case RightBottom:
                        newDoor.position = Left;
                        break;
                    }
                }
                else {
                    // Right
                    switch (door.position)
                    {
                    case Top:
                    case TopLeft:
                    case TopRight:
                        newDoor.position = BottomRight;
                        break;
                    case Bottom:
                    case BottomLeft:
                    case BottomRight:
                        newDoor.position = TopRight;
                        break;
                    case Left:
                    case LeftTop:
                    case LeftBottom:
                        newDoor.position = Right;
                        break;
                    }
                }
            }
            else {
                if (destinationPos == destinationRoom->positions[0]) {
                    // TopLeft
                    switch (door.position)
                    {
                    case Bottom:
                    case BottomLeft:
                    case BottomRight:
                        newDoor.position = TopLeft;
                        break;
                    case Right:
                    case RightTop:
                    case RightBottom:
                        newDoor.position = LeftTop;
                        break;
                    }
                }
                else if (destinationPos == destinationRoom->positions[1]) {
                    // TopRight
                    switch (door.position)
                    {
                    case Bottom:
                    case BottomLeft:
                    case BottomRight:
                        newDoor.position = TopRight;
                        break;
                    case Left:
                    case LeftTop:
                    case LeftBottom:
                        newDoor.position = RightTop;
                        break;
                    }
                }
                else if (destinationPos == destinationRoom->positions[2]) {
                    // BottomLeft
                    switch (door.position)
                    {
                    case Top:
                    case TopLeft:
                    case TopRight:
                        newDoor.position = BottomLeft;
                        break;
                    case Right:
                    case RightTop:
                    case RightBottom:
                        newDoor.position = LeftBottom;
                        break;
                    }
                }
                else if (destinationPos == destinationRoom->positions[3]) {
                    // BottomRight
                    switch (door.position)
                    {
                    case Top:
                    case TopLeft:
                    case TopRight:
                        newDoor.position = BottomRight;
                        break;
                    case Left:
                    case LeftTop:
                    case LeftBottom:
                        newDoor.position = RightBottom;
                        break;
                    }
                }
            }
            door.destinationPosition = newDoor.position;
            newDoor.destinationPosition = door.position;
            bool exists = false;
            for each (Door d in destinationRoom->doors)
            {
                if (d.position == newDoor.position) {
                    exists = true;
                    d.destinationPosition = door.position;
                }
            }
            if (!exists) {
                std::cout << "Insert reverse door" << std::endl;
                destinationRoom->doors.push_back(newDoor);
            }
        }
    }
}

glm::vec2 LevelBuilder::calculatePosition(std::shared_ptr<RoomSettings> fromRoom, Door door) {

    glm::vec2 pos = fromRoom->positions[0];
    if (fromRoom->roomSize == Medium) {
        switch (door.position)
        {
        case Top:
            pos += glm::vec2(0, 1);
            break;
        case Bottom:
            pos += glm::vec2(0, -1);
            break;
        case Left:
            pos += glm::vec2(-1, 0);
            break;
        case Right:
            pos += glm::vec2(1, 0);
            break;
        }
    }
    else if (fromRoom->roomSize == Wide) {
        switch (door.position)
        {
        case TopLeft:
            pos = fromRoom->positions[0] + glm::vec2(0, 1);
            break;
        case TopRight:
            pos = fromRoom->positions[1] + glm::vec2(0, 1);
            break;
        case BottomLeft:
            pos = fromRoom->positions[0] + glm::vec2(0, -1);
            break;
        case BottomRight:
            pos = fromRoom->positions[1] + glm::vec2(0, -1);
            break;
        case Left:
            pos = fromRoom->positions[0] + glm::vec2(-1, 0);
            break;
        case Right:
            pos = fromRoom->positions[1] + glm::vec2(1, 0);
            break;
        }
    }
    else if (fromRoom->roomSize == Long) {
        switch (door.position)
        {
        case Top:
            pos = fromRoom->positions[0] + glm::vec2(0, 1);
            break;
        case Bottom:
            pos = fromRoom->positions[1] + glm::vec2(0, -1);
            break;
        case LeftTop:
            pos = fromRoom->positions[0] + glm::vec2(-1, 0);
            break;
        case LeftBottom:
            pos = fromRoom->positions[1] + glm::vec2(-1, 0);
            break;
        case RightTop:
            pos = fromRoom->positions[0] + glm::vec2(1, 0);
            break;
        case RightBottom:
            pos = fromRoom->positions[1] + glm::vec2(1, 0);
            break;
        }
    }
    else {
        switch (door.position)
        {
        case TopLeft:
            pos = fromRoom->positions[0] + glm::vec2(0, 1);
            break;
        case TopRight:
            pos = fromRoom->positions[1] + glm::vec2(0, 1);
            break;
        case BottomLeft:
            pos = fromRoom->positions[2] + glm::vec2(0, -1);
            break;
        case BottomRight:
            pos = fromRoom->positions[3] + glm::vec2(0, -1);
            break;
        case LeftTop:
            pos = fromRoom->positions[0] + glm::vec2(-1, 0);
            break;
        case LeftBottom:
            pos = fromRoom->positions[2] + glm::vec2(-1, 0);
            break;
        case RightTop:
            pos = fromRoom->positions[1] + glm::vec2(1, 0);
            break;
        case RightBottom:
            pos = fromRoom->positions[3] + glm::vec2(1, 0);
            break;
        }
    }

    return pos;

    /*
    glm::vec2 pos = fromRoom->position;
    switch (door.position)
    {
        // Top
    case Top:
        pos += glm::vec2(0, 1);
        break;
    case TopLeft:
        if (fromRoom->roomSize == Wide) {
            pos += glm::vec2(0, 1);
        }
        else {
            pos += glm::vec2(0, 2);
        }
        break;
    case TopRight:
        if (fromRoom->roomSize == Wide) {
            pos += glm::vec2(1, 1);
        }
        else {
            pos += glm::vec2(1, 2);
        }
        break;
        // Bottom
    case Bottom:
    case BottomLeft:
        pos += glm::vec2(0, -1);
        break;
    case BottomRight:
        pos += glm::vec2(1, -1);
        break;
        // Left
    case Left:
    case LeftBottom:
        pos += glm::vec2(-1, 0);
        break;
    case LeftTop:
        pos += glm::vec2(-1, 1);
        break;
        // Right
    case Right:
        pos += glm::vec2(1, 0);
        break;
    case RightBottom:
        if (fromRoom->roomSize == Long) {
            pos += glm::vec2(1, 0);
        }
        else {
            pos += glm::vec2(2, 0);
        }
        break;
    case RightTop:
        if (fromRoom->roomSize == Long) {
            pos += glm::vec2(1, 1);
        }
        else {
            pos += glm::vec2(2, 1);
        }
        break;
    }
    */

}