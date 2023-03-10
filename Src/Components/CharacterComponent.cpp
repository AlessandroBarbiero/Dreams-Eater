#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"
#include "DreamGame.hpp"
#include "SpriteComponent.hpp"
#include "BulletComponent.hpp"
#include "SpriteAnimationComponent.hpp"
#include "PowerupComponent.hpp"
#include "GuiHelper.hpp"
#include "PlayerController.hpp"
#include <math.h>
#include "CharacterBuilder.hpp"
#define KNOCKBACK_SCALE 10

std::shared_ptr<sre::SpriteAtlas> CharacterComponent::effectAtlas;

CharacterComponent::CharacterComponent(GameObject* gameObject) : Component(gameObject) {
    if (CharacterComponent::effectAtlas == nullptr) {
        std::cout << "Loading effect atlas" << std::endl;
        CharacterComponent::effectAtlas = sre::SpriteAtlas::create("Sprites/SpecialEffects/SpecialEffects_atlas.json", "Sprites/SpecialEffects/SpecialEffects_atlas.png");
    }

    initSpecialEffectObject();

    //GUI
    heartTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Heart.png").withFilterSampling(false).build();
    messagePaperTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "MessagePaper.png").withFilterSampling(false).build();
    signTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "GreyPaper.png").withFilterSampling(false).build();

    spriteSize = gameObject->getComponent<SpriteComponent>()->getSprite().getSpriteSize();

    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;
    
    uv0 = GuiHelper::getInstance()->uv0;
    uv1 = GuiHelper::getInstance()->uv1;

}

void CharacterComponent::initSpecialEffectObject() {
    auto specialEffectsObj = DreamGame::instance->currentScene->createGameObject();
    specialEffectsObj->name = "SpecialEffectsOf" + gameObject->name;

    auto sprComp = specialEffectsObj->addComponent<SpriteComponent>();
    sprComp->deactivate();
    specialEffects = specialEffectsObj->addComponent<SpriteAnimationComponent>();
    // The special effect should be visible only when necessary
    specialEffects->deactivate();

    static std::vector<sre::Sprite> hitAnim = CharacterBuilder::getAnimationVector("Hit/", 11, CharacterComponent::effectAtlas, Depth::Effect);
    static std::vector<sre::Sprite> victoryAnim = CharacterBuilder::getAnimationVector("Victory/", 16, CharacterComponent::effectAtlas, Depth::Effect);
    static std::vector<sre::Sprite> itemAnim = CharacterBuilder::getAnimationVector("Item/", 9, CharacterComponent::effectAtlas, Depth::Effect);
    static std::vector<sre::Sprite> chargeAnim = CharacterBuilder::getAnimationVector("Charge/", 12, CharacterComponent::effectAtlas, Depth::Effect);

    specialEffects->addAnimationSequence(State::Victory,    Direction::RIGHT,       victoryAnim);
    specialEffects->addAnimationSequence(State::Victory,    Direction::LEFT,        victoryAnim);
    specialEffects->addAnimationSequence(State::Hit,        Direction::RIGHT,       hitAnim);
    specialEffects->addAnimationSequence(State::Hit,        Direction::LEFT,        hitAnim);
    specialEffects->addAnimationSequence(State::Item,       Direction::RIGHT,       itemAnim);
    specialEffects->addAnimationSequence(State::Item,       Direction::LEFT,        itemAnim);
    specialEffects->addAnimationSequence(State::Charge,     Direction::RIGHT,       chargeAnim);
    specialEffects->addAnimationSequence(State::Charge,     Direction::LEFT,        chargeAnim);
    specialEffects->setBaseAnimationTime(0.1f);

    gameObject->addChild(specialEffectsObj.get());
}

void CharacterComponent::update(float deltaTime) {
    if(stun)
        updateStunTimeout(deltaTime);

    checkRateOfFire(deltaTime);
   
    updateFlyingProj();

    if (powerupMessageTimeOut > 0) {
        powerupMessageTimeOut -= deltaTime;
    }

}

void CharacterComponent::changeState(State newState)
{
    state = newState;
}

