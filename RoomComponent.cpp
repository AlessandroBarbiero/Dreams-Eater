//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "RoomComponent.hpp"
#include "GameObject.hpp"
#include "DreamGame.hpp"
#include <Box2D/Box2D.h>
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

RoomComponent::RoomComponent(GameObject *gameObject) : Component(gameObject) {}

RoomComponent::~RoomComponent() {
	std::cout << "Room Destroyed" << std::endl;
	/*
	for each (std::shared_ptr<GameObject> obj in roomObjects)
	{
		obj->destroy();
	}
	roomObjects.clear();
	*/
}

glm::vec2 RoomComponent::getRoomSize() {
	return roomSize;
}

void RoomComponent::DoorsToPositions(std::vector<Door> doors, int (&skips)[4][2]) {

	int* skipTop = skips[0];
	int* skipBottom = skips[1];
	int* skipLeft = skips[2];
	int* skipRight = skips[3];
	for each (Door d in doors)
	{
		switch (d.position) {
		case Top:
			if (skipTop[0] == 0) {
				skipTop[0] = std::ceil((roomSize.x - 2) / 2.0f);
			}
			else {
				skipTop[1] = std::ceil((roomSize.x - 2) / 2.0f);
			}
			break;
		case TopLeft:
			if (skipTop[0] == 0) {
				skipTop[0] = std::ceil((roomSize.x - 2) / 4.0f);
			}
			else {
				skipTop[1] = std::ceil((roomSize.x - 2) / 4.0f);
			}
			break;
		case TopRight:
			if (skipTop[0] == 0) {
				skipTop[0] = std::ceil((roomSize.x - 2) * 0.75f);
			}
			else {
				skipTop[1] = std::ceil((roomSize.x - 2) * 0.75f);
			}
			break;
		case Bottom:
			if (skipBottom[0] == 0) {
				skipBottom[0] = std::ceil((roomSize.x - 2) / 2.0f);
			}
			else {
				skipBottom[1] = std::ceil((roomSize.x - 2) / 2.0f);
			}
			break;
		case BottomLeft:
			if (skipBottom[0] == 0) {
				skipBottom[0] = std::ceil((roomSize.x - 2) / 4.0f);
			}
			else {
				skipBottom[1] = std::ceil((roomSize.x - 2) / 4.0f);
			}
			break;
		case BottomRight:
			if (skipBottom[0] == 0) {
				skipBottom[0] = std::ceil((roomSize.x - 2) * 0.75f);
			}
			else {
				skipBottom[1] = std::ceil((roomSize.x - 2) * 0.75f);
			}
			break;
		case Left:
			if (skipLeft[0] == 0) {
				skipLeft[0] = std::ceil((roomSize.y - 2) / 2.0f);
			}
			else {
				skipLeft[1] = std::ceil((roomSize.y - 2) / 2.0f);
			}
			break;
		case LeftBottom:
			if (skipLeft[0] == 0) {
				skipLeft[0] = std::ceil((roomSize.y - 2) / 4.0f);
			}
			else {
				skipLeft[1] = std::ceil((roomSize.y - 2) / 4.0f);
			}
			break;
		case LeftTop:
			if (skipLeft[0] == 0) {
				skipLeft[0] = std::ceil((roomSize.y - 2) * 0.75f);
			}
			else {
				skipLeft[1] = std::ceil((roomSize.y - 2) * 0.75f);
			}
			break;
		case Right:
			if (skipRight[0] == 0) {
				skipRight[0] = std::ceil((roomSize.y - 2) / 2.0f);
			}
			else {
				skipRight[1] = std::ceil((roomSize.y - 2) / 2.0f);
			}
			break;
		case RightBottom:
			if (skipRight[0] == 0) {
				skipRight[0] = std::ceil((roomSize.y - 2) / 4.0f);
			}
			else {
				skipRight[1] = std::ceil((roomSize.y - 2) / 4.0f);
			}
			break;
		case RightTop:
			if (skipRight[0] == 0) {
				skipRight[0] = std::ceil((roomSize.y - 2) * 0.75f);
			}
			else {
				skipRight[1] = std::ceil((roomSize.y - 2) * 0.75f);
			}
			break;
		}
	}

}

glm::vec2 RoomComponent::getRoomSizeInPixels() {
	std::string wallString = TileSetWallsToString.at(tileSetWalls);
	//auto spriteWallHorizontalBottom = DreamGame::instance->spriteAtlas_inside->get(wallString + "Bottom.png");
	auto spriteWallHorizontalBottom = DreamGame::instance->spriteAtlas_inside->get("Walls/walls_0005_Layer-6.png");
	float length = spriteWallHorizontalBottom.getSpriteSize().x;
	return roomSize * length;
}

