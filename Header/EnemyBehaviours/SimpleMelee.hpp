#pragma once

#include "IEnemyController.hpp"
#include "Component.hpp"

//Melee enemy, move toward the player and attack when on range
class SimpleMelee : public IEnemyController {
public:
    explicit SimpleMelee(GameObject* gameObject);

    //   void onCollisionStart(PhysicsComponent* comp) override;
    //   void onCollisionEnd(PhysicsComponent* comp) override;
    //   void update(float deltaTime) override;
    //   void onGui() override;

    void attack() override;
    void movement() override;

private:
    bool isAttacking = false;
};
