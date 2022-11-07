//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"
#include "SpriteComponent.hpp"
#include "DreamGame.hpp"

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) {
    
    auto physicsScale = DreamGame::instance->physicsScale;

    playerPhysics = gameObject->addComponent<PhysicsComponent>();
    playerSprite = gameObject->getComponent<SpriteComponent>();

    playerPhysics->initCircle(b2_dynamicBody, 10/physicsScale, gameObject->getPosition(), 1);
    playerPhysics->fixRotation();
}

/*void PlayerController::update(float deltaTime) {
       


    glm::vec2 movement{ 0,0 };



    if (fwd) {
        movement.y++;
    }
    if (bwd) {
        movement.y--;
    }
    if (left) {
        movement.x--;
    }
    if (right) {
        movement.x++;
    }
        
    float accelerationSpeed = 0.010f;
    playerPhysics->addImpulse(movement * accelerationSpeed);

    auto linearVelocity = playerPhysics->getLinearVelocity();
    
    if (linearVelocity.x > maximumVelocity) {
        linearVelocity.x = glm::sign(linearVelocity.x) * maximumVelocity;
    }

    if (linearVelocity.y > maximumVelocity) {
        linearVelocity.y = glm::sign(linearVelocity.y) * maximumVelocity;
    }

    
    playerPhysics -> setLinearVelocity(linearVelocity);
    
}*/


bool PlayerController::onKey(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_UP:{
            if (event.type == SDL_KEYDOWN) {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ currentVelocity.x, speed });
            }
            else {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ currentVelocity.x, 0 });
            }
            
        }
        break;
        
        case SDLK_DOWN:{
            if (event.type == SDL_KEYDOWN) {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ currentVelocity.x, -speed });
            }
            else {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ currentVelocity.x, 0 });
            }
        }
        break;
    
    
        case SDLK_LEFT:{
            if (event.type == SDL_KEYDOWN) {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{-speed, currentVelocity.y });
            }
            else {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ 0, currentVelocity.y });
            }
        }
        break;

        case SDLK_RIGHT:{
            if (event.type == SDL_KEYDOWN) {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ speed, currentVelocity.y });
            }
            else {
                auto currentVelocity = playerPhysics->getLinearVelocity();
                playerPhysics->setLinearVelocity(glm::vec2{ 0, currentVelocity.y });
            }
        }
        break;
        }

    return false;

}

void PlayerController::onCollisionStart(PhysicsComponent* comp) {

}

void PlayerController::onCollisionEnd(PhysicsComponent* comp) {

}





