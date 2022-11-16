#pragma once

#include "Component.hpp"
#include "BulletComponent.hpp"
#include <queue>

class CharacterComponent : public Component {
public:
    explicit CharacterComponent(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
    
    void update(float deltaTime) override;

    void shot(glm::vec2 direction);
    void setShotSprite(const sre::Sprite& sprite);

private:

    float life = 5.0f;          //not working
    float damage = 1.5f;
    float range = 10.0f;
    float armor = 0;            //not working
    float rateOfFire = 3.0f;    // shot per second
    float shotSpeed = 10.0f;
    float knockback = 0;        //not working

    float radius; // The radius of the physic component -> it is also the distance from which the bullet is spawned

    float shotCooldownTimer = 0;
    bool readyToShoot = true;

    std::queue<std::weak_ptr<BulletComponent>> flyingProj;

    sre::Sprite shotSprite;
    void startShotCooldown();

    friend class CharacterBuilder;
};
