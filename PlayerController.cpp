//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"
#include "DreamGame.hpp"

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) {
    
}

void PlayerController::update(float deltaTime) {
      
    glm::vec2 movement{ 0,0 };

    if (up) {
        movement.y++;
    }
    if (down) {
        movement.y--;
    }
    if (left) {
        movement.x--;
    }
    if (right) {
        movement.x++;
    }
    
    // TODO: Conditional on knockback/stun/other
    if (movement != glm::vec2(0)) {
        lastDirection = glm::normalize(movement);
        playerPhysics->setLinearVelocity(lastDirection * speed);
    }

    if (shooting)
        character->shot(lastDirection);

}


bool PlayerController::onKey(SDL_Event& event) {
    auto sym = event.key.keysym.sym;
    if (sym == keyUp) {
        up = event.type == SDL_KEYDOWN;
        
    }
    if (sym == keyDown) {
        down = event.type == SDL_KEYDOWN;
        
    }
    if (sym == keyLeft) {
        left = event.type == SDL_KEYDOWN;
        
    }
    if (sym == keyRight) {
        right = event.type == SDL_KEYDOWN;

    }
    if (sym == keyShot) {
        shooting = event.type == SDL_KEYDOWN;
     
    }

    return false;

}

void PlayerController::onCollisionStart(PhysicsComponent* comp) {

}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}





