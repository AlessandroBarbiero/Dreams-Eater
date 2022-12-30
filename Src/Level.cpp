#include "Level.hpp"
#include <iostream>
#include "RoomComponent.hpp"
#include "DreamGame.hpp"
#include "CharacterBuilder.hpp"
#include "PhysicsComponent.hpp"
#include "PortalComponent.hpp"
#include <Builders/PowerupBuilder.hpp>
#include <rapidjson.h>
#include <document.h>
#include <istreamwrapper.h>
#include <fstream>
#include "Components/CharacterComponent.hpp"

void Level::loadLevel() {

	using namespace rapidjson;
	std::ifstream fileStream("data/enemies.json");
	IStreamWrapper isw(fileStream);
	Document d;
	d.ParseStream(isw);

	Value& regularEnemies = d["regular"];
	State state;
	for (auto& m : regularEnemies.GetArray()) {
		EnemySettings enemy;
		auto test = m.GetObject();
		enemy.name = test["name"].GetString();
		enemy.speed = test["speed"].GetFloat();
		enemy.knockback = test["knockback"].GetFloat();
		enemy.damage = test["damage"].GetFloat();
		enemy.hp = test["hp"].GetFloat();
		enemy.rateOfFire = test["rateOfFire"].GetFloat();
		enemy.range = test["range"].GetFloat();
		enemy.shotSpeed = test["shotSpeed"].GetFloat();
		enemy.scale = test["scale"].GetFloat();


		const char* type = test["type"].GetString();
		enemy.type = CharacterStringToType.at(type);


		//auto it = StringToState.find(name);
		//state = it->second;
		regularEnemySettings.push_back(enemy);
	}

	Value& bossEnemies = d["boss"];
	for (auto& m : bossEnemies.GetArray()) {
		EnemySettings enemy;
		auto test = m.GetObject();
		enemy.name = test["name"].GetString();
		enemy.speed = test["speed"].GetFloat();
		enemy.knockback = test["knockback"].GetFloat();
		enemy.damage = test["damage"].GetFloat();
		enemy.hp = test["hp"].GetFloat();
		enemy.rateOfFire = test["rateOfFire"].GetFloat();
		enemy.range = test["range"].GetFloat();
		enemy.shotSpeed = test["shotSpeed"].GetFloat();
		enemy.scale = test["scale"].GetFloat();


		const char* type = test["type"].GetString();
		enemy.type = CharacterStringToType.at(type);


		//auto it = StringToState.find(name);
		//state = it->second;
		bossEnemySettings.push_back(enemy);
	}

	loadRoom(0, Center);
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

		currentRoom->destroy();
	}

	// Destroy all bullets
	auto sceneObjects = DreamGame::instance->currentScene->getSceneObjects();
	std::for_each(sceneObjects->begin(), sceneObjects->end(), [](std::shared_ptr<GameObject> go) {if (go->tag == Tag::EnemyBullet || go->tag == Tag::PlayerBullet) go->destroy();});

	// Create room
	auto obj = RoomBuilder::createRoom(*roomSettings[room]);
	auto newRoom = obj->getComponent<RoomComponent>();
	currentRoom = obj;
	currentRoomIndex = room;
	//newRoom->level = this;

	if (roomEntered[room]) {
		// Reactivate saved objects
		newRoom->roomObjects = roomObjects[room];
		roomObjects[room].clear();
		for (auto go : newRoom->roomObjects) {
			DreamGame::instance->reactivateGameObject(go);
		}
	}
	else {
		// Create room contents
		auto roomSize = newRoom->getRoomSizeInPixels() / DreamGame::instance->physicsScale;
		std::cout << "roomSize: (" << roomSize.x << ", " << roomSize.y << ")" << std::endl;
		int random = 0;
		int roomSizeMultiplier = 1;
		switch (roomSettings[room]->roomSize)
		{
		case Medium:
			roomSizeMultiplier = 1;
			break;
		case Wide:
		case Long:
			roomSizeMultiplier = 2;
			break;
		case Large:
			roomSizeMultiplier = 3;
			break;
		}
		PowerupBuilder* pBuilder = PowerupBuilder::getInstance();
		EnemySettings eSettings;
		switch (roomSettings[room]->roomType) {
		case SpawnRoom:
			break;
		case EnemyRoom:
			random = (rand() % 3) + 1;
			random *= roomSizeMultiplier;
			for (int i = 0; i < random; i++) {
				int randomEnemy = rand() % regularEnemySettings.size();
				eSettings = regularEnemySettings[randomEnemy];
				eSettings.name = eSettings.name + std::to_string(i);
				eSettings.position = glm::vec2(rand() % (int)(roomSize.x - 2), rand() % (int)(roomSize.y - 2)) - glm::vec2(roomSize.x-2, roomSize.y-2)/2.0f;
				eSettings.player = player;
				auto enemy = CharacterBuilder::createEnemy(eSettings);
				// Chance to drop powerup
				enemy->getComponent<CharacterComponent>()->onDeath = [pBuilder](GameObject* self) {
					int random = rand() % 100;
					if (random < 10) {
						random = rand() % 4; 
						DreamGame::instance->level->currentRoom->getComponent<RoomComponent>()->roomObjects.push_back(pBuilder->createSinglePowerupObject(static_cast<PowerupType>(random), self->getPosition()));
					}
				};
				newRoom->roomObjects.push_back(enemy);
			}
			break;
		case PowerUpRoom:
			for (int i = 0; i < roomSizeMultiplier; i++) {
				random = rand() % 4;
				newRoom->roomObjects.push_back(pBuilder->createSinglePowerupObject(static_cast<PowerupType>(random), { 300*i, 0 }));
			}
			break;
		case BossRoom:
			random = rand() % bossEnemySettings.size();
			eSettings = bossEnemySettings[random];
			eSettings.position = glm::vec2(roomSize.x / 2, roomSize.y / 2) - roomSize/2.0f;
			eSettings.player = player;
			auto enemy = CharacterBuilder::createEnemy(eSettings);
			// Drop portal to next level
			enemy->getComponent<CharacterComponent>()->onDeath = [this](GameObject* self) {
				DreamGame::instance->level->currentRoom->getComponent<RoomComponent>()->roomObjects.push_back(createPortal(self->getPosition()));
			};

			boss = std::shared_ptr<GameObject>(enemy);//is there a better way?
			newRoom->roomObjects.push_back(enemy);

			random = (rand() % 2) + 1;
			random *= roomSizeMultiplier;
			for (int i = 0; i < random; i++) {
				int randomEnemy = rand() % regularEnemySettings.size();
				eSettings = regularEnemySettings[randomEnemy];
				eSettings.name = eSettings.name + std::to_string(i);
				eSettings.position = glm::vec2(rand() % (int)(roomSize.x - 2), rand() % (int)(roomSize.y - 2)) - glm::vec2(roomSize.x - 2, roomSize.y - 2) / 2.0f;
				eSettings.player = player;
				auto enemy = CharacterBuilder::createEnemy(eSettings);
				// Chance to drop powerup
				enemy->getComponent<CharacterComponent>()->onDeath = [pBuilder, newRoom](GameObject* self) {
					int random = rand() % 100;
					if (random < 10) {
						random = rand() % 4;
						newRoom->roomObjects.push_back(pBuilder->createSinglePowerupObject(static_cast<PowerupType>(random), self->getPosition()));
					}
				};
				newRoom->roomObjects.push_back(enemy);
			}
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

void Level::unloadLevel() {
	for (auto go : currentRoom->getComponent<RoomComponent>()->roomObjects) {
		go->destroy();
	}
	currentRoom->getComponent<RoomComponent>()->roomObjects.clear();
	currentRoom->destroy();
	currentRoom = nullptr;
	roomObjects.clear();
}

std::shared_ptr<GameObject> Level::createPortal(glm::vec2 position) {
	auto game = DreamGame::instance;
	auto go = game->currentScene->createGameObject();
	go->setPosition(position);
	go->setScale(6);
	go->tag = Tag::Powerup;

	auto spriteComp = go->addComponent<SpriteComponent>();
	auto sprite = game->spriteAtlas_baseWraith->get("Spells-Effect.png");
	//Set the Enemy sprite to be on top of the background but behind the player
	sprite.setOrderInBatch(Depth::Enemy);
	sprite.setScale({ 6,6 });
	spriteComp->setSprite(sprite);

	auto physics = go->addComponent<PhysicsComponent>();
	float radius = (sprite.getSpriteSize().x * sprite.getScale().x / 2) / game->physicsScale;
	physics->initCircle(b2_staticBody, radius, go->getPosition() / game->physicsScale, 1);
	physics->setSensor(true);

	auto portal = go->addComponent<PortalComponent>();
	LevelSettings nextLevelSettings;
	nextLevelSettings.difficulty = difficulty + 1;
	nextLevelSettings.name = "Level " + std::to_string(nextLevelSettings.difficulty);
	nextLevelSettings.rooms = rooms + 2;
	portal->nextLevelSettings = nextLevelSettings;
	portal->level = this;

	return go;
}