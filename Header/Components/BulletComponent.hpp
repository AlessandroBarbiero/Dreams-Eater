#pragma once

#include "Component.hpp"

class BulletComponent : public Component {
public:
    explicit BulletComponent(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
                                                         
    void update(float deltaTime) override;

    // Return true if the bullet went further than the range
    bool finished();

    void destroyBullet();

    float getDamage();
    float getKnockback();

private:
    bool shrinking = false;

    float shrinkRate = 0.99;
    float damage = 1.0f;
    float range = 5.0f;
    float knockback = 0;
    glm::vec2 startingPosition;
    bool valid;

    friend class CharacterComponent;
};
