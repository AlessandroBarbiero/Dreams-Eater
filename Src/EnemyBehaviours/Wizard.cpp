
#include "Wizard.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

Wizard::Wizard(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void Wizard::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    // It is still not working even if the sprite pivot is right
    /*if (direction.x < 0)
         anim->displayCompleteAnimation(State::AttackLeft, 1 / character->getRateOfFire(), [direction, this]() {character->shoot(direction); });
     else
         anim->displayCompleteAnimation(State::AttackRight, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction); });*/
    character->shoot(direction);

}

void Wizard::movement()
{
    float distance = glm::length(towardPlayer);


    // Range enemies don't go toward the player until the end
    if (distance < idealDistance) {
        character->changeState(State::Idle);
        return;
    }

    glm::vec2 direction = glm::normalize(towardPlayer);

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    if (direction.x >= 0)
        character->changeState(State::WalkRight);
    else
        character->changeState(State::WalkLeft);
}
