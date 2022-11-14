#pragma once

#include "Component.hpp"

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject* gameObject);

    bool onKey(SDL_Event& event) override;

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
                                                         
    void update(float deltaTime) override;

private:

    std::shared_ptr<PhysicsComponent> playerPhysics;

    float speed = 5.0;
    float radius = 10.0;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    float maximumVelocity = 2;

    SDL_Keycode keyUp = SDLK_UP;
    SDL_Keycode keyDown;
    SDL_Keycode keyLeft;
    SDL_Keycode keyRight;

    friend class CharacterBuilder;
};
