#include "CharacterBuilder.hpp"
#include "DreamGame.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"


std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;
    
    auto player = game->createGameObject();
    player->name = settings.name;
    player->setPosition(settings.position);

    auto spriteComp = player->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas_baseWraith()->get("Idle/Wraith_01_Idle_000.png");
    //Set the Player sprite to be on top of the background
    sprite.setOrderInBatch(Depth::Player);
    spriteComp->setSprite(sprite);

    auto playerPhysics = player->addComponent<PhysicsComponent>();
    playerPhysics->initCircle(b2_dynamicBody, (sprite.getSpriteSize().x / 2) / physicsScale, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    playerPhysics->fixRotation();

    auto playerController = player->addComponent<PlayerController>();
    playerController->speed = settings.speed;
    playerController->playerPhysics = playerPhysics;

    // Controls
    playerController->keyUp = settings.keybinds.up;
    playerController->keyDown = settings.keybinds.down;
    playerController->keyLeft = settings.keybinds.left;
    playerController->keyRight = settings.keybinds.right;

    return player;
}


std::shared_ptr<GameObject> CharacterBuilder::createEnemy(EnemySettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;

    auto enemy = game->createGameObject();
    enemy->name = settings.name;
    enemy->setPosition(settings.position);

    auto spriteComp = enemy->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas_baseWizard()->get("Idle/1_IDLE_000.png");
    //Set the Enemy sprite to be on top of the background but behind the player
    sprite.setOrderInBatch(Depth::Enemy);
    sprite.setScale(glm::vec2(0.9f));
    spriteComp->setSprite(sprite);

    auto physics = enemy->addComponent<PhysicsComponent>();
    physics->initCircle(b2_dynamicBody, (sprite.getSpriteSize().x / 2) / physicsScale, enemy->getPosition(), 1);
    physics->getBody()->SetLinearDamping(5.0f);
    physics->fixRotation();

    auto enemyController = enemy->addComponent<EnemyController>();
    enemyController->speed = settings.speed;
    enemyController->physics = physics;
    enemyController->player = settings.player;
    enemyController->idealDistance = settings.idealDistance;
    enemyController->range = settings.range;

    return enemy;
}