// RoomSize is number of segments excluding corners
void RoomComponent::setRoomSize(glm::vec2 newSize) {
	roomSize = newSize + glm::vec2(2, 2);
}

const std::map<TileSetWalls, std::string> RoomComponent::TileSetWallsToString {
		{TileSetWalls::WoodWalls, "Walls/Wood/"},
};

const std::map<TileSetFloor, std::string> RoomComponent::TileSetFloorToString{
		{TileSetFloor::WoodFloor, "Floor/Floor1"},
};

void RoomComponent::buildWalls() {
	auto go = getGameObject();
	auto game = DreamGame::instance;

	/*
	*/
	auto spriteWallHorizontalBottom = game->spriteAtlas_inside->get("Walls/walls_0005_Layer-6.png");
	auto spriteWallHorizontalTop = game->spriteAtlas_inside->get("Walls/walls_0006_Layer-7.png");

	auto spriteWallVerticalLeft = game->spriteAtlas_inside->get("Walls/walls_0003_Layer-4.png");
	auto spriteWallVerticalRight = game->spriteAtlas_inside->get("Walls/walls_0008_Layer-9.png");

	auto spriteWallTopLeft = game->spriteAtlas_inside->get("Walls/walls_0002_Layer-3.png");
	auto spriteWallTopRight = game->spriteAtlas_inside->get("Walls/walls_0007_Layer-8.png");
	auto spriteWallBottomLeft = game->spriteAtlas_inside->get("Walls/walls_0004_Layer-5.png");
	auto spriteWallBottomRight = game->spriteAtlas_inside->get("Walls/walls_0009_Layer-10.png");

	/*
	std::string wallString = TileSetWallsToString.at(tileSetWalls);

	auto spriteWallHorizontalBottom = game->spriteAtlas_inside->get(wallString + "Bottom.png");
	auto spriteWallHorizontalTop = game->spriteAtlas_inside->get(wallString + "Top.png");

	auto spriteWallVerticalLeft = game->spriteAtlas_inside->get(wallString + "Left.png");
	auto spriteWallVerticalRight = game->spriteAtlas_inside->get(wallString + "Right.png");

	auto spriteWallTopLeft = game->spriteAtlas_inside->get(wallString + "TopLeft.png");
	auto spriteWallTopRight = game->spriteAtlas_inside->get(wallString + "TopRight.png");
	auto spriteWallBottomLeft = game->spriteAtlas_inside->get(wallString + "BottomLeft.png");
	auto spriteWallBottomRight = game->spriteAtlas_inside->get(wallString + "BottomRight.png");
	*/

	// Offset between center of corners and straight walls
	// corner center is 170.5, horizontal center is 49.5, 170.5 - 49.5 = 121 offset
	int offset = (spriteWallBottomLeft.getSpriteSize().y / 2) - (spriteWallHorizontalBottom.getSpriteSize().y / 2);
	
	//std::cout << offset << std::endl;

	// Bottom Left corner
	auto position = glm::vec2(0, 0);
	go->children.push_back(spawnWall(spriteWallBottomLeft, glm::vec2(0,0)));

	// Bottom Right corner
	position = glm::vec2((roomSize.x - 1) * spriteWallHorizontalBottom.getSpriteSize().x, 0);
	go->children.push_back(spawnWall(spriteWallBottomRight, position));

	// Top Left corner
	position = glm::vec2(0, (roomSize.y - 1) * spriteWallVerticalLeft.getSpriteSize().y);
	go->children.push_back(spawnWall(spriteWallTopLeft, position));

	// Top Right corner
	position = glm::vec2((roomSize.x - 1) * spriteWallHorizontalBottom.getSpriteSize().x, (roomSize.y - 1) * spriteWallVerticalLeft.getSpriteSize().y);
	go->children.push_back(spawnWall(spriteWallTopRight, position));


	int skips[4][2] = { {0,0}, {0,0}, {0,0}, {0,0} };
	DoorsToPositions(doors, skips);

	int* skipTop = skips[0];
	int* skipBottom = skips[1];
	int* skipLeft = skips[2];
	int* skipRight = skips[3];

	// Horizontal walls
	for (int x = 1; x < roomSize.x-1; x++)
	{
		// Bottom Wall
		if (x != skipBottom[0] && x != skipBottom[1]) {
			auto position = glm::vec2(x * spriteWallHorizontalBottom.getSpriteSize().x, -offset);
			go->children.push_back(spawnWall(spriteWallHorizontalBottom, position));
		}

		// Top Wall
		int y = roomSize.y - 1;

		if (x != skipTop[0] && x != skipTop[1]) {
			position = glm::vec2(x * spriteWallHorizontalBottom.getSpriteSize().x, y * spriteWallVerticalLeft.getSpriteSize().y + offset);
			go->children.push_back(spawnWall(spriteWallHorizontalTop, position));
		}
	}
	
	// Vertical walls. Starts at 1 and ends at max-1 because they already exist from the previous loop
	for (int y = 1; y < roomSize.y-1; y++)
	{
		// Left Wall
		if (y != skipLeft[0] && y != skipLeft[1]) {
			auto position = glm::vec2(-offset, y * spriteWallVerticalLeft.getSpriteSize().y);
			go->children.push_back(spawnWall(spriteWallVerticalLeft, position));
		}

		// Right Wall
		int x = roomSize.x - 1;
		if (y != skipRight[0] && y != skipRight[1]) {
			position = glm::vec2(x * spriteWallHorizontalBottom.getSpriteSize().x + offset, y * spriteWallVerticalLeft.getSpriteSize().y);
			go->children.push_back(spawnWall(spriteWallVerticalRight, position));
		}
	}

	getGameObject()->setPosition(glm::vec2( (roomSize.x-1)* spriteWallHorizontalBottom.getSpriteSize().x / 2, (roomSize.y-1)* spriteWallVerticalLeft.getSpriteSize().y / 2));
	// Collision
	auto size = glm::vec2(getRoomSizeInPixels().x - 198, getRoomSizeInPixels().y - 198) / game->physicsScale;
	auto center = getGameObject()->getPosition() / game->physicsScale;
	b2Vec2 points[4];
	glm::vec2 bottomLeft =	glm::vec2(center.x - (size.x/2), center.y - (size.y/2));
	glm::vec2 bottomRight = glm::vec2(center.x + (size.x/2), center.y - (size.y/2));
	glm::vec2 topRight =	glm::vec2(center.x + (size.x/2), center.y + (size.y/2));
	glm::vec2 topLeft =		glm::vec2(center.x - (size.x/2), center.y + (size.y/2));

	points[0].Set(bottomLeft.x, bottomLeft.y); // bottom left
	points[1].Set(bottomRight.x, bottomRight.y); // bottom right
	points[2].Set(topRight.x, topRight.y); // top right
	points[3].Set(topLeft.x, topLeft.y); // top left

	getGameObject()->getComponent<PhysicsComponent>()->initChain(b2_staticBody, points, 4, center / game->physicsScale, 1);

	game->camera->getCamera().setOrthographicProjection((roomSize.x) * spriteWallHorizontalBottom.getSpriteSize().x/2, -1, 1); // Fit room width to window
}

