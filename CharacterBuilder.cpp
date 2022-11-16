#include "CharacterBuilder.hpp"
#include "DreamGame.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "CharacterComponent.hpp"


std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;
    
    auto player = game->createGameObject();
    player->name = settings.name;
    player->tag = Tag::Player;
    player->setPosition(settings.position);

    auto spriteComp = player->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas_baseWraith()->get("Idle/0.png");
    sprite.setOrderInBatch(Depth::Player);
    spriteComp->setSprite(sprite);

    auto playerPhysics = player->addComponent<PhysicsComponent>();
    playerPhysics->initCircle(b2_dynamicBody, (sprite.getSpriteSize().x / 2) / physicsScale, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    playerPhysics->fixRotation();

    auto playerCharacter = player->addComponent<CharacterComponent>();
    auto shotSprite = spriteAtlas_baseWraith()->get("Spells-Effect.png");
    playerCharacter->setShotSprite(shotSprite);

    auto playerController = player->addComponent<PlayerController>();
    playerController->speed = settings.speed;
    playerController->playerPhysics = playerPhysics;
    playerController->character = playerCharacter;

    // Controls
    playerController->keyUp = settings.keybinds.up;
    playerController->keyDown = settings.keybinds.down;
    playerController->keyLeft = settings.keybinds.left;
    playerController->keyRight = settings.keybinds.right;
    playerController->keyShot = settings.keybinds.shot;

    return player;
}


std::shared_ptr<GameObject> CharacterBuilder::createEnemy(EnemySettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;

    auto enemy = game->createGameObject();
    enemy->name = settings.name;
    enemy->tag = Tag::Enemy;
    enemy->setPosition(settings.position);

    auto spriteComp = enemy->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas_baseWizard()->get("Idle/0.png");
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