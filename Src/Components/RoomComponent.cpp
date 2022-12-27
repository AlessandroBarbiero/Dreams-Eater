//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "RoomComponent.hpp"
#include "GameObject.hpp"
#include "DreamGame.hpp"
#include <Box2D/Box2D.h>
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "DoorComponent.hpp"
#include "GuiHelper.hpp";

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
	auto spriteWallHorizontalBottom = DreamGame::instance->spriteAtlas_inside->get(wallString + "Bottom.png");
	//auto spriteWallHorizontalBottom = DreamGame::instance->spriteAtlas_inside->get("Walls/walls_0005_Layer-6.png");
	float length = spriteWallHorizontalBottom.getSpriteSize().x;
	return roomSize * length;
}

// RoomSize is number of segments excluding corners
void RoomComponent::setRoomSize(glm::vec2 newSize) {
	roomSize = newSize + glm::vec2(2, 2);
}

const std::map<TileSetWalls, std::string> RoomComponent::TileSetWallsToString {
		{TileSetWalls::WoodWalls, "Walls/Wood/"},
		{TileSetWalls::BricksWalls, "Walls/Bricks/"},
		{TileSetWalls::LightWoodWalls, "Walls/LightWood/"},
		{TileSetWalls::StoneWalls, "Walls/Stone/"},
		{TileSetWalls::ShogiWalls, "Walls/Shogi/"}
};

const std::map<TileSetFloor, std::string> RoomComponent::TileSetFloorToString{
		{TileSetFloor::WoodFloor, "Floor/Floor5"},
		{TileSetFloor::LightWoodFloor, "Floor/Floor1"},
		{TileSetFloor::ShogiFloor, "Floor/Floor3"},
		{TileSetFloor::StoneFloor, "Floor/Floor4"},
		{TileSetFloor::BricksFloor, "Floor/Floor2"},
};

