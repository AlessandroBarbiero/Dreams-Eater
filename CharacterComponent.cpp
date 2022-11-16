#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"
#include "DreamGame.hpp"
#include "SpriteComponent.hpp"
#include "BulletComponent.hpp"

CharacterComponent::CharacterComponent(GameObject* gameObject) : Component(gameObject) {
    
}

//Look for the first projectile shot and check if it has to be destroyed
void CharacterComponent::update(float deltaTime) {
    if (!readyToShoot) {
        shotCooldownTimer += deltaTime;
        if (shotCooldownTimer >= 1 / rateOfFire)
            readyToShoot = true;
    }

    if (flyingProj.empty())
        return;

    auto lastProj = flyingProj.front();
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


void CharacterComponent::onCollisionStart(PhysicsComponent* comp) {
    if (gameObject->tag == Tag::Player && comp->getGameObject()->tag == Tag::Enemy) {
        gameObject->destroy();
        DreamGame::instance->gameOver();
    }
}

void CharacterComponent::onCollisionEnd(PhysicsComponent* comp) {

}

// Shoot a bullet in the passed direction that is destroyed after it travels for range distance, this method is subjected to the stats:
// - damage
// - shotSpeed
// - range
// - rateOfFire
void CharacterComponent::shot(glm::vec2 direction) {
    if (!readyToShoot) 
        return; // cooldown is not finished
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto shot = game->createGameObject();
    shot->name = "playerBullet";
    shot->tag = Tag::Bullet;

    shot->setPosition(gameObject->getPosition()/ physicsScale + direction * (radius*2));

    auto spriteComp = shot->addComponent<SpriteComponent>();
    shotSprite.setScale(glm::vec2(damage));
    spriteComp->setSprite(shotSprite);

    auto shotPhy = shot->addComponent<PhysicsComponent>();
    float radius = shotSprite.getSpriteSize().x * shotSprite.getScale().x / (2 * physicsScale);
    shotPhy->initCircle(b2_dynamicBody, radius, shot->getPosition(), 1);
    shotPhy->setLinearVelocity(direction * shotSpeed);
    shotPhy->setSensor(true);

    auto bullet = shot->addComponent<BulletComponent>();
    bullet->startingPosition = gameObject->getPosition();
    bullet->range = range;
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





