#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "IEnemyController.hpp"
#include "CharacterComponent.hpp"

std::shared_ptr<sre::SpriteAtlas> IEnemyController::explosionsAtlas;

IEnemyController::IEnemyController(GameObject* gameObject) : Component(gameObject) { }

void IEnemyController::update(float deltaTime) {
    if (character->state == State::Die)
        return;

    towardPlayer = player->getPosition() - gameObject->getPosition();

    if (!character->stun)
        movement();
    
    attack();
}

void IEnemyController::setBulletSprites(sre::SpriteAtlas* atlas)
{
}

std::shared_ptr<sre::SpriteAtlas> IEnemyController::getExplosionsAtlas()
{
    if (!explosionsAtlas) {
        std::cout << "Loading atlas resources for explosions" << std::endl;
        std::string attackName = "Explosions";
        explosionsAtlas =
            sre::SpriteAtlas::create("Sprites/AttackAnimation/" + attackName + "/" + attackName + "_atlas.json", "Sprites/AttackAnimation/" + attackName + "/" + attackName + "_atlas.png");
    }

    return explosionsAtlas;
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





