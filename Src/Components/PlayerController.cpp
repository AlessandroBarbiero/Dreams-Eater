#include <SDL_events.h>
#include <iostream>
#include <sre/Renderer.hpp>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerController.hpp"
#include "DreamGame.hpp"
#include "SpriteAnimationComponent.hpp"
#include"GuiHelper.hpp"

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) { 
    
    barTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Bar.png").withFilterSampling(false).build();;

    uv0 = GuiHelper::getInstance()->uv0;
    uv1 = GuiHelper::getInstance()->uv1;

    barSize = { (float)barTexture->getWidth(), (float)barTexture->getHeight() };

    auto r = sre::Renderer::instance;
    barPosition = {barSize.y, r->getWindowSize().y - 2*barSize.y};
    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;

}

void PlayerController::update(float deltaTime) {
    if (character->state == State::Die)
        return;

    if (character->stun) {
        character->changeState(State::Idle);
        return;
    }

    glm::vec2 movement{ 0,0 };

    if (up)
        movement.y++;
    if (down)
        movement.y--;
    if (left)
        movement.x--;
    if (right)
        movement.x++;


    if (movement != glm::vec2(0)) {

        character->changeState(State::Walk);
        if (movement.x != 0) // If I am not moving on the x axis mantain old orientation
            character->setDirection(vectorToDirection(movement));

        lastDirection = glm::normalize(movement);
        playerPhysics->setLinearVelocity(lastDirection * character->speed);
    }
    else {
        character->changeState(State::Idle);
    }

    // If there is a super bullet registered
    if (superBullet.size() != 0)
        handleSuperAttack(deltaTime);

    fireOnKeyPress();
}



void PlayerController::resetKeys(){
    up  = false;
    down = false;
    left  = false;
    right = false;
    superShoot = false;

    shootUp = false;
    shootDown = false;
    shootLeft = false;
    shootRight = false;
}

bool PlayerController::onKey(SDL_Event& event) {

    auto sym = event.key.keysym.sym;

    if (sym == keyUp) 
        up = event.type == SDL_KEYDOWN;
    if (sym == keyDown) 
        down = event.type == SDL_KEYDOWN;   
    if (sym == keyLeft) 
        left = event.type == SDL_KEYDOWN;    
    if (sym == keyRight) 
        right = event.type == SDL_KEYDOWN;   

    if (sym == keySuperShot) 
        superShoot = event.type == SDL_KEYDOWN;


    if (sym == keyShootUp)
        shootUp = event.type == SDL_KEYDOWN;
    if (sym == keyShootDown)
        shootDown = event.type == SDL_KEYDOWN;
    if (sym == keyShootLeft)
        shootLeft = event.type == SDL_KEYDOWN;
    if (sym == keyShootRight)
        shootRight = event.type == SDL_KEYDOWN;

    return false;

}

// If the keys for fire are pressed generate a bullet in the right direction
void PlayerController::fireOnKeyPress() {

    glm::vec2 direction{ 0,0 };
    if (shootUp)
        direction.y++;
    if (shootDown)
        direction.y--;
    if (shootLeft)
        direction.x--;
    if (shootRight)
        direction.x++;

    if (direction != glm::vec2(0)) {
        auto anim = gameObject->getComponent<SpriteAnimationComponent>();
        direction = glm::normalize(direction);
        anim->displayCompleteAnimation(State::Attack, 1 / character->rateOfFire, [direction, this]() {character->shoot(direction, bulletSprite); });
        anim->setFacingDirection(vectorToDirection(direction), true);

        // If the animation cannot go any faster just spawn the bullets
        if (1 / character->rateOfFire < anim->getMinDuration())
            character->shoot(direction, bulletSprite);

        return;
    }
}

void PlayerController::handleSuperAttack(float deltaTime)
{
    if (superReady) {
        if (superShoot) {
            character->specialAttack(lastDirection, superDmg, superBullet, superScale);
            superReady = false;
            superCooldownTimer = 0;
        }
    }
    else {
        // Update cooldown
        superCooldownTimer += deltaTime;
        if (superCooldownTimer >= superCooldown)
            superReady = true;
    }
}



void PlayerController::setBulletSprites(sre::SpriteAtlas* atlas)
{
    bulletSprite = atlas->get("Bullet.png");
    bulletSprite.setOrderInBatch(Depth::Bullet);
}

void PlayerController::overrideSuperAttack(float dmg, float cooldown, const std::vector<sre::Sprite> bulletSprites, float imageScale)
{
    superDmg = dmg;
    superBullet = bulletSprites;
    superScale = imageScale;
    superCooldown = cooldown;
    superCooldownTimer = cooldown;
}


void PlayerController::onGui() {


    if (superCooldownTimer > 0.0) {

        ImGui::SetNextWindowSize(barSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(barPosition, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);

        bool* open = nullptr;

        ImGui::Begin("SuperAttack", open, flags);

        ImGui::Image(barTexture->getNativeTexturePtr(), barSize, uv0, uv1);

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - barSize.x - itemSpacing.x);

        auto ratio = (float)superCooldownTimer / superCooldown;

        auto uv1Fill = ImVec2{ uv1.x * ratio, uv1.y };
        auto fillSize = ImVec2{ barSize.x * ratio, barSize.y };

        ImVec4 color = { 0,ratio,0,1 };

        ImGui::Image(barTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, color);


        ImGui::End();
    }
    

}
