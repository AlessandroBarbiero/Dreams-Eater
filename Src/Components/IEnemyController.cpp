#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "IEnemyController.hpp"
#include "CharacterComponent.hpp"

IEnemyController::IEnemyController(GameObject* gameObject) : Component(gameObject) { }

void IEnemyController::update(float deltaTime) {
    if (character->state == State::DieRight || character->state == State::DieLeft)
        return;

    towardPlayer = player->getPosition() - gameObject->getPosition();

    if (!character->stun)
        movement();
    
    attack();
}


//void IEnemyController::onGui()
//{
//}
//
//void IEnemyController::onCollisionStart(PhysicsComponent* comp) {
//
//}
//
//void IEnemyController::onCollisionEnd(PhysicsComponent* comp) {
//
//}





