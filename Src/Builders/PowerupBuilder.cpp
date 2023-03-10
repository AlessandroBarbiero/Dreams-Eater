#include "PowerupBuilder.hpp"
#include <Components/PowerupComponent.hpp>
#include <PhysicsComponent.hpp>
#include <DreamGame.hpp>
#include <CharacterType.hpp>
#include <CharacterComponent.hpp>
#include <CharacterBuilder.hpp> //Used for transform function
#include <PlayerController.hpp>

PowerupBuilder::PowerupBuilder(){}

void PowerupBuilder::firePowerup(GameObject* player)
{
    player->getComponent<CharacterComponent>()->damage += 2;
}

void PowerupBuilder::waterPowerup(GameObject* player)
{
    auto character = player->getComponent<CharacterComponent>();
    character->speed += 3;
    character->armor += 0.1;
    character->rateOfFire += 0.5;
}

void PowerupBuilder::icePowerup(GameObject* player)
{
    auto character = player->getComponent<CharacterComponent>();
    character->speed -= 0.5;
    float newHp = character->hp + 10;
    character->hp = newHp > character->maxHp ? character->maxHp : newHp;
}

void PowerupBuilder::earthPowerup(GameObject* player)
{
    auto character = player->getComponent<CharacterComponent>();
    if (character->type != CharacterType::BrownWraith)
        CharacterBuilder::transform(player, CharacterType::BrownWraith);
    else {
        auto controller = player->getComponent<PlayerController>();
        controller->superDmg += 3;
        controller->superCooldown -= 0.2f;
    }
}

std::shared_ptr<GameObject> PowerupBuilder::createSinglePowerupObject(PowerupType type, glm::vec2 position)
{
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto& powerup = PowerupMap.at(type);

    auto powerupObj = game->currentScene->createGameObject();
    powerupObj->name = powerup->name;
    powerupObj->tag = Tag::Powerup;
    powerupObj->setPosition(position);

    auto powerupComp = powerupObj->addComponent<PowerupComponent>();
    powerupComp->addPowerup(powerup);

    auto spriteComp = powerupObj->addComponent<SpriteComponent>();
    // Maybe if we want to differenciate the image inside the powerup and the displayed one we can do something here
    // auto spriteAtlas = getPowerupAtlas();
    // auto sprite = spriteAtlas->get("SomethingElse");
    auto sprite = powerup->getSprite();
    sprite.setOrderInBatch(Depth::Object);
    spriteComp->setSprite(sprite);

    auto powerupPhysics = powerupObj->addComponent<PhysicsComponent>();
    float radius = (sprite.getSpriteSize().x / 2.0f) / physicsScale;
    powerupPhysics->initCircle(b2_dynamicBody, radius, powerupObj->getPosition() / physicsScale, 1);
    powerupPhysics->fixRotation();
    powerupPhysics->setSensor(true);

    powerupObj->setScale(0.4f);

	return powerupObj;
}


std::shared_ptr<sre::SpriteAtlas> PowerupBuilder::getPowerupAtlas()
{
	if (PowerupBuilder::powerupAtlas == nullptr) {
        std::cout << "Loading atlas for Pickups" << std::endl;
		PowerupBuilder::powerupAtlas = sre::SpriteAtlas::create("Sprites/PickUp/PickUp_atlas.json", "Sprites/PickUp/PickUp_atlas.png");
	}
	return PowerupBuilder::powerupAtlas;
}


PowerupBuilder* PowerupBuilder::getInstance()
{
	static PowerupBuilder instance;
	return &instance;
}