Direction CharacterComponent::getDirection()
{
    return facingDirection;
}

void CharacterComponent::setDirection(Direction newFacingDirection)
{
    facingDirection = newFacingDirection;
}

// Update readyToShoot variable based on rateOfFire and cooldownTimer
void CharacterComponent::checkRateOfFire(float deltaTime) {
    if (!readyToShoot) {
        shotCooldownTimer += deltaTime;
        if (shotCooldownTimer >= 1 / rateOfFire)
            readyToShoot = true;
    }
}

//Look for the first projectile shot still in the scene and check if it has to be destroyed (reached the range)
void CharacterComponent::updateFlyingProj() {
    if (flyingProj.empty())
        return;

    auto& lastProj = flyingProj.front();
    // Remove the projectiles collided and already destroyed
    while (lastProj.expired()) {
        flyingProj.pop();
        if (flyingProj.empty())
            return;
        lastProj = flyingProj.front();
    }

    if (std::shared_ptr<BulletComponent> bullet = lastProj.lock()) {
        if (bullet->finished()) {
            flyingProj.pop();
            bullet->destroyBullet();
        }
    }
    else
        flyingProj.pop();
}

void CharacterComponent::onCollisionStart(PhysicsComponent* comp) {
    if (hp <= 0)
        return;
    Tag myTag = gameObject->tag;
    Tag hisTag = comp->getGameObject()->tag;
    if (myTag == Tag::Player && hisTag == Tag::Enemy) {
        auto enemy = comp->getGameObject()->getComponent<CharacterComponent>();
        inflictDamage(enemy->damage);
    }
    if (myTag == Tag::Player && hisTag == Tag::EnemyBullet ||
        myTag == Tag::Enemy && hisTag == Tag::PlayerBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        applyKnockback(bullet);
        inflictDamage(bullet->getDamage());
    }
}

void CharacterComponent::applyKnockback(std::shared_ptr<BulletComponent> bullet) {
    if (bullet->knockback > 0) {
        stunned(true);
        glm::vec2 direction = glm::normalize(gameObject->getPosition() - bullet->getGameObject()->getPosition());
        auto phys = gameObject->getComponent<PhysicsComponent>();
        phys->addImpulse(direction * bullet->knockback);
    }
}

void CharacterComponent::inflictDamage(float damage) {
    float realDamage = damage - armor;
    if (realDamage > 0) {
        hp -= realDamage;
        if (hp <= 0) {
            state = State::Die;
            auto anim = gameObject->getComponent<SpriteAnimationComponent>();
            anim->displayCompleteAnimation(state, [this]() {die(); }, true);
        }
        else
            showEffect(State::Hit);
    }
}

void CharacterComponent::die() {
    gameObject->destroy();
    if (gameObject->tag == Tag::Player)
        DreamGame::instance->gameOver();
    else if (gameObject->tag == Tag::Enemy) {
        DreamGame::instance->countEnemiesKilled++;
        onDeath(getGameObject());
        onDeath = nullptr;
    }
}


void CharacterComponent::updateStunTimeout(float deltaTime) {
    stunTimeout -= deltaTime;
    if (stunTimeout <= 0) {
        stun = false;
        stunTimeout = 0;
    }
}

void CharacterComponent::stunned(float stunTimeout) {
    this->stun = true;
    this->stunTimeout += stunTimeout;
}

void CharacterComponent::stunned(bool stun) {
    this->stun = stun;
    if (stun)
        stunTimeout = KNOCKBACK_TIME;
}

State CharacterComponent::getState()
{
    return state;
}


