//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include <sre/Renderer.hpp>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"
#include "DreamGame.hpp"


PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) {
    
}

void PlayerController::update(float deltaTime) {

    if (stun) {
        updateStunTimeout(deltaTime);
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
        lastDirection = glm::normalize(movement);
        playerPhysics->setLinearVelocity(lastDirection * speed);
    }

    if (shooting)
        character->shot(lastDirection);
}

void PlayerController::updateStunTimeout(float deltaTime) {
    stunTimeout -= deltaTime;
    if (stunTimeout <= 0) {
        stun = false;
        stunTimeout = 0;
    }
}

void PlayerController::stunned(float stunTimeout) {
    this->stun = true;
    this->stunTimeout += stunTimeout;
}

void PlayerController::stunned(bool stun) {
    this->stun = stun;
    if (stun)
        stunTimeout += KNOCKBACK_TIME;

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
        if (bullet->getKnockback() > 0)
            stunned(true);
    }
}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}





