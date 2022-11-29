#include <SDL_events.h>
#include <iostream>
#include <sre/Renderer.hpp>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"
#include "DreamGame.hpp"
#include "SpriteAnimationComponent.hpp"

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) { }

void PlayerController::update(float deltaTime) {
    if (character->stun) {
        character->changeState(State::Idle);
        return;
    }
        
    glm::vec2 movement{ 0,0 };

    if (up) 
        movement.y++;
    if (down) 
        movement.y--;    
    if (left) 
        movement.x--;    
    if (right) 
        movement.x++;
    
    if (movement != glm::vec2(0)) {
        if (movement.x < 0)
            character->changeState(State::WalkLeft);
        else
            character->changeState(State::WalkRight);
        lastDirection = glm::normalize(movement);
        playerPhysics->setLinearVelocity(lastDirection * character->speed);
    }
    else {
        character->changeState(State::Idle);
    }

    if (shooting)
        character->shot(lastDirection);
}

bool PlayerController::onKey(SDL_Event& event) {
    auto sym = event.key.keysym.sym;

    if (sym == keyUp) 
        up = event.type == SDL_KEYDOWN;
    if (sym == keyDown) 
        down = event.type == SDL_KEYDOWN;   
    if (sym == keyLeft) 
        left = event.type == SDL_KEYDOWN;    
    if (sym == keyRight) 
        right = event.type == SDL_KEYDOWN;   
    if (sym == keyShot) 
        shooting = event.type == SDL_KEYDOWN;  

    return false;

}

void PlayerController::onCollisionStart(PhysicsComponent* comp) {
    if (comp->getGameObject()->tag == Tag::EnemyBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        float knockback = bullet->getKnockback();
        if (knockback > 0) {
            character->stunned(true);
            glm::vec2 direction = glm::normalize(gameObject->getPosition() - comp->getGameObject()->getPosition());
            playerPhysics->addImpulse(direction * knockback);
        }
    }
}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}
