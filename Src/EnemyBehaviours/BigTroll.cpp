#include "BigTroll.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"

BigTroll::BigTroll(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void BigTroll::attack()
{

    glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    anim->displayCompleteAnimation(State::Attack1, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction); });

    anim->setFacingDirection(vectorToDirection(direction));

    character->shoot(direction);


}

void BigTroll::movement()
{
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);
}
