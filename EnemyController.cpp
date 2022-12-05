#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "EnemyController.hpp"
#include "DreamGame.hpp"
#include "SpriteAnimationComponent.hpp"

EnemyController::EnemyController(GameObject* gameObject) : Component(gameObject) {
    
}

void EnemyController::update(float deltaTime) {
    if (character->state == State::Die)
        return;

    if (character->stun)
        return;
    
    glm::vec2 movement =  player->getPosition() - gameObject->getPosition();
    float distance = glm::length(movement);
    glm::vec2 direction = glm::normalize(movement);

    if (canShoot) {
        auto anim = gameObject->getComponent<SpriteAnimationComponent>();
       /* It is not working because the sprites have a wrong pivot
       if (direction.x < 0)
            anim->displayCompleteAnimation(State::AttackLeft, 1 / character->rateOfFire, [direction, this]() {character->shoot(direction); });
        else
            anim->displayCompleteAnimation(State::AttackRight, 1 / character->rateOfFire, [direction, this]() { character->shoot(direction); });*/
        character->shoot(direction);
        if (distance < idealDistance) {
            character->changeState(State::Idle);
            return;     // Range enemies don't go toward the player until the end
        }
    }

    movement = direction * character->speed;
    physics->setLinearVelocity(movement);
    if(direction.x >= 0)
        character->changeState(State::WalkRight);
    else
        character->changeState(State::WalkLeft);
}

void EnemyController::onGui()
{
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {

}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}





