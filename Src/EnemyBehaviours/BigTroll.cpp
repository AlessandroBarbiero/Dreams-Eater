#include "BigTroll.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"

BigTroll::BigTroll(GameObject* gameObject) : IEnemyController(gameObject)
{
}

void BigTroll::onCollisionStart(PhysicsComponent* comp)
{
    if (character->getState() == State::Die)
        player->getComponent<CharacterComponent>()->showEffect(State::Victory);

}

void BigTroll::attack()
{

    glm::vec2 direction = glm::normalize(towardPlayer);

    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    //anim->displayCompleteAnimation(State::Attack1, 1 / character->getRateOfFire(), [direction, this]() { character->shoot(direction, rocks[0]); });

    anim->setFacingDirection(vectorToDirection(direction));

    //character->shoot(direction);


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

void BigTroll::setBulletSprites(sre::SpriteAtlas* atlas)
{
    rocks[0] = atlas->get("rock1.png");
    rocks[0].setOrderInBatch(Depth::Bullet);
    rocks[1] = atlas->get("rock2.png");
    rocks[1].setOrderInBatch(Depth::Bullet);
}
