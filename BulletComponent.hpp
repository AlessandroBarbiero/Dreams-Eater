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

private:

    float damage = 1.0f;
    float range = 5.0f;
    glm::vec2 startingPosition;
    bool valid;

    friend class CharacterComponent;
};
