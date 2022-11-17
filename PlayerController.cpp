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
        if (event.type == SDL_KEYDOWN)
            up = true;
        else 
            up = false;
        
    }
    if (sym == keyDown) {
        if (event.type == SDL_KEYDOWN) 
            down = true;
        else 
            down = false;
        
    }
    if (sym == keyLeft) {
        if (event.type == SDL_KEYDOWN) 
            left = true;
        else 
            left = false;
        
    }
    if (sym == keyRight) {
        if (event.type == SDL_KEYDOWN) 
            right = true;
        else 
            right = false;
        
    }
    if (sym == keyShot) {
        if (event.type == SDL_KEYDOWN)
            shooting = true;
        else 
            shooting = false;
    }

    return false;

}

void PlayerController::onCollisionStart(PhysicsComponent* comp) {

}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}





