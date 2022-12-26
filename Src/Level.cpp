#include "Level.hpp"
#include <iostream>
#include "RoomComponent.hpp"
#include "DreamGame.hpp"
#include "CharacterBuilder.hpp"
#include "PhysicsComponent.hpp"
#include <Builders/PowerupBuilder.hpp>


void Level::loadLevel() {
}

void Level::loadRoom(int room, DoorPosition enteredAt) {
	if (currentRoom != nullptr) {
		// Save currentRoom
		std::cout << "Saving room" << std::endl;
		/*
		*/
		roomObjects[currentRoomIndex] = currentRoom->getComponent<RoomComponent>()->roomObjects;
		for (auto go : roomObjects[currentRoomIndex]) {
			//std::cout << "Destroying " << go->name << std::endl;
			go->destroy();
			auto phys = go->getComponent<PhysicsComponent>();
			if (phys != nullptr) {
				auto shape = phys->fixture->GetShape();
				//phys->lastShape = shape->c
				/*
				if (phys->shapeType == b2Shape::e_circle) {
					phys->circle = dynamic_cast<b2CircleShape*>(shape);
				}
				else if (phys->shapeType == b2Shape::e_polygon) {
					phys->polygon = dynamic_cast<b2PolygonShape*>(shape);
				}
				*/
				//DreamGame::instance->deregisterPhysicsComponent(phys.get());
				//DreamGame::instance->world->DestroyBody(phys->getBody());
				//phys->~PhysicsComponent();
				phys->pause();
				if (phys->getBody() != nullptr) {
					//std::cout << "Body still exists" << std::endl;
				}
			}
		}
		auto sceneObjects = DreamGame::instance->currentScene->getSceneObjects();
		std::for_each(sceneObjects->begin(), sceneObjects->end(), [](std::shared_ptr<GameObject> go) {if (go->tag == Tag::EnemyBullet || go->tag == Tag::PlayerBullet) go->destroy();});

		currentRoom->destroy();
	}

	// Create room
	auto obj = RoomBuilder::createRoom(*roomSettings[room]);
	auto newRoom = obj->getComponent<RoomComponent>();
	currentRoom = obj;
	currentRoomIndex = room;
	//newRoom->level = this;

	if (roomEntered[room]) {
		// Reactivate saved objects
		newRoom->roomObjects = roomObjects[room];

		for (auto go : newRoom->roomObjects) {
			DreamGame::instance->reactivateGameObject(go);
		}
	}
	else {
		// Create room contents
		auto roomSize = newRoom->getRoomSizeInPixels() / DreamGame::instance->physicsScale;
		std::cout << "roomSize: (" << roomSize.x << ", " << roomSize.y << ")" << std::endl;
		int random = 0;
		PowerupBuilder* pBuilder = PowerupBuilder::getInstance();
		EnemySettings eSettings;
		switch (roomSettings[room]->roomType) {
		case SpawnRoom:
			break;
		case EnemyRoom:
			random = (rand() % 3) + 1;
			for (int i = 0; i < random; i++) {
				eSettings.name = "Enemy" + std::to_string(i);
				eSettings.position = glm::vec2(rand() % (int)(roomSize.x - 2), rand() % (int)(roomSize.y - 2)) - glm::vec2(roomSize.x-2, roomSize.y-2)/2.0f;
				eSettings.player = player;
				eSettings.speed = 2.0f;
				eSettings.knockback = 1.0f;
				auto enemy = CharacterBuilder::createEnemy(eSettings);
				newRoom->roomObjects.push_back(enemy);
			}
			break;
		case PowerUpRoom:
			random = rand() % 4;
			newRoom->roomObjects.push_back(pBuilder->createSinglePowerupObject(static_cast<PowerupType>(random), { 5,5 }));
			break;
		case BossRoom:
			eSettings.name = "Boss";
			eSettings.type = CharacterType::FireWizard;
			eSettings.position = glm::vec2(roomSize.x / 2, roomSize.y / 2) - roomSize/2.0f;
			eSettings.player = player;
			eSettings.speed = 3.0f;
			eSettings.knockback = 10.0f;
			eSettings.damage = 1.0f;
			eSettings.hp = 25;
			eSettings.rateOfFire = 9.0f;
			eSettings.range = 100;
			eSettings.shotSpeed = 10;
			auto enemy = CharacterBuilder::createEnemy(eSettings);
			newRoom->roomObjects.push_back(enemy);
			break;
		}

	}
	roomEntered[room] = true;

	// TODO: Calculate player door entry
	auto phys = player->getComponent<PhysicsComponent>();
	auto enterPos = newRoom->doorEntrances[enteredAt];
	enterPos = enterPos / DreamGame::instance->physicsScale;
	phys->getBody()->SetTransform({enterPos.x, enterPos.y} , 0);
	//auto roomSize = newRoom->getRoomSizeInPixels() / DreamGame::instance->physicsScale;
	//phys->getBody()->SetTransform({ roomSize.x / 4 - roomSize.x / 2.0f, roomSize.y / 4 - roomSize.y / 2.0f }, 0) ;

}