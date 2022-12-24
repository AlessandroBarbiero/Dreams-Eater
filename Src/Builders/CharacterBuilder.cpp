#include "CharacterBuilder.hpp"
#include "DreamGame.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "IEnemyController.hpp"
#include "CharacterComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include <rapidjson.h>
#include <document.h>
#include <istreamwrapper.h>
#include <fstream>
#include <iostream>
#include <EnemyBehaviours/Wizard.hpp>
#include <Components/PowerupComponent.hpp>
constexpr auto SIZES_PATH = "Sprites/animationSizes.json";

unordered_map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> CharacterBuilder::atlasMap;
unordered_map<CharacterType, unordered_map<State, int>> CharacterBuilder::animationSizesMap;

std::shared_ptr<sre::SpriteAtlas> CharacterBuilder::getAtlas(CharacterType type) {
    if (atlasMap.size() == 0)
        initAtlasMap();
    return atlasMap[type];
}

unordered_map<State, int> CharacterBuilder::getAnimationSizes(CharacterType type) {
    if (animationSizesMap[type].size() == 0)
        initSizesMap(type);
    return animationSizesMap[type];
}

// Searching for the CharacterType it will return a function that links the right behaviour to the gameObject passed and return an EnemyController shared pointer
// If there is no Controller associated with the specified type a default value is chosen
std::function<std::shared_ptr<IEnemyController>(GameObject*)> findRightController(CharacterType type) {

    auto it = CharacterTypeToBehaviour.find(type);

    if (it == CharacterTypeToBehaviour.end()){
        auto default_value = [](GameObject* obj) {	return obj->addComponent<DEFAULT_BEHAVIOUR>();	};
        return default_value;
    }

    return it->second;
}


// Read the number of images for each animation from a json file and populate the animationSizesMap for the specific type of character
void CharacterBuilder::initSizesMap(CharacterType type) {
    auto& sizes = animationSizesMap[type];

    using namespace rapidjson;
    std::ifstream fis(SIZES_PATH);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    const char* typeString = CharacterTypeToString.at(type);

    Value& sizesMap = d[typeString];
    std::string name;
    int value;
    State state;
    for (auto& m : sizesMap.GetObject()) {
        name = m.name.GetString();
        value = m.value.GetInt();
        auto it = StringToState.find(name);
        state = it->second;
        sizes.insert({ state, value});
    }

}

// Create the atlases for all the types and save them in the atlasMap structure
void CharacterBuilder::initAtlasMap() {
    for (const auto& charStringPair : CharacterTypeToString)
    {
        std::string typeName = charStringPair.second;
        atlasMap.insert({ charStringPair.first,
            sre::SpriteAtlas::create("Sprites/Characters/" + typeName + "/" + typeName + "_atlas.json", "Sprites/Characters/" + typeName + "/" + typeName + "_atlas.png") });
    }
}

std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;
    auto type = settings.type; //todo reset to settings.type

    auto player = game->currentScene->createGameObject();
    player->name = settings.name;
    player->tag = Tag::Player;
    player->setPosition(settings.position);

    auto spriteComp = player->addComponent<SpriteComponent>();
    auto spriteAtlas = getAtlas(type);
    auto sprite = spriteAtlas->get("Right/Idle/0.png");
    sprite.setOrderInBatch(Depth::Player);
    spriteComp->setSprite(sprite);

    auto playerPhysics = player->addComponent<PhysicsComponent>();
    float radius = (sprite.getSpriteSize().x / 2.0f) / physicsScale;
    playerPhysics->initCircle(b2_dynamicBody, radius, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    playerPhysics->fixRotation();

    auto playerCharacter = player->addComponent<CharacterComponent>();
    //This will always be bullet if the main is a wraith, modify this if you want to play with another character
    auto shotSprite = spriteAtlas->get("Bullet.png");
    playerCharacter->setShotSprite(shotSprite);
    playerCharacter->type = type;
    playerCharacter->radius = radius;
    playerCharacter->hp = settings.hp;
    playerCharacter->speed = settings.speed;
    playerCharacter->armor = 5.0f; // TODO: put back --   settings.armor;
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
    std::unordered_map<State, int> animationSizes = getAnimationSizes(type);
    animationSetup(animation, spriteAtlas, animationSizes, 0.1f, Depth::Player);

    auto powerupComp = player->addComponent<PowerupComponent>();

    // Use for Guy
    // player->setScale(2.5f);

    return player;
}

