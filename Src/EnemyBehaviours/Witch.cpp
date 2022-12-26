
#include "Witch.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"


Witch::Witch(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void Witch::onCollisionStart(PhysicsComponent* comp)
{
    if (character->getState() == State::Die)
        player->getComponent<CharacterComponent>()->showEffect(State::Victory);

}

void Witch::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    anim->displayCompleteAnimation(State::Attack1, 1 / character->getRateOfFire(), [direction, this]() { /*character->shoot(direction);*/ });

    anim->setFacingDirection(vectorToDirection(direction));

 //   character->shoot(direction);

}

void Witch::movement()
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

void Witch::setBulletSprites(sre::SpriteAtlas* atlas)
{
    //bulletSprite = atlas->get("Bullet.png");
    //bulletSprite.setOrderInBatch(Depth::Bullet);
}