void RoomComponent::buildFloor() {
	auto go = getGameObject();
	auto game = DreamGame::instance;

	std::string floorString = TileSetFloorToString.at(tileSetFloor);
	auto spriteFloor = game->spriteAtlas_inside->get(floorString + ".png");

	//roomObjects.push_back(spawnFloor(spriteFloor, (roomSize.x-1)/2, (roomSize.y-1)/2));

	auto horizontal = roomSize.x * 341;
	auto vertical = roomSize.y * 341;

	auto floorSize = glm::vec2(horizontal / spriteFloor.getSpriteSize().x, vertical / spriteFloor.getSpriteSize().y);
	
	for (int x = 0; x < floorSize.x; x++) {
		for (int y = 0; y < floorSize.y; y++) {
			auto position = glm::vec2(x * spriteFloor.getSpriteSize().x - 170.5f, y * spriteFloor.getSpriteSize().y - 170.5f);
			go->children.push_back(spawnFloor(spriteFloor, position));
		}
	}
}

std::shared_ptr<GameObject> RoomComponent::spawnFloor(sre::Sprite spriteFloor, glm::vec2 pos) {
	auto game = DreamGame::instance;

	auto floor = game->createGameObject();

	floor->name = "Floor";
	auto sprite = floor->addComponent<SpriteComponent>();

	floor->setPosition(pos);
	sprite->setSprite(spriteFloor);

	glm::vec2 s{ spriteFloor.getSpriteSize().x * spriteFloor.getScale().x / 2, spriteFloor.getSpriteSize().y * spriteFloor.getScale().y / 2 };

	return floor;
}

std::shared_ptr<GameObject> RoomComponent::spawnWall(sre::Sprite spriteWall, glm::vec2 pos) {
	auto game = DreamGame::instance;

	auto wall = game->createGameObject();
	
	wall->name = "Wall";
	auto sprite = wall->addComponent<SpriteComponent>();

	wall->setPosition(pos);
	sprite->setSprite(spriteWall);

	//glm::vec2 s{ spriteWall.getSpriteSize().x * spriteWall.getScale().x / 2, spriteWall.getSpriteSize().y * spriteWall.getScale().y / 2 };

	//auto phys = wall->addComponent<PhysicsComponent>();

	//phys->initBox(b2_staticBody, s / game->physicsScale, { wall->getPosition().x / game->physicsScale, wall->getPosition().y / game->physicsScale }, 1);
	
	return wall;
}