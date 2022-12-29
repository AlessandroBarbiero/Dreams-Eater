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
#define KNOCKBACK_SCALE 10

std::shared_ptr<sre::SpriteAtlas> CharacterComponent::effectAtlas;

CharacterComponent::CharacterComponent(GameObject* gameObject) : Component(gameObject) {
    if (CharacterComponent::effectAtlas == nullptr) {
        std::cout << "Loading effect atlas" << std::endl;
        CharacterComponent::effectAtlas = sre::SpriteAtlas::create("Sprites/SpecialEffects/SpecialEffects_atlas.json", "Sprites/SpecialEffects/SpecialEffects_atlas.png");
    }

    initSpecialEffectObject();

    heartTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Heart.png").withFilterSampling(false).build();
    signTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "MessagePaper.png").withFilterSampling(false).build();

    spriteSize = gameObject->getComponent<SpriteComponent>()->getSprite().getSpriteSize();

    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;
    auto winsize = sre::Renderer::instance->getWindowSize();
    auto size = heartOffset + heartInRow * heartSize.x + (heartInRow - 1) * itemSpacing.x;
    menuSize = ImVec2(size, size);

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

    // Load the hit animation from memory
    std::vector<sre::Sprite> hitAnim(11);
    std::string spriteName = "Hit/";
    for (int i = 0; i < hitAnim.size(); i++) {
        hitAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        hitAnim[i].setOrderInBatch(Depth::Effect);
    }

    std::vector<sre::Sprite> victoryAnim(16);
    spriteName = "Victory/";
    for (int i = 0; i < victoryAnim.size(); i++) {
        victoryAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        victoryAnim[i].setOrderInBatch(Depth::Effect);
    }

    std::vector<sre::Sprite> itemAnim(9);
    spriteName = "Item/";
    for (int i = 0; i < itemAnim.size(); i++) {
        itemAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        itemAnim[i].setOrderInBatch(Depth::Effect);
    }

    std::vector<sre::Sprite> chargeAnim(12);
    spriteName = "Charge/";
    for (int i = 0; i < chargeAnim.size(); i++) {
        chargeAnim[i] = CharacterComponent::effectAtlas->get(spriteName + std::to_string(i) + ".png");
        chargeAnim[i].setOrderInBatch(Depth::Effect);
    }

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

    glm::vec2 position = gameObject->getPosition() / physicsScale + direction * (radius * gameObject->getScale() + damage);
    shot->setPosition(position * physicsScale);
    shot->setRotation(glm::atan(direction.y, direction.x));
    auto spriteComp = shot->addComponent<SpriteComponent>();
    spriteComp->setSprite(bulletSprite);

    auto shotPhy = shot->addComponent<PhysicsComponent>();
    float radius = bulletSprite.getSpriteSize().x / (2 * physicsScale);
    shotPhy->initCircle(b2_dynamicBody, radius, position, 1);
    shotPhy->setLinearVelocity(direction * shotSpeed);
    shotPhy->setSensor(true);

    shot->setScale({ damage });

    auto bullet = shot->addComponent<BulletComponent>();
    bullet->startingPosition = shot->getPosition();
    bullet->range = range;
    bullet->damage = damage;
    bullet->knockback = knockback * KNOCKBACK_SCALE;
    std::weak_ptr<BulletComponent> weakBullet = bullet;
    flyingProj.push(weakBullet);
    startShotCooldown();
}


void CharacterComponent::specialAttack(glm::vec2 direction, float dmg, const std::vector<sre::Sprite> bulletSprites, float imageScale)
{

    std::function<void()> callback = [=]() {
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
        specialShot->setRotation(glm::atan(direction.y, direction.x));

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

        //TODO: use animation component to animate the attack
    };
    specialEffects->displayOnce(State::Charge, callback);
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
        setPlayerGui();
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
            ImGui::DragFloat(std::string("HP ##").append(gameObject->name).c_str(),          &hp, 0.1f, 0, 20);
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
        if (powerupMessageTimeOut > 0) {
            if (ImGui::CollapsingHeader(titleGui.c_str())) {
                ImGui::DragFloat(std::string("TextOffset##").append(gameObject->name).c_str(), &textOffset, 1.0f, 10, 100);
            }
        }
        ImGui::End();
    }
}



void CharacterComponent::setPlayerGui() {

    ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(GuiHelper::getInstance()->baseVec, ImGuiCond_Always, guiPivot);
    ImGui::SetNextWindowBgAlpha(0.0f);

    bool* open = nullptr;

    ImGui::Begin("#player", open, flags);
    
    
    int hpInt = (int)hp;
    auto decimal = hp - hpInt;

    for (int i = 0; i < hpInt && i <= maxHp; i++) {
        ImGui::Image(heartTexture->getNativeTexturePtr(), heartSize, uv0, uv1,  RED);
        if (i < heartInRow - 1 || i > heartInRow - 1)
            ImGui::SameLine();
    }
    if (decimal > 0.0 && hpInt < maxHp){
        ImGui::Image(heartTexture->getNativeTexturePtr(), heartSize, uv0, uv1, WHITE);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - heartSize.x - itemSpacing.x);

        auto uv1Fill = ImVec2{ uv1.x * decimal, uv1.y};
        auto fillSize = ImVec2{ heartSize.x * decimal, heartSize.y};

        ImGui::Image(heartTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, RED);
    }
  
    ImGui::End();
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

    while (textSize.x + textOffset > signTexture->getWidth() * scale || textSize.y + textOffset > signTexture->getHeight() * scale) {
        scale += 0.1f;
    }

    auto signSize = ImVec2{ signTexture->getWidth() * scale, signTexture->getHeight() * scale };
    
    auto powerupMessagePosition = ImVec2{ position.x - signSize.x / 2.0f , position.y - spriteSize.y / 2.0f - signSize.y / 1.3f};

    ImGui::SetNextWindowPos(powerupMessagePosition, ImGuiCond_Always);
    ImGui::SetNextWindowSize(signSize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Sign", open, flags);
    ImGui::Image(signTexture->getNativeTexturePtr(), signSize);

    /*ImGui::GetWindowDrawList()->AddImageRounded(signTexture->getNativeTexturePtr(), powerupMessagePosition,
        { powerupMessagePosition.x + signSize.x, powerupMessagePosition.y + signSize.y }, uv0, uv1, IM_COL32(255, 255, 255, 255), 10.0);*/
    

    ImGui::End();
    ImGui::SetNextWindowPos(powerupMessagePosition, ImGuiCond_Always);
    ImGui::SetNextWindowSize(signSize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    
    ImGui::Begin("Text", open, flags);
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