// Insert both right and left animations related to the State
void insertAnimationSequence(std::shared_ptr<SpriteAnimationComponent> animation, State state, int animationSize,
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas, Depth visualDepth) {

    //RIGHT
    std::vector<sre::Sprite> rightAnimationVector(animationSize);
    std::string spriteName = std::string("Right/") + std::string(StateToString.at(state)) + std::string("/");

    for (int i = 0; i < rightAnimationVector.size(); i++) {
        rightAnimationVector[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        rightAnimationVector[i].setOrderInBatch(visualDepth);
    }

    animation->addAnimationSequence(state, Direction::RIGHT, rightAnimationVector);

    // LEFT
    std::vector<sre::Sprite> leftAnimationVector(animationSize);
    spriteName = std::string("Left/") + std::string(StateToString.at(state)) + std::string("/");

    for (int i = 0; i < leftAnimationVector.size(); i++) {
        leftAnimationVector[i] = spriteAtlas->get(spriteName + std::to_string(i) + ".png");
        leftAnimationVector[i].setOrderInBatch(visualDepth);
    }

    animation->addAnimationSequence(state, Direction::LEFT, leftAnimationVector);
}

// Fill up the animation component with all the animations following the animation sizes map 
void CharacterBuilder::animationSetup(std::shared_ptr<SpriteAnimationComponent> animation,
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas, std::unordered_map<State, int> animationSizes, float baseAnimTime, Depth visualDepth) {

    for (const auto& pairStateSize : animationSizes) {
        insertAnimationSequence(animation, pairStateSize.first, pairStateSize.second, spriteAtlas, visualDepth);
    }

    animation->setBaseAnimationTime(baseAnimTime);
}


std::shared_ptr<GameObject> CharacterBuilder::createEnemy(EnemySettings settings) {

    CharacterType type = CharacterType::Witch;       //TODO: put back --  settings.type;

    auto game = DreamGame::instance;
    auto physicsScale = DreamGame::instance->physicsScale;

    auto enemy = game->currentScene->createGameObject();
    enemy->name = settings.name;
    enemy->tag = Tag::Enemy;
    enemy->setPosition(settings.position);

    auto spriteComp = enemy->addComponent<SpriteComponent>();
    auto spriteAtlas = getAtlas(type);
    auto sprite = spriteAtlas->get("Right/Idle/0.png");
    //Set the Enemy sprite to be on top of the background but behind the player
    sprite.setOrderInBatch(Depth::Enemy);
    spriteComp->setSprite(sprite);

    auto physics = enemy->addComponent<PhysicsComponent>();
    float radius = (sprite.getSpriteSize().x * sprite.getScale().x / 2) / physicsScale;
    physics->initCircle(b2_dynamicBody, radius, enemy->getPosition(), 1);
    physics->getBody()->SetLinearDamping(5.0f);
    physics->fixRotation();

    auto enemyCharacter = enemy->addComponent<CharacterComponent>();

    auto shotSprite = getAtlas(CharacterType::Wizard)->get("Bullet.png");     //spriteAtlas->get("Bullet.png"); // TODO: Add from enemy behaviour
    enemyCharacter->setShotSprite(shotSprite);
    enemyCharacter->type = type;
    enemyCharacter->radius = radius;
    enemyCharacter->hp = settings.hp;
    enemyCharacter->armor = settings.armor;
    enemyCharacter->speed = settings.speed;
    enemyCharacter->damage = settings.damage;
    enemyCharacter->range = settings.range;
    enemyCharacter->rateOfFire = 1.0f; // TODO: put back --  settings.rateOfFire;
    enemyCharacter->shotSpeed = settings.shotSpeed;
    enemyCharacter->knockback = 0; // TODO: put back --  settings.knockback;

    // Add a IEnemyController based on the type of enemy
    auto& addControllerFunction = findRightController(type);
    auto enemyController = addControllerFunction(enemy.get());
    enemyController->character = enemyCharacter;
    enemyController->physics = physics;
    enemyController->player = settings.player;

    auto animation = enemy->addComponent<SpriteAnimationComponent>();

    std::unordered_map<State, int> animationSizes = getAnimationSizes(type);

    animationSetup(animation, spriteAtlas, animationSizes, 0.2f, Depth::Enemy);


    enemy->setScale(0.9f);
    return enemy;
}

void CharacterBuilder::transform(GameObject* character, CharacterType newType)
{
    auto playerCharacter = character->getComponent<CharacterComponent>();
    auto spriteAtlas = getAtlas(newType);
    auto shotSprite = spriteAtlas->get("Bullet.png");
    playerCharacter->setShotSprite(shotSprite);
    playerCharacter->type = newType;

    Depth depth = static_cast<Depth>(character->getComponent<SpriteComponent>()->getSprite().getOrderInBatch());

    auto animation = character->getComponent<SpriteAnimationComponent>();
    std::unordered_map<State, int> animationSizes = getAnimationSizes(newType);
    animationSetup(animation, spriteAtlas, animationSizes, animation->getBaseAnimationTime(), depth);

}