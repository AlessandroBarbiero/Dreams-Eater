#pragma once

#include "IEnemyController.hpp"
#include "Component.hpp"
#include <vector>

//TODO: not implemented yet
//
// Boss enemy, alternates between 3 attacks:
// - Melee stab
// - Magic shot
// - Thunder tempest
class Witch : public IEnemyController {
public:
    explicit Witch(GameObject* gameObject);

     void onCollisionStart(PhysicsComponent* comp) override;
    //   void onCollisionEnd(PhysicsComponent* comp) override;
    //   void update(float deltaTime) override;
    //   void onGui() override;

    void attack() override;
    void movement() override;
    void setBulletSprites(sre::SpriteAtlas* atlas) override;

private:

    void thunder();

    std::vector<sre::Sprite> thunderAnimation;
    sre::Sprite bulletSprite;
    float idealDistance = 600.0f;
};