void RoomComponent::buildWalls() {
	auto go = getGameObject();
	auto game = DreamGame::instance;

	/*
	auto spriteWallHorizontalBottom = game->spriteAtlas_inside->get("Walls/walls_0005_Layer-6.png");
	auto spriteWallHorizontalTop = game->spriteAtlas_inside->get("Walls/walls_0006_Layer-7.png");

	auto spriteWallVerticalLeft = game->spriteAtlas_inside->get("Walls/walls_0003_Layer-4.png");
	auto spriteWallVerticalRight = game->spriteAtlas_inside->get("Walls/walls_0008_Layer-9.png");

	auto spriteWallTopLeft = game->spriteAtlas_inside->get("Walls/walls_0002_Layer-3.png");
	auto spriteWallTopRight = game->spriteAtlas_inside->get("Walls/walls_0007_Layer-8.png");
	auto spriteWallBottomLeft = game->spriteAtlas_inside->get("Walls/walls_0004_Layer-5.png");
	auto spriteWallBottomRight = game->spriteAtlas_inside->get("Walls/walls_0009_Layer-10.png");
	*/

	
	std::string wallString = TileSetWallsToString.at(tileSetWalls);

	auto spriteWallHorizontalBottom = game->spriteAtlas_inside->get(wallString + "Bottom.png");
	auto spriteWallHorizontalTop = game->spriteAtlas_inside->get(wallString + "Top.png");

	auto spriteWallVerticalLeft = game->spriteAtlas_inside->get(wallString + "Left.png");
	auto spriteWallVerticalRight = game->spriteAtlas_inside->get(wallString + "Right.png");

	auto spriteWallTopLeft = game->spriteAtlas_inside->get(wallString + "TopLeft.png");
	auto spriteWallTopRight = game->spriteAtlas_inside->get(wallString + "TopRight.png");
	auto spriteWallBottomLeft = game->spriteAtlas_inside->get(wallString + "BottomLeft.png");
	auto spriteWallBottomRight = game->spriteAtlas_inside->get(wallString + "BottomRight.png");

	auto spriteDoor = game->spriteAtlas_baseWraith->get("Spells-Effect.png");
	
	// Offsets are used for positioning
	int wallWidth = spriteWallHorizontalBottom.getSpriteSize().y;
	int offsetWidth = wallWidth / 2.0f;
	int wallLength = spriteWallHorizontalBottom.getSpriteSize().x;
	int offsetLength = wallLength / 2.0f;
	//std::cout << "BottomWallLength: " << spriteWallHorizontalBottom.getSpriteSize().x << std::endl;
	//std::cout << "TopWallLength: " << spriteWallHorizontalTop.getSpriteSize().x << std::endl;
	//std::cout << "LeftWallLength: " << spriteWallVerticalLeft.getSpriteSize().y << std::endl;
	//std::cout << "RightWallLength: " << spriteWallVerticalRight.getSpriteSize().y << std::endl;

	// Calculate dimensions
	auto roomSizePixels = getRoomSizeInPixels();
	glm::vec2 bottomLeft = { -(roomSizePixels.x / 2.0f), -(roomSizePixels.y / 2.0f) };
	glm::vec2 bottomRight = { roomSizePixels.x / 2.0f, -(roomSizePixels.y / 2.0f) };
	glm::vec2 topRight = { roomSizePixels.x / 2.0f, roomSizePixels.y / 2.0f };
	glm::vec2 topLeft = { -(roomSizePixels.x / 2.0f), roomSizePixels.y / 2.0f };
	

	// Bottom Left corner
	auto position = bottomLeft + glm::vec2(spriteWallBottomLeft.getSpriteSize().x / 2.0f, spriteWallBottomLeft.getSpriteSize().y / 2.0f);
	go->addChild(spawnWall(spriteWallBottomLeft, position).get());

	// Bottom Right corner
	position = bottomRight + glm::vec2(-spriteWallBottomRight.getSpriteSize().x / 2.0f, spriteWallBottomRight.getSpriteSize().y / 2.0f);
	go->addChild(spawnWall(spriteWallBottomRight, position).get());

	// Top Left corner
	position = topLeft + glm::vec2(spriteWallTopLeft.getSpriteSize().x / 2.0f, -spriteWallTopLeft.getSpriteSize().y / 2.0f);
	go->addChild(spawnWall(spriteWallTopLeft, position).get());

	// Top Right corner
	position = topRight + glm::vec2(-spriteWallTopRight.getSpriteSize().x / 2.0f, -spriteWallTopRight.getSpriteSize().y / 2.0f);
	go->addChild(spawnWall(spriteWallTopRight, position).get());


	int skips[4][2] = { {0,0}, {0,0}, {0,0}, {0,0} };
	DoorsToPositions(doors, skips);

	int* skipTop = skips[0];
	int* skipBottom = skips[1];
	int* skipLeft = skips[2];
	int* skipRight = skips[3];

	// All walls use HorizontalBottom length because it is 1 pixel longer. Otherwise they leave a gap.

	// Horizontal walls
	for (int x = 1; x < roomSize.x-1; x++)
	{
		// Bottom Wall
		if (x != skipBottom[0] && x != skipBottom[1]) {
			position = bottomLeft + glm::vec2(x * wallLength + offsetLength, offsetWidth);
			go->addChild(spawnWall(spriteWallHorizontalBottom, position).get());
		}

		// Top Wall
		int y = roomSize.y - 1;

		if (x != skipTop[0] && x != skipTop[1]) {
			position = topLeft + glm::vec2(x * wallLength + offsetLength, -offsetWidth);
			go->addChild(spawnWall(spriteWallHorizontalTop, position).get());
		}
	}
	
	// Vertical walls. Starts at 1 and ends at max-1 because they already exist from the previous loop
	for (int y = 1; y < roomSize.y-1; y++)
	{
		// Left Wall
		if (y != skipLeft[0] && y != skipLeft[1]) {
			position = bottomLeft + glm::vec2(offsetWidth, y * wallLength + offsetLength);
			go->addChild(spawnWall(spriteWallVerticalLeft, position).get());
		}

		// Right Wall
		int x = roomSize.x - 1;
		if (y != skipRight[0] && y != skipRight[1]) {
			position = bottomRight + glm::vec2(-offsetWidth, y * wallLength + offsetLength);
			go->addChild(spawnWall(spriteWallVerticalRight, position).get());
		}
	}

	int tblr[4] = { 0,0,0,0 };
	glm::vec2 entryPosition = {0, 0};
	for (auto &d : doors) {
		switch (d.position) {
			case Top:
			case TopLeft:
			case TopRight:
				position = topLeft + glm::vec2(skipTop[tblr[0]] * wallLength + offsetLength, -offsetWidth);
				entryPosition = position + glm::vec2(0, -wallLength * 2);
				tblr[0]++;
				break;
			case Bottom:
			case BottomLeft:
			case BottomRight:
				position = bottomLeft + glm::vec2(skipBottom[tblr[1]] * wallLength + offsetLength, offsetWidth);
				entryPosition = position + glm::vec2(0, wallLength * 2);
				tblr[1]++;
				break;
			case Left:
			case LeftTop:
			case LeftBottom:
				position = bottomLeft + glm::vec2(offsetWidth, skipLeft[tblr[2]] * wallLength + offsetLength);
				entryPosition = position + glm::vec2(wallLength * 2, 0);
				tblr[2]++;
				break;
			case Right:
			case RightTop:
			case RightBottom:
				position = bottomRight + glm::vec2(-offsetWidth, skipRight[tblr[3]] * wallLength + offsetLength);
				entryPosition = position + glm::vec2(-wallLength * 2, 0);
				tblr[3]++;
				break;
		}
		auto spawnedDoor = spawnDoor(spriteDoor, position, d).get();
		go->addChild(spawnedDoor);
		doorEntrances[d.position] = entryPosition;
	}

	//getGameObject()->setPosition(glm::vec2( (roomSize.x-1)* spriteWallHorizontalBottom.getSpriteSize().x / 2, (roomSize.y-1)* spriteWallVerticalLeft.getSpriteSize().y / 2));
	// Collision
	auto size = glm::vec2(getRoomSizeInPixels().x - (2 * wallWidth), getRoomSizeInPixels().y - (2 * wallWidth)) / game->physicsScale;
	auto center = getGameObject()->getPosition() / game->physicsScale;
	b2Vec2 points[4];
	bottomLeft = glm::vec2(center.x - (size.x / 2), center.y - (size.y / 2));
	bottomRight = glm::vec2(center.x + (size.x / 2), center.y - (size.y / 2));
	topRight = glm::vec2(center.x + (size.x / 2), center.y + (size.y / 2));
	topLeft = glm::vec2(center.x - (size.x / 2), center.y + (size.y / 2));

	points[0].Set(bottomLeft.x, bottomLeft.y); // bottom left
	points[1].Set(bottomRight.x, bottomRight.y); // bottom right
	points[2].Set(topRight.x, topRight.y); // top right
	points[3].Set(topLeft.x, topLeft.y); // top left

	getGameObject()->getComponent<PhysicsComponent>()->initChain(b2_staticBody, points, 4, center / game->physicsScale, 1);
}

