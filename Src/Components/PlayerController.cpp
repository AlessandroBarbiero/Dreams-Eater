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
    
    barTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Bar.png").withFilterSampling(false).build();
    signTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Sign.png").withFilterSampling(false).build();
    messagePaperTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "MessagePaper.png").withFilterSampling(false).build();

    uv0 = GuiHelper::getInstance()->uv0;
    uv1 = GuiHelper::getInstance()->uv1;

    barSize = { (float)barTexture->getWidth(), (float)barTexture->getHeight() };
    signSize = { (float)signTexture->getWidth() * scaleSign, (float)signTexture->getHeight() * scaleSign};

    auto scale = 0.95f;
    messageSize = { (float)messagePaperTexture->getWidth() * scale, (float)messagePaperTexture->getHeight() * scale };
    
    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;

    auto r = sre::Renderer::instance;
    menuPosition = { 0.0f, r->getWindowSize().y - signSize.y };

    spriteSize = gameObject->getComponent<SpriteComponent>()->getSprite().getSpriteSize();

    
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

    fireOnKeyPress();

    // If there is a super bullet registered
    if (superBullet.size() != 0)
        handleSuperAttack(deltaTime);

    if (initialMessageTimer > 0.0) {
        initialMessageTimer -= deltaTime;
    }

    //Check if the player has exited the first startingRoom in the first level
    if (!gone)
        gone = DreamGame::instance->level->startRoom != DreamGame::instance->level->currentRoomIndex;

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

        lastDirection = direction;

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
        
        if (superCooldownTimer >= superCooldown)
            superReady = true;
        else 
            // Update cooldown
            superCooldownTimer += deltaTime;
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

glm::vec2 PlayerController::getLastDirection()
{
    return lastDirection;
}

void PlayerController::showInitialMessage(){
    bool* open = nullptr;

    GuiHelper::getInstance()->setZeroPadding();
    ImGui::PushFont(GuiHelper::getInstance()->fontFunny35);

    auto position = DreamGame::instance->camera->getWindowCoordinates(glm::vec3(gameObject->getPosition(), 0.0));

    messagePosition = ImVec2{ position.x - messageSize.x / 2.0f , position.y + spriteSize.y / 5.0f + messageSize.y / 1.3f };

    ImGui::SetNextWindowPos(messagePosition, ImGuiCond_Always);
    ImGui::SetNextWindowSize(messageSize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    auto textSize1 = ImGui::CalcTextSize(initialMessage1);
    auto textSize2 = ImGui::CalcTextSize(initialMessage2);

    ImGui::Begin("Sign", open, flags);
    ImGui::Image(messagePaperTexture->getNativeTexturePtr(), messageSize);
    
    auto offset = (messageSize.y - textSize1.y - textSize2.y) / 2.0f;
    
    ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize1.x), offset  });
    ImGui::Text(initialMessage1);
    
    ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize2.x),messageSize.y - offset - textSize2.y });
    ImGui::Text(initialMessage2);


    ImGui::PopFont();
    ImGui::PopStyleVar();
    ImGui::End();

    

}

void PlayerController::showSuperAttackBar(){
    bool* open = nullptr;
    auto ratio = (float)superCooldownTimer / superCooldown;

    ImGui::SetNextWindowSize(signSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(menuPosition, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    GuiHelper::getInstance()->setZeroPadding();
    ImGui::Begin("SuperAttack", open, flags);
    ImGui::PushFont(GuiHelper::getInstance()->fontFunny20);

    superAttackMessage = "Press " + SuperAttackToString.at(keySuperShot);
    textSize = ImGui::CalcTextSize(superAttackMessage.c_str());

    ImGui::GetWindowDrawList()->AddImageRounded(signTexture->getNativeTexturePtr(), menuPosition,
        { menuPosition.x + signSize.x, menuPosition.y + signSize.y }, uv0, uv1, IM_COL32(255, 255, 255, 255), rounding);

    if (ratio > 0.99) {

        auto offset = (signSize.y - barSize.y - itemSpacing.y - textSize.y) / scaleOffset;

        ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize.x), offset / 2.0f });

        auto color = ImVec4(0, 0, 0, 1);

        ImGui::TextColored(color, superAttackMessage.c_str());

        ImGui::SetCursorPosY(signSize.y - offset / 2.0f - barSize.y);
    }

    else {
        ImGui::SetCursorPosY(signSize.y / 2.0f - barSize.y / 2.0f);
    }

    ImGui::SetCursorPosX(signSize.x / 2.0f - barSize.x / 2.0f);


    ImGui::Image(barTexture->getNativeTexturePtr(), barSize, uv0, uv1);

    ImGui::SameLine();

    auto uv1Fill = ImVec2{ uv1.x * ratio, uv1.y };
    auto fillSize = ImVec2{ barSize.x * ratio, barSize.y };

    ImVec4 barColor = { 0, ratio, 0, 0.5f + 0.5f * ratio };

    ImGui::SetCursorPosX(signSize.x / 2.0f - barSize.x / 2.0f);

    ImGui::Image(barTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, barColor);

    ImGui::PopStyleVar();
    ImGui::PopFont();

    ImGui::End();
}






void PlayerController::onGui() {

    if (DreamGame::instance->doDebugDraw) {

        auto title = "Player Controller GUI - " + gameObject->name;

        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        if (ImGui::CollapsingHeader(title.c_str())) {
            ImGui::DragFloat(std::string("TextOffset##").append(gameObject->name).c_str(), &scaleOffset, 0.1f, 0.1, 3);
        }
        ImGui::End();
    }

    

    if (initialMessageTimer > 0.0 && !gone) 
            showInitialMessage();
    


    if (superCooldownTimer > 0.0)
            showSuperAttackBar();
    
    

}
