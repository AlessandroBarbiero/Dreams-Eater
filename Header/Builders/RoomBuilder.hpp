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
	DoorPosition destinationPosition;
};

struct RoomSettings {
	std::string name = "room";
	int id;
	glm::vec2 position;
	/*
	* If size = Wide -> Positions[0] = Left and Positions[1] = Right
	* If size = Long -> Positions[0] = Top and Positions[1] = Bottom
	* If size = Large -> Positions[0] = TopLeft, Positions[1] = TopRight, Positions[2] = BottomLeft, Positions[3] = BottomRight
	*/
	glm::vec2 positions[4];
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