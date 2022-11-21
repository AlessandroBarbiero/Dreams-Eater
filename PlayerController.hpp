#pragma once

#include "Component.hpp"
#include "CharacterComponent.hpp"
constexpr auto KNOCKBACK_TIME = 0.2;

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject* gameObject);

    bool onKey(SDL_Event& event) override;

    void onCollisionStart(PhysicsComponent* comp) override;

    void onCollisionEnd(PhysicsComponent* comp) override;
    
    void update(float deltaTime) override;

    void stunned(float stunTimeout);
    void stunned(bool stun);

private:

    void updateStunTimeout(float deltaTime);

    std::shared_ptr<PhysicsComponent> playerPhysics;
    std::shared_ptr<CharacterComponent> character;

    float speed = 5.0;
    glm::vec2 lastDirection;

    // If set to true the player cannot move
    bool stun = false;

    float stunTimeout = 0;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shooting = false;

    SDL_Keycode keyUp;
    SDL_Keycode keyDown;
    SDL_Keycode keyLeft;
    SDL_Keycode keyRight;
    SDL_Keycode keyShot;

    friend class CharacterBuilder;
};