void RoomComponent::buildFloor() {
	auto go = getGameObject();
	auto game = DreamGame::instance;

	std::string floorString = TileSetFloorToString.at(tileSetFloor);
	auto spriteFloor = game->spriteAtlas_inside->get(floorString + ".png");
	glm::vec2 spriteFloorSize = spriteFloor.getSpriteSize();
	float scale = 0.9*6.0f; // Scaled by 0.9 to fit size of room, then by an even number
	
	
	std::string wallString = TileSetWallsToString.at(tileSetWalls);
	auto spriteWallHorizontalBottom = game->spriteAtlas_inside->get(wallString + "Bottom.png");
	int wallLength = spriteWallHorizontalBottom.getSpriteSize().x;


	auto roomSizePixels = getRoomSizeInPixels();
	glm::vec2 bottomLeft = { -(roomSizePixels.x / 2.0f), -(roomSizePixels.y / 2.0f) };

	// Wall length is 341 pixels per segment
	auto horizontal = roomSize.x * wallLength;
	auto vertical = roomSize.y * wallLength;

	scale = horizontal / (spriteFloorSize.x*6);
	spriteFloorSize *= scale;

	auto floorSize = glm::vec2(horizontal / spriteFloorSize.x, vertical / spriteFloorSize.y);
	
	for (int x = 0; x < floorSize.x; x++) {
		for (int y = 0; y < floorSize.y; y++) {
			auto position = bottomLeft + glm::vec2(x * spriteFloorSize.x + spriteFloorSize.x/2, y * spriteFloorSize.y + spriteFloorSize.y/2);
			go->addChild(spawnFloor(spriteFloor, position, scale).get());
		}
	}
}

