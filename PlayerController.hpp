#pragma once

#include "Component.hpp"

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject* gameObject);

    bool onKey(SDL_Event& event) override;

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
                                                         


private:

    std::shared_ptr<PhysicsComponent> playerPhysics;
    std::shared_ptr<SpriteComponent> playerSprite;

    float speed = 5.0;
    float radius = 10.0;

    bool fwd = false;
    bool bwd = false;
    bool left = false;
    bool right = false;

    float maximumVelocity = 2;

};
