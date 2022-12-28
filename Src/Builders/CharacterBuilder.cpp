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
constexpr auto R_TO_SPRITE_RATIO = 0.6;

unordered_map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> CharacterBuilder::atlasMap;
unordered_map<CharacterType, unordered_map<State, int>> CharacterBuilder::animationSizesMap;

std::shared_ptr<sre::SpriteAtlas> CharacterBuilder::getAtlas(CharacterType type) {
#ifdef LAZY_LOADING
    // Use this for lazy resource loading
    auto it = atlasMap.find(type);
    // If the atlas doesn't exist yet in the map
    if (it == atlasMap.end())
        initAtlasMap(type);
#else
    if (atlasMap.size() == 0)
        initAtlasMap();
#endif
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
    std::cout << "Read animation dimensions from json for "<< CharacterTypeToString.at(type) << std::endl;
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
    std::cout << "Loading atlas resources for all Characters" << std::endl;
    for (const auto& charStringPair : CharacterTypeToString)
    {
        std::string typeName = charStringPair.second;
        atlasMap.insert({ charStringPair.first,
            sre::SpriteAtlas::create("Sprites/Characters/" + typeName + "/" + typeName + "_atlas.json", "Sprites/Characters/" + typeName + "/" + typeName + "_atlas.png") });
    }
}

void CharacterBuilder::initAtlasMap(CharacterType type)
{
    std::string typeName = CharacterTypeToString.at(type);
    std::cout << "Loading atlas resources for " << typeName << std::endl;
    atlasMap.insert({ type,
        sre::SpriteAtlas::create("Sprites/Characters/" + typeName + "/" + typeName + "_atlas.json", "Sprites/Characters/" + typeName + "/" + typeName + "_atlas.png") });
}

std::shared_ptr<GameObject> CharacterBuilder::createPlayer(PlayerSettings settings) {
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;
    auto type = settings.type;

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
    float radius = R_TO_SPRITE_RATIO*(sprite.getSpriteSize().x / 2.0f) / physicsScale;
    playerPhysics->initCircle(b2_dynamicBody, radius, player->getPosition(), 1);
    playerPhysics->getBody()->SetLinearDamping(5.0f);
    playerPhysics->fixRotation();

    auto playerCharacter = player->addComponent<CharacterComponent>();
    playerCharacter->type = type;
    playerCharacter->radius = radius;
    playerCharacter->hp = settings.hp;
    playerCharacter->speed = settings.speed;
    playerCharacter->armor = settings.armor;
    playerCharacter->damage = settings.damage;
    playerCharacter->range = settings.range;
    playerCharacter->rateOfFire = settings.rateOfFire;
    playerCharacter->shotSpeed = settings.shotSpeed;
    playerCharacter->knockback = settings.knockback;

    auto playerController = player->addComponent<PlayerController>();
    playerController->playerPhysics = playerPhysics;
    playerController->character = playerCharacter;
    playerController->setBulletSprites(spriteAtlas.get());

    // Controls
    playerController->keyUp = settings.keybinds.up;
    playerController->keyDown = settings.keybinds.down;
    playerController->keyLeft = settings.keybinds.left;
    playerController->keyRight = settings.keybinds.right;
    playerController->keyShot = settings.keybinds.shot;

    playerController->keyShootUp = settings.keybinds.shootUp;
    playerController->keyShootDown = settings.keybinds.shootDown;
    playerController->keyShootLeft = settings.keybinds.shootLeft;
    playerController->keyShootRight = settings.keybinds.shootRight;

     
    auto animation = player->addComponent<SpriteAnimationComponent>();
    std::unordered_map<State, int> animationSizes = getAnimationSizes(type);
    animationSetup(animation, spriteAtlas, animationSizes, 0.1f, Depth::Player);

    auto powerupComp = player->addComponent<PowerupComponent>();

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

    CharacterType type = settings.type;

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
    float radius = R_TO_SPRITE_RATIO*(sprite.getSpriteSize().x * sprite.getScale().x / 2) / physicsScale;
    physics->initCircle(b2_dynamicBody, radius, enemy->getPosition(), 1);
    physics->getBody()->SetLinearDamping(5.0f);
    physics->fixRotation();

    auto enemyCharacter = enemy->addComponent<CharacterComponent>();

    enemyCharacter->type = type;
    enemyCharacter->radius = radius;
    enemyCharacter->hp = settings.hp;
    enemyCharacter->armor = settings.armor;
    enemyCharacter->speed = settings.speed;
    enemyCharacter->damage = settings.damage;
    enemyCharacter->range = settings.range;
    enemyCharacter->rateOfFire = settings.rateOfFire;
    enemyCharacter->shotSpeed = settings.shotSpeed;
    enemyCharacter->knockback = settings.knockback;

    // Add a IEnemyController based on the type of enemy
    auto& addControllerFunction = findRightController(type);
    auto enemyController = addControllerFunction(enemy.get());
    enemyController->character = enemyCharacter;
    enemyController->physics = physics;
    enemyController->player = settings.player;
    enemyController->setBulletSprites(spriteAtlas.get());

    auto animation = enemy->addComponent<SpriteAnimationComponent>();

    std::unordered_map<State, int> animationSizes = getAnimationSizes(type);

    animationSetup(animation, spriteAtlas, animationSizes, 0.2f, Depth::Enemy);


   enemy->setScale(settings.scale);
   // enemy->setScale(2.0f);
    return enemy;
}

void CharacterBuilder::transform(GameObject* character, CharacterType newType)
{
    auto playerCharacter = character->getComponent<CharacterComponent>();
    auto spriteAtlas = getAtlas(newType);
    playerCharacter->type = newType;

    if (character->tag == Tag::Player) {
        auto playerController = character->getComponent<PlayerController>();
        playerController->setBulletSprites(spriteAtlas.get());
    }
    else if (character->tag == Tag::Enemy) {
        auto enemyController = character->getComponent<IEnemyController>();
        enemyController->setBulletSprites(spriteAtlas.get());
    }


    Depth depth = static_cast<Depth>(character->getComponent<SpriteComponent>()->getSprite().getOrderInBatch());

    auto animation = character->getComponent<SpriteAnimationComponent>();
    std::unordered_map<State, int> animationSizes = getAnimationSizes(newType);
    animationSetup(animation, spriteAtlas, animationSizes, animation->getBaseAnimationTime(), depth);

}