#pragma once

#include "Component.hpp"
#include "CharacterComponent.hpp"

class PlayerController : public Component {
public:
    explicit PlayerController(GameObject* gameObject);

    bool onKey(SDL_Event& event) override;

    //  void onCollisionStart(PhysicsComponent* comp) override;
    //  void onCollisionEnd(PhysicsComponent* comp) override;
    
    void update(float deltaTime) override;
    void setBulletSprites(sre::SpriteAtlas* atlas);
    void overrideSuperAttack(float dmg, float cooldown, const std::vector<sre::Sprite> bulletSprites, float imageScale);
    glm::vec2 getLastDirection();

    void resetKeys() override;

    void onGui() override;

private:

    std::shared_ptr<PhysicsComponent> playerPhysics;
    std::shared_ptr<CharacterComponent> character;

    sre::Sprite bulletSprite;
    void fireOnKeyPress();

    //SuperAttack
    std::vector<sre::Sprite> superBullet;
    float superDmg;
    float superScale;
    bool superReady = false;
    float superCooldownTimer = 0;
    float superCooldown;
    void handleSuperAttack(float deltaTime);

    glm::vec2 lastDirection;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool superShoot = false;

    bool shootUp = false;
    bool shootDown = false;
    bool shootLeft = false;
    bool shootRight = false;

    SDL_Keycode keyUp;
    SDL_Keycode keyDown;
    SDL_Keycode keyLeft;
    SDL_Keycode keyRight;
    SDL_Keycode keySuperShot;

    SDL_Keycode keyShootUp;
    SDL_Keycode keyShootDown;
    SDL_Keycode keyShootLeft;
    SDL_Keycode keyShootRight;



    //===================GUI===================

    void showInitialMessage();
    void showSuperAttackBar();


    int flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar;

    std::shared_ptr<sre::Texture> barTexture;
    std::shared_ptr<sre::Texture> signTexture;
    std::shared_ptr<sre::Texture> messagePaperTexture;

    ImVec2 signSize;
    ImVec2 messageSize;
    ImVec2 barSize;
    ImVec2 menuPosition;
    ImVec2 messagePosition;
    ImVec2 uv0;
    ImVec2 uv1;
    ImVec2 itemSpacing;
    ImVec2 textSize;

    bool gone = false;

    float scaleSign = 0.3;
    float startText;
    float rounding = 5.0f;
    float scaleOffset = 1.2;

    std::string superAttackMessage;

    float initialMessageTimer = 7.0f;

    glm::vec2 spriteSize;

    char* initialMessage1 = "A swarm of enemies has come to haunt your dreams.";
    char* initialMessage2 = "Can you send them away?";


    friend class CharacterBuilder;
};
