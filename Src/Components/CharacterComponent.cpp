#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"
#include "DreamGame.hpp"
#include "SpriteComponent.hpp"
#include "BulletComponent.hpp"
#include "DreamInspector.hpp"
#include "SpriteAnimationComponent.hpp"
#define KNOCKBACK_SCALE 10

std::shared_ptr<sre::SpriteAtlas> CharacterComponent::effectAtlas;

CharacterComponent::CharacterComponent(GameObject* gameObject) : Component(gameObject) {
    if (CharacterComponent::effectAtlas == nullptr) {
        CharacterComponent::effectAtlas = sre::SpriteAtlas::create("Sprites/SpecialEffects/SpecialEffects_atlas.json", "Sprites/SpecialEffects/SpecialEffects_atlas.png");
    }

    initSpecialEffectObject();
}

void CharacterComponent::initSpecialEffectObject() {
    auto specialEffectsObj = DreamGame::instance->currentScene->createGameObject();
    specialEffectsObj->name = "SpecialEffectsOf" + gameObject->name;

    auto sprComp = specialEffectsObj->addComponent<SpriteComponent>();
    sprComp->deactivate();
    specialEffects = specialEffectsObj->addComponent<SpriteAnimationComponent>();
    // The special effect should be visible only when necessary
    specialEffects->deactivate();

    // Load the hit animation from memory
    std::vector<sre::Sprite> hitAnim(11);
    std::string spriteName = "Hit/";
    for (int i = 0; i < hitAnim.size(); i++) {
        hitAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        hitAnim[i].setOrderInBatch(Depth::Effect);
    }

    std::vector<sre::Sprite> victoryAnim(16);
    spriteName = "Victory/";
    for (int i = 0; i < victoryAnim.size(); i++) {
        victoryAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        victoryAnim[i].setOrderInBatch(Depth::Effect);
    }

    specialEffects->addAnimationSequence(State::Victory,    Direction::RIGHT,       victoryAnim);
    specialEffects->addAnimationSequence(State::Victory,    Direction::LEFT,        victoryAnim);
    specialEffects->addAnimationSequence(State::Hit,        Direction::RIGHT,       hitAnim);
    specialEffects->addAnimationSequence(State::Hit,        Direction::LEFT,        hitAnim);
    specialEffects->setBaseAnimationTime(0.1f);

    gameObject->addChild(specialEffectsObj.get());
}

void CharacterComponent::update(float deltaTime) {
    if(stun)
        updateStunTimeout(deltaTime);

    checkRateOfFire(deltaTime);

    if(useShootingKeys)
        fireOnKeyPress();
   
    updateFlyingProj();
}

// If the keys for fire are pressed generate a bullet in the right direction
void CharacterComponent::fireOnKeyPress() {

    glm::vec2 direction{ 0,0 };
    if (up)
        direction.y++;
    if (down)
        direction.y--;
    if (left)
        direction.x--;
    if (right)
        direction.x++;

    if (direction != glm::vec2(0)) {
        auto anim = gameObject->getComponent<SpriteAnimationComponent>();
        direction = glm::normalize(direction);
        anim->displayCompleteAnimation(State::Attack, 1 / rateOfFire, [direction, this]() {shoot(direction); });
        anim->setFacingDirection(vectorToDirection(direction));

        // If the animation cannot go any faster just spawn the bullets
        if (1 / rateOfFire < anim->getMinDuration())
            shoot(direction);

        return;
    }
}

void CharacterComponent::changeState(State newState)
{
    state = newState;
}

Direction CharacterComponent::getDirection()
{
    return facingDirection;
}

void CharacterComponent::setDirection(Direction newFacingDirection)
{
    facingDirection = newFacingDirection;
}

// Update readyToShoot variable based on rateOfFire and cooldownTimer
void CharacterComponent::checkRateOfFire(float deltaTime) {
    if (!readyToShoot) {
        shotCooldownTimer += deltaTime;
        if (shotCooldownTimer >= 1 / rateOfFire)
            readyToShoot = true;
    }
}

//Look for the first projectile shot still in the scene and check if it has to be destroyed (reached the range)
void CharacterComponent::updateFlyingProj() {
    if (flyingProj.empty())
        return;

    auto& lastProj = flyingProj.front();
    // Remove the projectiles collided and already destroyed
    while (lastProj.expired()) {
        flyingProj.pop();
        if (flyingProj.empty())
            return;
        lastProj = flyingProj.front();
    }

    if (std::shared_ptr<BulletComponent> bullet = lastProj.lock()) {
        if (bullet->finished()) {
            flyingProj.pop();
            bullet->destroyBullet();
        }
    }
    else
        flyingProj.pop();
}

bool CharacterComponent::onKey(SDL_Event& event) {
    if (useShootingKeys) {
        auto sym = event.key.keysym.sym;
        if (sym == keyShootUp) 
            up = event.type == SDL_KEYDOWN;        
        if (sym == keyShootDown) 
            down = event.type == SDL_KEYDOWN;        
        if (sym == keyShootLeft) 
            left = event.type == SDL_KEYDOWN;        
        if (sym == keyShootRight) 
            right = event.type == SDL_KEYDOWN;
    }

    return false;
}

