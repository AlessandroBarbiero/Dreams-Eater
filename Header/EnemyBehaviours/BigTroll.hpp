#pragma once

#include "IEnemyController.hpp"
#include "Component.hpp"

//TODO: not implemented yet
//
//Boss enemy, act within states alternating between melee attacks and circle spawning static rocks
class BigTroll : public IEnemyController {
public:
    explicit BigTroll(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;
    //   void onCollisionEnd(PhysicsComponent* comp) override;
    //   void update(float deltaTime) override;
    //   void onGui() override;

    void attack() override;
    void movement() override;
    void setBulletSprites(sre::SpriteAtlas* atlas) override;
private:

    sre::Sprite rocks[2];
};
