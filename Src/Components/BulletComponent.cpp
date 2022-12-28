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
    Tag tag = comp->getGameObject()->tag;
    if (tag == Tag::EnemyBullet || tag == Tag::PlayerBullet || tag == Tag::Powerup)
        return;
    Tag myTag = gameObject->tag;
    if ((tag == Tag::Enemy && myTag == Tag::EnemyBullet )|| (tag == Tag::Player && myTag == Tag::PlayerBullet) )
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

float BulletComponent::getDamage() {
    return damage;
}

float BulletComponent::getKnockback()
{
    return knockback;
}
