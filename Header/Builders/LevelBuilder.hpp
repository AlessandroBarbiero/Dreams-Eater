#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include "Level.hpp"
#include <deque>

struct LevelSettings {
	std::string name = "room";
	int rooms;
	int difficulty;


};

class LevelBuilder {
public:
	static std::shared_ptr<Level> createLevel(LevelSettings settings);

private:
	static void generateLayout(std::shared_ptr<Level> level);
	static std::shared_ptr<RoomSettings> generateRoom(std::shared_ptr<Level> level, std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> &roomMap, int roomsLeft, int roomId, int roomEnteredFrom, glm::vec2 position, bool spawn = false);
	
	static std::vector<Door> generateDoors(std::shared_ptr<RoomSettings> roomSettings, std::map<DoorPosition, bool>& chosen, int doors);
	static void insertRoomInMap(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> &map, std::shared_ptr<RoomSettings> room);
	static std::vector<std::pair<int, DoorPosition>> getNeighbours(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> &roomMap, glm::vec2(&positions)[4], RoomSize size);
	static glm::vec2 calculatePosition(std::shared_ptr<RoomSettings> fromRoom, Door door);
	static RoomSize expandRoom(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>> &roomMap, glm::vec2 pos, RoomSize roomSize, glm::vec2(&positions)[4]);
	static void fixDoors(std::shared_ptr<Level> level);
	static void forceNewRoom(std::shared_ptr<Level> level, std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>>& roomMap, std::deque<std::pair<int, Door>>& roomsToGenerate, int& roomsLeft, int& nextRoomId);
	static void prepareNewRoom(std::map<std::pair<int, int>, std::shared_ptr<RoomSettings>>& roomMap, std::deque<std::pair<int, Door>>& roomsToGenerate, int& roomsLeft, int& nextRoomId, std::shared_ptr<RoomSettings> r, Door& d);
};