std::shared_ptr<GameObject> RoomComponent::spawnFloor(sre::Sprite spriteFloor, glm::vec2 pos, float scale) {
	auto game = DreamGame::instance;

	auto floor = game->currentScene->createGameObject();

	floor->name = "Floor";
	auto sprite = floor->addComponent<SpriteComponent>();

	floor->setPosition(pos);
	floor->setScale(scale);
	sprite->setSprite(spriteFloor);
	

	glm::vec2 s{ spriteFloor.getSpriteSize().x * spriteFloor.getScale().x / 2, spriteFloor.getSpriteSize().y * spriteFloor.getScale().y / 2 };

	return floor;
}

std::shared_ptr<GameObject> RoomComponent::spawnWall(sre::Sprite spriteWall, glm::vec2 pos) {
	auto game = DreamGame::instance;

	auto wall = game->currentScene->createGameObject();
	
	wall->name = "Wall";
	auto sprite = wall->addComponent<SpriteComponent>();

	wall->setPosition(pos);
	sprite->setSprite(spriteWall);

	//glm::vec2 s{ spriteWall.getSpriteSize().x * spriteWall.getScale().x / 2, spriteWall.getSpriteSize().y * spriteWall.getScale().y / 2 };

	//auto phys = wall->addComponent<PhysicsComponent>();

	//phys->initBox(b2_staticBody, s / game->physicsScale, { wall->getPosition().x / game->physicsScale, wall->getPosition().y / game->physicsScale }, 1);
	
	return wall;
}

std::shared_ptr<GameObject> RoomComponent::spawnDoor(sre::Sprite spriteDoor, glm::vec2 pos, Door door) {
	auto game = DreamGame::instance;

	auto go = game->currentScene->createGameObject();

	go->name = "Door";
	auto sprite = go->addComponent<SpriteComponent>();
	
	go->setPosition(pos);
	sprite->setSprite(spriteDoor);
	
	glm::vec2 s{ spriteDoor.getSpriteSize().x * spriteDoor.getScale().x / 2, spriteDoor.getSpriteSize().y * spriteDoor.getScale().y / 2 };

	auto phys = go->addComponent<PhysicsComponent>();

	phys->initBox(b2_staticBody, s / game->physicsScale, { go->getPosition().x / game->physicsScale, go->getPosition().y / game->physicsScale }, 1);

	go->setScale(5);
	auto doorComponent = go->addComponent<DoorComponent>();
	doorComponent->door = door;
	doorComponent->level = level;
	doorComponent->locked = door.locked;
	doorComponent->destinationRoomId = door.destinationRoom;

	if (!door.locked) {
		phys->setSensor(true);
	}
	return go;
}



void RoomComponent::onGui() {

	if (DreamGame::instance->doDebugDraw) {
		auto wallType = TileSetWallsToString.at(tileSetWalls); //get name
		wallType.pop_back(); //remove last character(/)
		bool* open = nullptr;
		ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
		if (ImGui::CollapsingHeader(gameObject->name.c_str())) {
			ImGui::Text("Room size: %.3f, %.3f", roomSize.x, roomSize.y);
			ImGui::Text(wallType.substr(6, -1).insert(0, "Wall type: ").c_str());
		}
		ImGui::End();
	}

}