// Shoot a bullet in the passed direction that is destroyed after it travels for range distance, this method is subjected to the stats:
// - damage
// - shotSpeed
// - range
// - rateOfFire
void CharacterComponent::shoot(glm::vec2 direction, const sre::Sprite& bulletSprite) {
    if (!readyToShoot) 
        return; // cooldown is not finished

    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto shot = game->currentScene->createGameObject();
    shot->name = "Bullet";
    if (gameObject->tag == Tag::Player)
        shot->tag = Tag::PlayerBullet;
    else if (gameObject->tag == Tag::Enemy)
        shot->tag = Tag::EnemyBullet;
    else
        shot->tag = Tag::Bullet;

    auto spriteComp = shot->addComponent<SpriteComponent>();
    spriteComp->setSprite(bulletSprite);
    float bulletRadius = bulletSprite.getSpriteSize().x / (2 * physicsScale);

    glm::vec2 position = gameObject->getPosition() / physicsScale + direction * (radius * gameObject->getScale() + bulletRadius*damage);
    shot->setPosition(position * physicsScale);
    shot->setRotation(glm::atan(direction.y, direction.x));

    auto shotPhy = shot->addComponent<PhysicsComponent>();
    shotPhy->initCircle(b2_dynamicBody, bulletRadius, position, 1);
    shotPhy->setLinearVelocity(direction * shotSpeed);
    shotPhy->setSensor(true);

    shot->setScale(damage);

    auto bullet = shot->addComponent<BulletComponent>();
    bullet->startingPosition = shot->getPosition();
    bullet->range = range;
    bullet->damage = damage;
    bullet->knockback = knockback * KNOCKBACK_SCALE;
    std::weak_ptr<BulletComponent> weakBullet = bullet;
    flyingProj.push(weakBullet);
    startShotCooldown();
}


void CharacterComponent::specialAttack(glm::vec2 givenDirection, float dmg, const std::vector<sre::Sprite> bulletSprites, float imageScale, bool displayEffect, bool rotateBullet)
{

    std::function<void()> callback = [=]() {
        glm::vec2 direction;
        if (gameObject->tag == Tag::Player)
            direction = gameObject->getComponent<PlayerController>()->getLastDirection();
        else
            direction = givenDirection;
        auto game = DreamGame::instance;
        auto physicsScale = game->physicsScale;

        auto specialShot = game->currentScene->createGameObject();
        specialShot->name = "SpecialBullet";
        if (gameObject->tag == Tag::Player)
            specialShot->tag = Tag::PlayerBullet;
        else if (gameObject->tag == Tag::Enemy)
            specialShot->tag = Tag::EnemyBullet;
        else
            specialShot->tag = Tag::Bullet;

        auto spriteComp = specialShot->addComponent<SpriteComponent>();
        spriteComp->setSprite(bulletSprites[0]);
        float bulletRadius = bulletSprites[0].getSpriteSize().x / (2 * physicsScale);

        glm::vec2 position = gameObject->getPosition() / physicsScale + direction * (radius * gameObject->getScale() + bulletRadius * imageScale);
        specialShot->setPosition(position * physicsScale);
        if (rotateBullet)
            specialShot->setRotation(glm::atan(direction.y, direction.x));

        auto animComp = specialShot->addComponent<SpriteAnimationComponent>();
        animComp->showFixedAnimation(bulletSprites);
        animComp->setBaseAnimationTime(0.1f);

        auto shotPhy = specialShot->addComponent<PhysicsComponent>();
        shotPhy->initCircle(b2_dynamicBody, bulletRadius, position, 1);
        shotPhy->setLinearVelocity(direction * shotSpeed);
        shotPhy->setSensor(true);

        specialShot->setScale(imageScale);

        auto bullet = specialShot->addComponent<BulletComponent>();
        bullet->startingPosition = specialShot->getPosition();
        // Infitity range
        bullet->range = 50;
        bullet->damage = dmg;
        bullet->knockback = 1.5 * KNOCKBACK_SCALE;
        std::weak_ptr<BulletComponent> weakBullet = bullet;
        specialProj.push(weakBullet);

    };
    if (displayEffect)
        specialEffects->displayOnce(State::Charge, callback);
    else
        callback();
}

void CharacterComponent::startShotCooldown() {
    readyToShoot = false;
    shotCooldownTimer = 0;
}

void CharacterComponent::showEffect(State effect, float animTime) {
    specialEffects->displayOnce(effect, animTime);
    if (effect == State::Item) {
        powerupMessageTimeOut = messageTime;
    }
}



// %%%%%%%%%%%%%%%%% GUI %%%%%%%%%%%%%%%%%%%%%%%%

