#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "BulletComponent.hpp"
#include "DreamGame.hpp"
#include "SpriteComponent.hpp"

BulletComponent::BulletComponent(GameObject* gameObject) : Component(gameObject) {
    valid = true;
}

void BulletComponent::update(float deltaTime) {
    if (shrinking) {
        auto phy = gameObject->getComponent<PhysicsComponent>();
        phy->setRadius(phy->getRadius() * shrinkRate);
        auto spr = gameObject->getComponent<SpriteComponent>();
        auto sprite = spr->getSprite();
        sprite.setScale(sprite.getScale() * shrinkRate);
        spr->setSprite(sprite);
    }
}

// Destroy the bullet if it collides with something that is not another bullet
void BulletComponent::onCollisionStart(PhysicsComponent* comp) {
    if (comp->getGameObject()->tag == Tag::Bullet)
        return;
    destroyBullet();
}

void BulletComponent::onCollisionEnd(PhysicsComponent* comp) {

}

// If the bullet was already destroyed it is a no-op
void BulletComponent::destroyBullet() {
    if (valid) {
        gameObject->destroy();
        valid = false;
    }
}

// Return true if the bullet has traveled enough distance compared to the range or it has been already destroyed on collision
bool BulletComponent::finished() {
    if (!valid)
        return true;
    float travelDistance = glm::distance(gameObject->getPosition(), startingPosition);
    return travelDistance > range * DreamGame::instance->physicsScale;
}