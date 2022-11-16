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

    if (distance > idealDistance) {
        movement = glm::normalize(movement) * speed;
        physics->setLinearVelocity(movement);
    }
    if(canShoot)
        character->shot(glm::normalize(movement));
}


void EnemyController::onCollisionStart(PhysicsComponent* comp) {

}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}





