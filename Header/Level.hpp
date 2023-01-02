#pragma once

#include "GameObject.hpp"
#include "RoomBuilder.hpp"
#include "CharacterBuilder.hpp"
#include "sre/SpriteAtlas.hpp"

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
	std::shared_ptr<GameObject> boss;

	/*
		Loads enemy data, then loads the spawnRoom.
	*/
	void loadLevel();

	/*
		Saves the objects in the current room, then loads the room and places the player near the specified DoorPosition,
		or in the center of the room if enterAtCenter is true.
	*/
	void loadRoom(int room, DoorPosition enteredAt, bool enterAtCenter = false);

	void unloadLevel();

	std::shared_ptr<Level> nextLevel = nullptr;
	bool finished = false;
private:
	std::shared_ptr<GameObject> createPortal(glm::vec2 position);

	// Used for portal to next level
	static std::shared_ptr<sre::SpriteAtlas> getPurpleWraithAtlas() {
		static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_purpleWraith = sre::SpriteAtlas::create("Sprites/Characters/PurpleWraith/PurpleWraith_atlas.json", "Sprites/Characters/PurpleWraith/PurpleWraith_atlas.png");
		return spriteAtlas_purpleWraith;
	}

};