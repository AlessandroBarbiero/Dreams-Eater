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

	// DoorPosition doesn't matter since the third parameter is true, so just Top as default
	loadRoom(0, Top, true);
}

void Level::loadRoom(int room, DoorPosition enteredAt, bool enterAtCenter) {
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
		auto roomSize = newRoom->getRoomSizeInPixels();
		//std::cout << "roomSize: (" << roomSize.x << ", " << roomSize.y << ")" << std::endl;
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
			random = (rand() % 3) + difficulty;
			random *= roomSizeMultiplier;
			for (int i = 0; i < random; i++) {
				int randomEnemy = rand() % regularEnemySettings.size();
				eSettings = regularEnemySettings[randomEnemy];
				eSettings.name = eSettings.name + std::to_string(i);
				eSettings.position = glm::vec2(rand() % (int)(roomSize.x - 200), rand() % (int)(roomSize.y - 200)) - glm::vec2(roomSize.x-200, roomSize.y-200)/2.0f;
				eSettings.player = player;
				auto enemy = CharacterBuilder::createEnemy(eSettings);
				// Chance to drop powerup
				enemy->getComponent<CharacterComponent>()->onDeath = [pBuilder](GameObject* self) {
					int random = rand() % 100;
					if (random < 5) {
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
			// Drop portal to next level and clean up pointers
			enemy->getComponent<CharacterComponent>()->onDeath = [this](GameObject* self) {
				DreamGame::instance->level->currentRoom->getComponent<RoomComponent>()->roomObjects.push_back(createPortal(self->getPosition()));
				this->boss.reset();
			};

			boss = std::shared_ptr<GameObject>(enemy);//is there a better way?
			newRoom->roomObjects.push_back(enemy);

			random = (rand() % 2) + difficulty;
			random *= roomSizeMultiplier;
			for (int i = 0; i < random; i++) {
				int randomEnemy = rand() % regularEnemySettings.size();
				eSettings = regularEnemySettings[randomEnemy];
				eSettings.name = eSettings.name + std::to_string(i);
				eSettings.position = glm::vec2(rand() % (int)(roomSize.x - 200), rand() % (int)(roomSize.y - 200)) - glm::vec2(roomSize.x - 200, roomSize.y - 200) / 2.0f;
				eSettings.player = player;
				auto enemy = CharacterBuilder::createEnemy(eSettings);
				// Chance to drop powerup
				enemy->getComponent<CharacterComponent>()->onDeath = [pBuilder](GameObject* self) {
					int random = rand() % 100;
					if (random < 5) {
						random = rand() % 4;
						DreamGame::instance->level->currentRoom->getComponent<RoomComponent>()->roomObjects.push_back(pBuilder->createSinglePowerupObject(static_cast<PowerupType>(random), self->getPosition()));
					}
				};
				newRoom->roomObjects.push_back(enemy);
			}
			break;
		}

	}
	roomEntered[room] = true;

	auto phys = player->getComponent<PhysicsComponent>();
	glm::vec2 enterPos;
	if (enterAtCenter) {
		// Overrides enteredAt
		enterPos = { 0, 0 };
	}
	else {
		enterPos = newRoom->doorEntrances[enteredAt] / DreamGame::instance->physicsScale;
	}
	phys->getBody()->SetTransform({enterPos.x, enterPos.y} , 0);

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
	auto sprite = getPurpleWraithAtlas()->get("Bullet.png");

	sprite.setOrderInBatch(Depth::Object);
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