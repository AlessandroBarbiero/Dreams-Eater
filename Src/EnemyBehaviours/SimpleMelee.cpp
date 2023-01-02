
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
    if (glm::length(towardPlayer) < 600) {
        if (!isAttacking) {
            physics->addForce((towardPlayer));
            anim->displayCompleteAnimation(State::Attack, 1.0f, [this]() {isAttacking = false; physics->addForce(-(towardPlayer));});
            isAttacking = true;
        }
    }

    anim->setFacingDirection(vectorToDirection(direction));

}

void SimpleMelee::movement()
{
    if (isAttacking) {
        return;
    }
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);

}
