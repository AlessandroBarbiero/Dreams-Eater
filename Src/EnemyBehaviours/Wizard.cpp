
#include "Wizard.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"


Wizard::Wizard(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void Wizard::onCollisionStart(PhysicsComponent* comp)
{
    if (character->getState() == State::DieRight || character->getState() == State::DieLeft)
        player->getComponent<CharacterComponent>()->showEffect(State::Victory);

}

void Wizard::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    // It is still not working even if the sprite pivot is right
    if (direction.x < 0)
         anim->displayCompleteAnimation(State::AttackLeft, 1 / character->getRateOfFire(), [direction, this]() {character->shoot(direction); });
     else
         anim->displayCompleteAnimation(State::AttackRight, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction); });
    character->shoot(direction);

}

void Wizard::movement()
{
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);


    // Range enemies don't go toward the player until the end
    if (distance < idealDistance) {
        if(direction.x > 0)
            character->changeState(State::IdleRight);
        else
            character->changeState(State::IdleLeft);
        return;
    }

    

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    if (direction.x >= 0)
        character->changeState(State::WalkRight);
    else
        character->changeState(State::WalkLeft);
}
