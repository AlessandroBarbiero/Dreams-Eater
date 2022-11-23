#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include <memory>
#include "DreamGame.hpp"

SpriteAnimationComponent::SpriteAnimationComponent(GameObject *gameObject) : Component(gameObject) {}

void SpriteAnimationComponent::update(float deltaTime) {
    auto spriteComponent = gameObject->getComponent<SpriteComponent>();

    assert(spriteComponent != nullptr);

    time += deltaTime;

    if (time > animationTime){
        time = fmod(time, animationTime);
        spriteIndex = (spriteIndex + 1) % sprites.size();
        spriteComponent->setSprite(sprites[spriteIndex]);
    }
}

void SpriteAnimationComponent::setSprites(std::vector<sre::Sprite> sprites) {
    this->sprites = sprites;
}

float SpriteAnimationComponent::getAnimationTime() const {
    return animationTime;
}

void SpriteAnimationComponent::setAnimationTime(float animationTime) {
    SpriteAnimationComponent::animationTime = animationTime;
}

void SpriteAnimationComponent::onGui() {
    std::string name = gameObject->name;
    if (DreamGame::instance->doDebugDraw) {
        if (ImGui::CollapsingHeader(name.c_str())) {
            ImGui::DragFloat(name.insert(0, "AnimationTime##").c_str(), &animationTime, 0.05f, 0.05f, 5);
        }
    }
}
