#include "CharacterBuilder.hpp"
#include "DreamGame.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "CharacterComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>
constexpr auto SIZES_PATH = "Sprites/animationSizes.json";

map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> CharacterBuilder::atlasMap;
map<CharacterType, map<State, int>> CharacterBuilder::animationSizesMap;

std::shared_ptr<sre::SpriteAtlas> CharacterBuilder::getAtlas(CharacterType type) {
    if (atlasMap.size() == 0)
        initAtlasMap();
    return atlasMap[type];
}

map<State, int> CharacterBuilder::getAnimationSizes(CharacterType type) {
    if (animationSizesMap[type].size() == 0)
        initSizesMap(type);
    return animationSizesMap[type];
}

void CharacterBuilder::initSizesMap(CharacterType type) {
    auto& sizes = animationSizesMap[type];

    using namespace rapidjson;
    std::ifstream fis(SIZES_PATH);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    const char* typeString = CharacterTypeToString.at(type);

    Value& sizesMap = d[typeString];
    sizes.insert({ State::Idle,         sizesMap["Idle"].GetInt()});
    sizes.insert({ State::AttackRight,  sizesMap["Attack"].GetInt() });
    sizes.insert({ State::WalkRight,    sizesMap["Walk"].GetInt() });
    sizes.insert({ State::Die,          sizesMap["Die"].GetInt() });

}


void CharacterBuilder::initAtlasMap() {
    for (const auto& charStringPair : CharacterTypeToString)
    {
        std::string typeName = charStringPair.second;
        atlasMap.insert({ charStringPair.first,
            sre::SpriteAtlas::create("Sprites/Characters/" + typeName + "/" + typeName + "_atlas.json", "Sprites/Characters/" + typeName + "/" + typeName + "_atlas.png") });

    }
    /*atlasMap.insert({ CharacterType::Wraith, sre::SpriteAtlas::create("Sprites/Wraith/Wraith_base_atlas.json", "Sprites/Wraith/Wraith_base_atlas.png") });
    atlasMap.insert({ CharacterType::BrownWraith, sre::SpriteAtlas::create("Sprites/Wraith/Wraith_brown_atlas.json", "Sprites/Wraith/Wraith_brown_atlas.png") });
    atlasMap.insert({ CharacterType::PurpleWraith, sre::SpriteAtlas::create("Sprites/Wraith/Wraith_purple_atlas.json", "Sprites/Wraith/Wraith_purple_atlas.png") });
    atlasMap.insert({ CharacterType::Wizard, sre::SpriteAtlas::create("Sprites/Wizard/Wizard_base_atlas.json", "Sprites/Wizard/Wizard_base_atlas.png") });
    atlasMap.insert({ CharacterType::FireWizard, sre::SpriteAtlas::create("Sprites/Wizard/Wizard_fire_atlas.json", "Sprites/Wizard/Wizard_fire_atlas.png") });
    atlasMap.insert({ CharacterType::IceWizard, sre::SpriteAtlas::create("Sprites/Wizard/Wizard_ice_atlas.json", "Sprites/Wizard/Wizard_ice_atlas.png") });*/
}

std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;
    
    auto player = game->currentScene->createGameObject();
    player->name = settings.name;
    player->tag = Tag::Player;
    player->setPosition(settings.position);

    auto spriteComp = player->addComponent<SpriteComponent>();
    auto spriteAtlas = getAtlas(settings.type);
    auto sprite = spriteAtlas->get("Idle/0.png");
    sprite.setOrderInBatch(Depth::Player);
    spriteComp->setSprite(sprite);

    auto playerPhysics = player->addComponent<PhysicsComponent>();
    float radius = (sprite.getSpriteSize().x / 2.0f) / physicsScale;
    playerPhysics->initCircle(b2_dynamicBody, radius, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    playerPhysics->fixRotation();

    auto playerCharacter = player->addComponent<CharacterComponent>();
    auto shotSprite = getAtlas(CharacterType::Wraith)->get("Spells-Effect.png");
    playerCharacter->setShotSprite(shotSprite);
    playerCharacter->radius = radius;
    playerCharacter->hp = settings.hp;
    playerCharacter->speed = settings.speed;
    playerCharacter->armor = settings.armor;
    playerCharacter->damage = settings.damage;
    playerCharacter->range = settings.range;
    playerCharacter->rateOfFire = settings.rateOfFire;
    playerCharacter->shotSpeed = settings.shotSpeed;
    playerCharacter->knockback = settings.knockback;
    playerCharacter->useShootingKeys = true;
    playerCharacter->keyShootUp = settings.keybinds.shootUp;
    playerCharacter->keyShootDown = settings.keybinds.shootDown;
    playerCharacter->keyShootLeft = settings.keybinds.shootLeft;
    playerCharacter->keyShootRight = settings.keybinds.shootRight;


    auto playerController = player->addComponent<PlayerController>();
    playerController->playerPhysics = playerPhysics;
    playerController->character = playerCharacter;

    // Controls
    playerController->keyUp = settings.keybinds.up;
    playerController->keyDown = settings.keybinds.down;
    playerController->keyLeft = settings.keybinds.left;
    playerController->keyRight = settings.keybinds.right;
    playerController->keyShot = settings.keybinds.shot;
     
    auto animation = player->addComponent<SpriteAnimationComponent>();
    std::map<State, int> animationSizes = getAnimationSizes(settings.type);
    /*animationSizes.insert({ State::Idle, 12 });
    animationSizes.insert({ State::AttackRight, 12 });
    animationSizes.insert({ State::WalkRight, 12 });
    animationSizes.insert({ State::Die, 15 });*/
    animationSetup(animation, spriteAtlas, animationSizes, 0.1f, Depth::Player);

    player->setScale(2.5f);

    return player;
}

