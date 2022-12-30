#pragma once

#include "GameObject.hpp"
#include "RoomBuilder.hpp"
#include "CharacterBuilder.hpp"

class Level {
public:
	std::string name;
	int rooms;
	int difficulty;
	int startRoom;

	std::vector<std::shared_ptr<RoomSettings>> roomSettings;
	std::vector<std::vector<std::shared_ptr<GameObject>>> roomObjects; // roomObjects[i] = list of objects in room i
	std::shared_ptr<GameObject> currentRoom;
	int currentRoomIndex;
	std::vector<bool> roomEntered;

	std::vector<EnemySettings> regularEnemySettings;
	std::vector<EnemySettings> bossEnemySettings;

	std::shared_ptr<GameObject> player;

	void loadLevel();

	void loadRoom(int room, DoorPosition enteredAt);

	void unloadLevel();

	std::shared_ptr<Level> nextLevel = nullptr;
	bool finished = false;
private:
	std::shared_ptr<GameObject> createPortal(glm::vec2 position);

};