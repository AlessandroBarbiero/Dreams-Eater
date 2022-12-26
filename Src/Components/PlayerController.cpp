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
    if (character->state == State::Die)
        return;

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

        character->changeState(State::Walk);
        character->setDirection(vectorToDirection(movement));
        lastDirection = glm::normalize(movement);
        playerPhysics->setLinearVelocity(lastDirection * character->speed);
    }
    else {
        character->changeState(State::Idle);
    }

    if (shooting)
        character->shoot(lastDirection);
}

void PlayerController::resetKeys(){
    up  = false;
    down = false;
    left  = false;
    right = false;
    shooting = false;
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

}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}
