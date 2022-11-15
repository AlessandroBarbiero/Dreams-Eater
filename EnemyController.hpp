#pragma once

#include "Component.hpp"

class EnemyController : public Component {
public:
    explicit EnemyController(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
                                                         
    void update(float deltaTime) override;

private:

    std::shared_ptr<PhysicsComponent> physics;
    std::shared_ptr<GameObject> player;

    float speed = 5.0;
    float range = 5.0f;
    float idealDistance = 600.0f;

    friend class CharacterBuilder;
};
