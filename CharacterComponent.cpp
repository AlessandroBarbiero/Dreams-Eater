#include <SDL_events.h>
#include <iostream>
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"
#include "DreamGame.hpp"
#include "SpriteComponent.hpp"
#include "BulletComponent.hpp"
#include "DreamInspector.hpp"

CharacterComponent::CharacterComponent(GameObject* gameObject) : Component(gameObject) {}

//Look for the first projectile shot and check if it has to be destroyed
void CharacterComponent::update(float deltaTime) {
    if (!readyToShoot) {
        shotCooldownTimer += deltaTime;
        if (shotCooldownTimer >= 1 / rateOfFire)
            readyToShoot = true;
    }

    if (flyingProj.empty())
        return;

    auto lastProj = flyingProj.front();
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


void CharacterComponent::onGui() {
    if (gameObject->tag == Tag::Player)
        setPlayerGui();
    else
        setEnemyGui();

    if (DreamGame::instance->doDebugDraw) {
        DreamInspector::instance->updateCharacterGui(gameObject->name,&hp, &armor, &damage, &rateOfFire, &shotSpeed, &knockback);
    }
}

void CharacterComponent::onCollisionStart(PhysicsComponent* comp) {
    Tag myTag = gameObject->tag;
    Tag hisTag = comp->getGameObject()->tag;
    if (myTag == Tag::Player && hisTag == Tag::Enemy) {
        die();
    }
    if (myTag == Tag::Player && hisTag == Tag::EnemyBullet ||
        myTag == Tag::Enemy && hisTag == Tag::PlayerBullet) {
        auto bullet = comp->getGameObject()->getComponent<BulletComponent>();
        float realDamage = bullet->getDamage() - armor;
        if (realDamage > 0) {
            hp -= realDamage;
            //std::cout << "Hit, new hp: " << hp << std::endl;
            if (hp <= 0)
                die();
        }
    }

}

void CharacterComponent::die() {
    gameObject->destroy();
    if (gameObject->tag == Tag::Player)
        DreamGame::instance->gameOver();
}

void CharacterComponent::setPlayerGui(){
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();
    
    ImVec2 pos = {0,0};

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, guiPivot);

    ImGui::SetNextWindowSize(guiSize, ImGuiCond_Always);

    bool* open = nullptr;

    ImGui::Begin("#player", open, flags);

    ImGui::Text("PLAYER");
    ImGui::Text("Health: %.2f", hp);

    ImGui::End();

    
}

void CharacterComponent::setEnemyGui(){
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();

    ImVec2 pos = { winsize.x - guiSize.x, 0 };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, guiPivot);

    ImGui::SetNextWindowSize(guiSize, ImGuiCond_Always);
   
    bool* open = nullptr;

    ImGui::Begin("#enemy", open, flags);

    ImGui::Text("ENEMY");
    ImGui::Text("Health: %.2f", hp);

    ImGui::End();
}

void CharacterComponent::onCollisionEnd(PhysicsComponent* comp) {

}

// Shoot a bullet in the passed direction that is destroyed after it travels for range distance, this method is subjected to the stats:
// - damage
// - shotSpeed
// - range
// - rateOfFire
void CharacterComponent::shot(glm::vec2 direction) {
    if (!readyToShoot) 
        return; // cooldown is not finished
    auto game = DreamGame::instance;
    auto physicsScale = game->physicsScale;

    auto shot = game->createGameObject();
    shot->name = "Bullet";
    if (gameObject->tag == Tag::Player)
        shot->tag = Tag::PlayerBullet;
    else if (gameObject->tag == Tag::Enemy)
        shot->tag = Tag::EnemyBullet;
    else
        shot->tag = Tag::Bullet;

    glm::vec2 position = gameObject->getPosition() / physicsScale + direction * (radius * 2);
    shot->setPosition(position * physicsScale);
    

    auto spriteComp = shot->addComponent<SpriteComponent>();
    shotSprite.setScale(glm::vec2(damage));
    spriteComp->setSprite(shotSprite);


    auto shotPhy = shot->addComponent<PhysicsComponent>();
    float radius = shotSprite.getSpriteSize().x * shotSprite.getScale().x / (2 * physicsScale);
    shotPhy->initCircle(b2_dynamicBody, radius, position, 1);
    shotPhy->setLinearVelocity(direction * shotSpeed);
    shotPhy->setSensor(true);


    auto bullet = shot->addComponent<BulletComponent>();
    bullet->startingPosition = gameObject->getPosition();
    bullet->range = range;
    bullet->damage = damage;
    std::weak_ptr<BulletComponent> weakBullet = bullet;
    flyingProj.push(weakBullet);
    startShotCooldown();

}

void CharacterComponent::setShotSprite(const sre::Sprite& sprite) {
    shotSprite = sprite;
    shotSprite.setOrderInBatch(Depth::Bullet);
}

void CharacterComponent::startShotCooldown() {
    readyToShoot = false;
    shotCooldownTimer = 0;
}







