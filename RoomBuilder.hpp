#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"

enum TileSetWalls {
	WoodWalls,
	BricksWalls,
	LightWoodWalls,
	StoneWalls,
	ShogiWalls
};

// Insert floor names
enum TileSetFloor {
	LightWoodFloor,
	WoodFloor,
	ShogiFloor,
	StoneFloor,
	BricksFloor
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

struct Door {
	bool locked = false;
	DoorPosition position;
	// TODO: Add destination
};

struct RoomSettings {
	std::string name = "room";
	glm::vec2 position;
	glm::vec2 size;

	TileSetWalls tileSetWalls;
	TileSetFloor tileSetFloor;
	std::vector<Door> doors;
};

class RoomBuilder {
public:
	static std::shared_ptr<GameObject> createRoom(RoomSettings settings);

private:
};