void CharacterComponent::onCollisionStart(PhysicsComponent* comp) {
    if (hp <= 0)
        return;
    Tag myTag = gameObject->tag;
    Tag hisTag = comp->getGameObject()->tag;
    if (myTag == Tag::Player && hisTag == Tag::Enemy) {
        auto enemy = comp->getGameObject()->getComponent<CharacterComponent>();
        inflictDamage(enemy->damage);
    }
    if (myTag == Tag::Player && hisTag == Tag::EnemyBullet ||
        myTag == Tag::Enemy && hisTag == Tag::PlayerBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        applyKnockback(bullet);
        inflictDamage(bullet->getDamage());
    }
}

void CharacterComponent::applyKnockback(std::shared_ptr<BulletComponent> bullet) {
    if (bullet->knockback > 0) {
        stunned(true);
        glm::vec2 direction = glm::normalize(gameObject->getPosition() - bullet->getGameObject()->getPosition());
        auto phys = gameObject->getComponent<PhysicsComponent>();
        phys->addImpulse(direction * bullet->knockback);
    }
}

void CharacterComponent::inflictDamage(float damage) {
    float realDamage = damage - armor;
    if (realDamage > 0) {
        hp -= realDamage;
        if (hp <= 0) {
            state = State::Die;
            auto anim = gameObject->getComponent<SpriteAnimationComponent>();
            anim->displayCompleteAnimation(state, [this]() {die(); }, true);
        }
        else
            showEffect(State::Hit);
    }
}

void CharacterComponent::die() {
    gameObject->destroy();
    if (gameObject->tag == Tag::Player)
        DreamGame::instance->gameOver();
}


void CharacterComponent::updateStunTimeout(float deltaTime) {
    stunTimeout -= deltaTime;
    if (stunTimeout <= 0) {
        stun = false;
        stunTimeout = 0;
    }
}

void CharacterComponent::stunned(float stunTimeout) {
    this->stun = true;
    this->stunTimeout += stunTimeout;
}

void CharacterComponent::stunned(bool stun) {
    this->stun = stun;
    if (stun)
        stunTimeout = KNOCKBACK_TIME;
}

State CharacterComponent::getState()
{
    return state;
}


// Shoot a bullet in the passed direction that is destroyed after it travels for range distance, this method is subjected to the stats:
// - damage
// - shotSpeed
// - range
// - rateOfFire
void CharacterComponent::shoot(glm::vec2 direction) {
    if (!readyToShoot) 
        return; // cooldown is not finished

    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto shot = game->currentScene->createGameObject();
    shot->name = "Bullet";
    if (gameObject->tag == Tag::Player)
        shot->tag = Tag::PlayerBullet;
    else if (gameObject->tag == Tag::Enemy)
        shot->tag = Tag::EnemyBullet;
    else
        shot->tag = Tag::Bullet;

    glm::vec2 position = gameObject->getPosition() / physicsScale + direction * (radius * gameObject->getScale() + damage/2);
    shot->setPosition(position * physicsScale);
    

    auto spriteComp = shot->addComponent<SpriteComponent>();
    //shotSprite.setScale(glm::vec2(damage));
    spriteComp->setSprite(shotSprite);


    auto shotPhy = shot->addComponent<PhysicsComponent>();
    float radius = shotSprite.getSpriteSize().x / (2 * physicsScale);
    shotPhy->initCircle(b2_dynamicBody, radius, position, 1);
    shotPhy->setLinearVelocity(direction * shotSpeed);
    shotPhy->setSensor(true);

    shot->setScale({ damage });

    auto bullet = shot->addComponent<BulletComponent>();
    bullet->startingPosition = shot->getPosition();
    bullet->range = range;
    bullet->damage = damage;
    bullet->knockback = knockback * KNOCKBACK_SCALE;
    std::weak_ptr<BulletComponent> weakBullet = bullet;
    flyingProj.push(weakBullet);
    startShotCooldown();
}

void CharacterComponent::setShotSprite(const sre::Sprite& sprite) {
    shotSprite = sprite;
    shotSprite.setOrderInBatch(Depth::Bullet);
}

void CharacterComponent::startShotCooldown() {
    readyToShoot = false;
    shotCooldownTimer = 0;
}

void CharacterComponent::showEffect(State effect) {
    specialEffects->displayOnce(effect);
}



// %%%%%%%%%%%%%%%%% GUI %%%%%%%%%%%%%%%%%%%%%%%%

void CharacterComponent::onGui() {
    if (gameObject->tag == Tag::Player)
        setPlayerGui();
    else
        setEnemyGui();

    if (DreamGame::instance->doDebugDraw) {
        DreamInspector::instance->updateCharacterGui(&hp, &armor, &damage, &rateOfFire, &shotSpeed, &knockback, gameObject);
    }
}

void CharacterComponent::setPlayerGui() {
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();

    ImVec2 pos = { 0,0 };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, guiPivot);

    ImGui::SetNextWindowSize(guiSize, ImGuiCond_Always);

    bool* open = nullptr;

    ImGui::Begin("#player", open, flags);

    ImGui::Text("PLAYER");
    ImGui::Text("Health: %.2f", hp);

    ImGui::End();


}

void CharacterComponent::setEnemyGui() {
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();

    ImVec2 pos = { winsize.x - guiSize.x, 0 };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, guiPivot);

    ImGui::SetNextWindowSize(guiSize, ImGuiCond_Always);

    bool* open = nullptr;

    ImGui::Begin("#enemy", open, flags);

    ImGui::Text("ENEMY");
    ImGui::Text("Health: %.2f", hp);

    ImGui::End();
}