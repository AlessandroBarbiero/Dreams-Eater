#include "BigTroll.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"

BigTroll::BigTroll(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void BigTroll::attack()
{

    glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    /* see Wizard
    if (direction.x < 0)
         anim->displayCompleteAnimation(State::AttackLeft, 1 / character->rateOfFire, [direction, this]() {character->shoot(direction); });
     else
         anim->displayCompleteAnimation(State::AttackRight, 1 / character->rateOfFire, [direction, this]() { character->shoot(direction); });*/
    character->shoot(direction);


}

void BigTroll::movement()
{
    float distance = glm::length(towardPlayer);


    // Range enemies don't go toward the player until the end
    //if (distance < idealDistance) {
    //    character->changeState(State::Idle);
    //    return;
    //}

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);
}
