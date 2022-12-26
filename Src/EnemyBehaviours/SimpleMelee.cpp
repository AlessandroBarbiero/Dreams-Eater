
#include "SimpleMelee.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"


SimpleMelee::SimpleMelee(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void SimpleMelee::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    anim->displayCompleteAnimation(State::Attack, 1 / character->getRateOfFire(), [direction, this]() { /*character->shoot(direction);*/ });

    anim->setFacingDirection(vectorToDirection(direction));

    //character->shoot(direction);

}

void SimpleMelee::movement()
{
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);

}