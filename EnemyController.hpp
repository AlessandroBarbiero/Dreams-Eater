#pragma once

#include "Component.hpp"
#include "CharacterComponent.hpp"

class EnemyController : public Component {
public:
    explicit EnemyController(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
                                                         
    void update(float deltaTime) override;

private:

    std::shared_ptr<PhysicsComponent> physics;
    std::shared_ptr<CharacterComponent> character;

    std::shared_ptr<GameObject> player;

    bool canShoot = true; //Set it to false for melee enemies
    float speed = 5.0;
    float range = 5.0f;
    float idealDistance = 600.0f;

    friend class CharacterBuilder;
};
