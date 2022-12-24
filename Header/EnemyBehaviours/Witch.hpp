#pragma once

#include "IEnemyController.hpp"
#include "Component.hpp"

//Ranged enemy, continuously attack and move toward player until a certain distance
class Witch : public IEnemyController {
public:
    explicit Witch(GameObject* gameObject);

     void onCollisionStart(PhysicsComponent* comp) override;
    //   void onCollisionEnd(PhysicsComponent* comp) override;
    //   void update(float deltaTime) override;
    //   void onGui() override;

    void attack() override;
    void movement() override;

private:

    float idealDistance = 600.0f;
};
