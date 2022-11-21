#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "DreamGame.hpp"

EnemyController::EnemyController(GameObject* gameObject) : Component(gameObject) {
    
}

void EnemyController::update(float deltaTime) {
    
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


void EnemyController::onCollisionStart(PhysicsComponent* comp) {

}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}





