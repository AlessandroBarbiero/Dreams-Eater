#include "CharacterBuilder.hpp"
#include "DreamGame.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"


std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;
    
    auto player = game->createGameObject();
    player->setPosition(settings.position);
    player->name = "player";
    auto playerController = player->addComponent<PlayerController>();
    // TODO: Set speed from settings
    playerController->speed = settings.speed;

    auto spriteWraithComp = player->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas_baseWraith()->get("Idle/Wraith_01_Idle_000.png");

    sprite.setOrderInBatch(10);
    spriteWraithComp->setSprite(sprite);
    std::cout << spriteWraithComp->getSprite().getOrderInBatch() << std::endl;


    auto playerPhysics = player->addComponent<PhysicsComponent>();

    playerPhysics->initCircle(b2_dynamicBody, (sprite.getSpriteSize().x / 2) / physicsScale, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    std::cout << playerPhysics->getBody()->GetLinearDamping() << std::endl;
    playerPhysics->fixRotation();

    playerController->playerPhysics = playerPhysics;

    // Controls
    playerController->keyUp = settings.keybinds.up;
    playerController->keyDown = settings.keybinds.down;
    playerController->keyLeft = settings.keybinds.left;
    playerController->keyRight = settings.keybinds.right;

    return player;
}