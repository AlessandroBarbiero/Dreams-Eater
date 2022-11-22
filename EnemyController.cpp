#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "DreamGame.hpp"

EnemyController::EnemyController(GameObject* gameObject) : Component(gameObject) {
    
}

void EnemyController::update(float deltaTime) {

    if (character->stun)
        return;
    
    glm::vec2 movement =  player->getPosition() - gameObject->getPosition();
    float distance = glm::length(movement);
    glm::vec2 direction = glm::normalize(movement);

    if (canShoot) {
        character->shot(direction);
        if (distance < idealDistance)
            return;     // Range enemies don't go toward the player until the end
    }

    movement = direction * character->speed;
    physics->setLinearVelocity(movement);
}

void EnemyController::onGui()
{
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {
    if (comp->getGameObject()->tag == Tag::PlayerBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        float knockback = bullet->getKnockback();
        if (knockback > 0) {
            character->stunned(true);
            glm::vec2 direction = glm::normalize(gameObject->getPosition() - comp->getGameObject()->getPosition());
            physics->addImpulse(direction * knockback);
        }
    }
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}





