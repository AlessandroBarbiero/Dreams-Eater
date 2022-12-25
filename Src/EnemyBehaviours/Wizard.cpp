
#include "Wizard.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"


Wizard::Wizard(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void Wizard::onCollisionStart(PhysicsComponent* comp)
{
    if (character->getState() == State::Die)
        player->getComponent<CharacterComponent>()->showEffect(State::Victory);

}

void Wizard::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    anim->displayCompleteAnimation(State::Attack, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction); });

    anim->setFacingDirection(vectorToDirection(direction));

    character->shoot(direction);

}

void Wizard::movement()
{
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    // Range enemies don't go toward the player until the end
    if (distance < idealDistance) {
        character->changeState(State::Idle);
        return;
    }

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);

}