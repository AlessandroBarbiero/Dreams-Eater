
#include "Wizard.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"


Wizard::Wizard(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void Wizard::attack()
{
	glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    anim->displayCompleteAnimation(State::Attack, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction, bulletSprite); });

    anim->setFacingDirection(vectorToDirection(direction));

    character->shoot(direction, bulletSprite);

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

void Wizard::setBulletSprites(sre::SpriteAtlas* atlas)
{
    bulletSprite = atlas->get("Bullet.png");
    bulletSprite.setOrderInBatch(Depth::Bullet);
}