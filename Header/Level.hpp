#pragma once

#include "GameObject.hpp"
#include "RoomBuilder.hpp"

class Level {
public:

	std::string name;
	int rooms;
	int difficulty;
	int startRoom;

	std::vector<RoomSettings> roomSettings;
	std::vector<std::vector<std::shared_ptr<GameObject>>> roomObjects; // roomObjects[i] = list of objects in room i
	std::shared_ptr<GameObject> currentRoom;
	int currentRoomIndex;
	std::vector<bool> roomEntered;

	std::shared_ptr<GameObject> player;

	void loadLevel();

	void loadRoom(int room);

private:

};