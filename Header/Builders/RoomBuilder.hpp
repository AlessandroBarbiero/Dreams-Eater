#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"

enum TileSetWalls {
	WoodWalls,
	LightWoodWalls,
	BricksWalls,
	StoneWalls,
	ShogiWalls
};

// Insert floor names
enum TileSetFloor {
	WoodFloor,
	LightWoodFloor,
	BricksFloor,
	StoneFloor,
	ShogiFloor
};

enum DoorPosition {
	Top,
	TopLeft,
	TopRight,
	Bottom,
	BottomLeft,
	BottomRight,
	Left,
	LeftTop,
	LeftBottom,
	Right,
	RightTop,
	RightBottom
};

enum RoomType {
	EnemyRoom,
	PowerUpRoom,
	BossRoom,
	SpawnRoom
};

enum RoomSize {
	Medium,
	Wide,
	Long,
	Large
};

struct Door {
	bool locked = false;
	DoorPosition position;
	int destinationRoom;
};

struct RoomSettings {
	std::string name = "room";
	glm::vec2 position;
	glm::vec2 size;

	TileSetWalls tileSetWalls;
	TileSetFloor tileSetFloor;
	std::vector<Door> doors;

	RoomType roomType;
	RoomSize roomSize;
};

class RoomBuilder {
public:
	static std::shared_ptr<GameObject> createRoom(RoomSettings settings);

private:
};