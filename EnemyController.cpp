#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "PlayerController.hpp"
#include "DreamGame.hpp"

EnemyController::EnemyController(GameObject* gameObject) : Component(gameObject) {
    
}

void EnemyController::update(float deltaTime) {

    if (stun) {
        updateStunTimeout(deltaTime);
        return;
    }
    
    glm::vec2 movement =  player->getPosition() - gameObject->getPosition();
    float distance = glm::length(movement);
    glm::vec2 direction = glm::normalize(movement);

    if (canShoot) {
        character->shot(direction);
        if (distance < idealDistance)
            return;     // Range enemies don't go toward the player until the end
    }

    movement = direction * speed;
    physics->setLinearVelocity(movement);
  
}

void EnemyController::onGui()
{
}

void EnemyController::updateStunTimeout(float deltaTime) {
    stunTimeout -= deltaTime;
    std::cout << stunTimeout << std::endl;
    if (stunTimeout <= 0) {
        stun = false;
        stunTimeout = 0;
    }
}

void EnemyController::stunned(float stunTimeout) {
    this->stun = true;
    this->stunTimeout += stunTimeout;
}

void EnemyController::stunned(bool stun) {
    this->stun = stun;
    if (stun)
        stunTimeout += KNOCKBACK_TIME;

}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {
    if (comp->getGameObject()->tag == Tag::PlayerBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        if (bullet->getKnockback() > 0)
            stunned(true);
    }
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}