void CharacterBuilder::animationSetup(std::shared_ptr<SpriteAnimationComponent> animation,
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas, std::map<State, int> animationSizes, float baseAnimTime, Depth visualDepth) {

    std::vector<sre::Sprite> idleAnim(animationSizes[State::Idle]);
    std::string spriteName = "Idle/";
    for (int i = 0; i < idleAnim.size(); i++) {
        idleAnim[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        idleAnim[i].setOrderInBatch(visualDepth);
    }

    std::vector<sre::Sprite> attackRAnim(animationSizes[State::AttackRight]);
    spriteName = "Attack/";
    for (int i = 0; i < attackRAnim.size(); i++) {
        attackRAnim[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        attackRAnim[i].setOrderInBatch(visualDepth);
    }
    std::vector<sre::Sprite> attackLAnim(attackRAnim.size());
    for (int i = 0; i < attackLAnim.size(); i++) {
        attackLAnim[i] = attackRAnim[i];
        attackLAnim[i].setFlip({ true, false });
    }

    std::vector<sre::Sprite> walkRAnim(animationSizes[State::AttackRight]);
    spriteName = "Walk/";
    for (int i = 0; i < walkRAnim.size(); i++) {
        walkRAnim[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        walkRAnim[i].setOrderInBatch(visualDepth);
    }
    std::vector<sre::Sprite> walkLAnim(walkRAnim.size());
    for (int i = 0; i < walkLAnim.size(); i++) {
        walkLAnim[i] = walkRAnim[i];
        walkLAnim[i].setFlip({ true, false });
    }

    std::vector<sre::Sprite> dieAnim(animationSizes[State::Die]);
    spriteName = "Die/";
    for (int i = 0; i < dieAnim.size(); i++) {
        dieAnim[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        dieAnim[i].setOrderInBatch(visualDepth);
    }

    animation->addAnimationSequence(State::Idle, idleAnim);
    animation->addAnimationSequence(State::WalkRight, walkRAnim);
    animation->addAnimationSequence(State::WalkLeft, walkLAnim);
    animation->addAnimationSequence(State::AttackRight, attackRAnim);
    animation->addAnimationSequence(State::AttackLeft, attackLAnim);
    animation->addAnimationSequence(State::Die, dieAnim);
    animation->setBaseAnimationTime(baseAnimTime);
}

std::shared_ptr<GameObject> CharacterBuilder::createEnemy(EnemySettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;

    auto enemy = game->currentScene->createGameObject();
    enemy->name = settings.name;
    enemy->tag = Tag::Enemy;
    enemy->setPosition(settings.position);

    auto spriteComp = enemy->addComponent<SpriteComponent>();
    auto spriteAtlas = getAtlas(settings.type);
    auto sprite = spriteAtlas->get("Idle/0.png");
    //Set the Enemy sprite to be on top of the background but behind the player
    sprite.setOrderInBatch(Depth::Enemy);
    spriteComp->setSprite(sprite);

    auto physics = enemy->addComponent<PhysicsComponent>();
    float radius = (sprite.getSpriteSize().x * sprite.getScale().x / 2) / physicsScale;
    physics->initCircle(b2_dynamicBody, radius, enemy->getPosition(), 1);
    physics->getBody()->SetLinearDamping(5.0f);
    physics->fixRotation();

    auto enemyCharacter = enemy->addComponent<CharacterComponent>();
    auto shotSprite = getAtlas(CharacterType::PurpleWraith)->get("Spells-Effect.png");
    enemyCharacter->setShotSprite(shotSprite);
    enemyCharacter->radius = radius;
    enemyCharacter->hp = settings.hp;
    enemyCharacter->armor = settings.armor;
    enemyCharacter->speed = settings.speed;
    enemyCharacter->damage = settings.damage;
    enemyCharacter->range = settings.range;
    enemyCharacter->rateOfFire = settings.rateOfFire;
    enemyCharacter->shotSpeed = settings.shotSpeed;
    enemyCharacter->knockback = settings.knockback;

    auto enemyController = enemy->addComponent<EnemyController>();
    enemyController->character = enemyCharacter;
    enemyController->physics = physics;
    enemyController->player = settings.player;
    enemyController->idealDistance = settings.idealDistance * physicsScale;

    auto animation = enemy->addComponent<SpriteAnimationComponent>();

    std::map<State, int> animationSizes = getAnimationSizes(settings.type);
    /*animationSizes.insert({ State::Idle, 5 });
    animationSizes.insert({ State::AttackRight, 5 });
    animationSizes.insert({ State::WalkRight, 5 });
    animationSizes.insert({ State::Die, 5 });*/
    animationSetup(animation, spriteAtlas, animationSizes, 0.2f, Depth::Enemy);


    enemy->setScale(0.9f);
    return enemy;
}