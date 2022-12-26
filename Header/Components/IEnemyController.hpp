#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "sre/SpriteAtlas.hpp"

class CharacterComponent; //Forward declaration

// Interface that generalize an Enemy behaviour, to implement a basic behaviour implement at least the attack and movement function,
// if you want to implement more complex behaviours you can override update function.
// towardPlayer vector is calculated and given at the beginning of each update to ease implementation of attack and movement
class IEnemyController : public Component {
public:
    explicit IEnemyController(GameObject* gameObject);

 //   void onCollisionStart(PhysicsComponent* comp) override;
 //   void onCollisionEnd(PhysicsComponent* comp) override;
 //   void onGui() override;                                       

    void update(float deltaTime) override;

    virtual void attack() = 0;
    virtual void movement() = 0;
    virtual void setBulletSprites(sre::SpriteAtlas* atlas);

protected:

    std::shared_ptr<PhysicsComponent> physics;
    std::shared_ptr<CharacterComponent> character;
    std::shared_ptr<GameObject> player;

    glm::vec2 towardPlayer;

    friend class CharacterBuilder;
    friend class EnemyBehaviour;
};