void CharacterComponent::onGui() {
    if (gameObject->tag == Tag::Player) {
        showLife();
        if (powerupMessageTimeOut > 0) {
            displayPowerupMessage();
        }
    }
        

    if (DreamGame::instance->doDebugDraw) {

        auto title = "Character Component - " + gameObject->name;
        auto titleGui = "Character Component GUI- " + gameObject->name;

        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        if (ImGui::CollapsingHeader(title.c_str())) {
            ImGui::DragFloat(std::string("HP ##").append(gameObject->name).c_str(),          &hp, 0.1f, 0, 24);
            ImGui::DragFloat(std::string("Armor##").append(gameObject->name).c_str(),        &armor, 0.1f, 0, 5);
            ImGui::DragFloat(std::string("Damage##").append(gameObject->name).c_str(),       &damage, 0.1f, 0, 5);
            ImGui::DragFloat(std::string("Rate Of Fire##").append(gameObject->name).c_str(), &rateOfFire, 0.1f, 0.5f, 10);
            ImGui::DragFloat(std::string("Shoot Speed##").append(gameObject->name).c_str(),  &shotSpeed, 0.1f, 0, 10);
            ImGui::DragFloat(std::string("Knockback##").append(gameObject->name).c_str(),    &knockback, 0.1f, 0, 10);
            
            ImGui::Text("Scale % .2f", gameObject->getScale());

            if (ImGui::Button(std::string("Scale+##").append(gameObject->name).c_str(), { 100,25 })) {
                gameObject->setScale(gameObject->getScale() + 0.1f);
            }
            if (ImGui::Button(std::string("Scale-##").append(gameObject->name).c_str(), { 100,25 })) {
                gameObject->setScale(gameObject->getScale() - 0.1f);
            }
        }
            if (ImGui::CollapsingHeader(titleGui.c_str())) {
                ImGui::DragFloat(std::string("TextOffset##").append(gameObject->name).c_str(), &textOffset, 1.0f, 10, 100);
                ImGui::DragFloat(std::string("HeartOffsets##").append(gameObject->name).c_str(), &heartOffset, 1.0f, 10, 100);
                ImGui::DragInt(std::string("HeartInRow##").append(gameObject->name).c_str(), &heartInRow, 0.1f, 1, 10);
        }
        ImGui::End();
    }
}



void CharacterComponent::showLife() {

    int hpInt = (int)hp;
    auto decimal = hp - hpInt;

    int intDecimal = ceil(decimal);

    auto scaleX = 0.05f;
    auto scaleY = 0.05f;

    int rows = hpInt > maxHp ? (maxHp + intDecimal) / heartInRow + 1 : (hpInt + intDecimal) / heartInRow + 1;

    rows = rows > ceil(maxHp / heartInRow) ? ceil(maxHp / heartInRow) : rows;

    auto widthHeartRow = 0.0f;
    auto heightHeartRow = heartSize.y * rows + heartOffset * 2 + itemSpacing.y * (rows - 1);

    
    if (hpInt + decimal > heartInRow) {
        //width is 2 times the offset (distance from borders) + heartInRow times the heartSize and heartInRow - 1 times the spacing between each of them
        widthHeartRow = heartOffset * 2 + heartInRow * heartSize.x + (heartInRow - 1) * itemSpacing.x;
    }
    else {
        if (hpInt + intDecimal >= defaultHp) {
            //width and heght are the same as before but we consider the actual number of hearts
            widthHeartRow = heartOffset * 2 + (hpInt + intDecimal) * heartSize.x + (hpInt + intDecimal - 1) * itemSpacing.x;
        }
        else {
            widthHeartRow = heartOffset * 2 + defaultHp * heartSize.x + (defaultHp - 1) * itemSpacing.x;
        }
    }

    while (scaleX * signTexture->getWidth() < widthHeartRow) {
        scaleX += 0.01f;
    }

    while (scaleY * signTexture->getHeight() < heightHeartRow) {
        scaleY += 0.01f;
    }

    menuSize = ImVec2(scaleX * signTexture->getWidth(), scaleY * signTexture->getHeight());

    ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos({ menuOffset, menuOffset }, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    GuiHelper::getInstance()->setZeroPadding();

    bool* open = nullptr;

    ImGui::Begin("#player", open, flags);

    ImGui::Image(signTexture->getNativeTexturePtr(), menuSize, uv0, uv1);

    ImGui::SetCursorPos({heartOffset, heartOffset });

    for (int i = 0; i < hpInt && i < maxHp; i++) {
        ImGui::Image(heartTexture->getNativeTexturePtr(), heartSize, uv0, uv1,  RED);
        if ((i+1) % heartInRow != 0)
            ImGui::SameLine();
        else
            ImGui::SetCursorPosX(heartOffset);
    }

    if (decimal > 0.0 && hpInt < maxHp) {
        ImGui::Image(heartTexture->getNativeTexturePtr(), heartSize, uv0, uv1, WHITE);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - heartSize.x - itemSpacing.x);

        auto uv1Fill = ImVec2{ uv1.x * decimal, uv1.y };
        auto fillSize = ImVec2{ heartSize.x * decimal, heartSize.y };

        ImGui::Image(heartTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, RED);
        ImGui::SameLine();
    }

    

    if (hpInt + intDecimal < defaultHp) {

        ImGui::SetCursorPosX({ (hpInt + intDecimal) * heartSize.x + (hpInt + intDecimal) * itemSpacing.x + heartOffset });

        for (int i = hpInt + 1 + intDecimal; i <= defaultHp; i++) {
            ImGui::Image(heartTexture->getNativeTexturePtr(), heartSize, uv0, uv1, WHITE);
            ImGui::SameLine();
        }
        
        
    }

    
  
    ImGui::End();
    ImGui::PopStyleVar();
}

