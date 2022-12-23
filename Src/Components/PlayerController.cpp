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
    if (character->state == State::DieRight || character->state == State::DieLeft)
        return;

    if (character->stun) {
        character->changeState(State::IdleRight); //TODO set right direction
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
        if (lastDirection.x >= 0)
            character->changeState(State::IdleRight);
        else
            character->changeState(State::IdleLeft);
    }

    if (shooting)
        character->shoot(lastDirection);
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
