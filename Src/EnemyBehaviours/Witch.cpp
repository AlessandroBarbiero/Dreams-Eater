
#include "Witch.hpp"
#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"
#include <iostream>
#include "CharacterBuilder.hpp"

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
    static int Thundercount = 0;
    Thundercount++;
    if (Thundercount > 100) {
        if (glm::length(towardPlayer) < 600) {
            //STAB
            if (anim->getState() != State::Attack1 && anim->getState() != State::Attack2 && anim->getState() != State::Attack3) {
                physics->addForce((towardPlayer)*5.0f);
                anim->displayCompleteAnimation(State::Attack1, 1 / character->getRateOfFire(), [this]() {});
            }
        }
        else {
            //SHOOT
            anim->displayCompleteAnimation(State::Attack2, 1 / character->getRateOfFire(), [direction, this]() {  character->shoot(direction, bulletSprite); });
        }
    }

    if (Thundercount > 500) {
        //THUNDER
        anim->displayCompleteAnimation(State::Attack3, [this]() {  thunder(); }, true);
        Thundercount = 0;
    }
    anim->setFacingDirection(vectorToDirection(direction));

}

void Witch::movement()
{
    auto anim = gameObject->getComponent<SpriteAnimationComponent>();
    if (anim->getState() == State::Attack1) {
        return;
    }
    float distance = glm::length(towardPlayer);

    glm::vec2 direction = glm::normalize(towardPlayer);
    character->setDirection(vectorToDirection(direction));

    glm::vec2 movement = direction * character->getSpeed();
    physics->setLinearVelocity(movement);
    character->changeState(State::Walk);

}

void Witch::setBulletSprites(sre::SpriteAtlas* atlas)
{
    auto expAtlas = getExplosionsAtlas();
    bulletSprite = expAtlas->get("ExplosionBlueCircle/8.png");
    bulletSprite.setOrderInBatch(Depth::Bullet);

    thunderAnimation = CharacterBuilder::getAnimationVector("Lightning/", 17, expAtlas, Depth::Bullet);
}

void Witch::thunder()
{
    float oneOversq2 = 1 / sqrt(2);
    character->specialAttack({1,0}, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ -1,0 }, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ 0,1 }, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ 0,-1 }, 4, thunderAnimation, 8, false, false);

    character->specialAttack({ oneOversq2,-oneOversq2 }, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ -oneOversq2,-oneOversq2 }, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ -oneOversq2,oneOversq2 }, 4, thunderAnimation, 8, false, false);
    character->specialAttack({ oneOversq2,oneOversq2 }, 4, thunderAnimation, 8, false, false);
}