void CharacterComponent::displayPowerupMessage(){
    bool* open = nullptr;

    GuiHelper::getInstance()->setZeroPadding();
    ImGui::PushFont(GuiHelper::getInstance()->fontMessage);

    auto position = DreamGame::instance->camera->getWindowCoordinates(glm::vec3(gameObject->getPosition(), 0.0));

    

    auto powerupComponent = gameObject->getComponent<PowerupComponent>();
    auto lastPowerup = powerupComponent->getPowerups()[powerupComponent->getPowerups().size() - 1];
    std::string message = lastPowerup->getMessage();

    auto textSize = ImGui::CalcTextSize(message.c_str());

    auto scale = 0.1f;

    while (textSize.x + textOffset > messagePaperTexture->getWidth() * scale || textSize.y + textOffset > messagePaperTexture->getHeight() * scale) {
        scale += 0.1f;
    }

    auto signSize = ImVec2{ messagePaperTexture->getWidth() * scale, messagePaperTexture->getHeight() * scale };
    
    auto powerupMessagePosition = ImVec2{ position.x - signSize.x / 2.0f , position.y - spriteSize.y / 5.0f - signSize.y / 1.3f};

    ImGui::SetNextWindowPos(powerupMessagePosition, ImGuiCond_Always);
    ImGui::SetNextWindowSize(signSize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Sign", open, flags);
    ImGui::Image(messagePaperTexture->getNativeTexturePtr(), signSize);

    
    
    ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2.0f - textSize.x / 2.0f, ImGui::GetWindowSize().y / 2.0f - textSize.y / 2.0f });
    ImGui::TextColored(GuiHelper::getInstance()->BLACK, message.c_str());
    ImGui::End();
    ImGui::PopFont();
    ImGui::PopStyleVar();
    
    
}

//void CharacterComponent::setEnemyGui() {
//    auto r = sre::Renderer::instance;
//    auto winsize = r->getWindowSize();
//
//    ImVec2 pos = { winsize.x - guiSize.x, 0 };
//
//    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, guiPivot);
//
//    ImGui::SetNextWindowSize(guiSize, ImGuiCond_Always);
//
//    bool* open = nullptr;
//
//    ImGui::Begin("#enemy", open, flags);
//
//    ImGui::Text("ENEMY");
//    ImGui::Text("Health: %.2f", hp);
//
//    ImGui::End();
//}