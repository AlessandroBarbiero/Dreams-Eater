#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include <memory>
#include "DreamGame.hpp"
#include "DreamInspector.hpp"

SpriteAnimationComponent::SpriteAnimationComponent(GameObject *gameObject) : Component(gameObject) {
    spriteComp = gameObject->getComponent<SpriteComponent>();
    assert(spriteComp != nullptr);
}

void SpriteAnimationComponent::update(float deltaTime) {
    
    time += deltaTime;
    
    if (time > animationTime){
        time = fmod(time, animationTime);
        spriteIndex++;
        if (!showingCompleteAnim) {
            // Retreive state to show from the character
            auto charComponent = gameObject->getComponent<CharacterComponent>();
            sprites = animationSequences[charComponent->getState()];
            spriteIndex = spriteIndex % sprites.size();
        }
        
        if (showingCompleteAnim && spriteIndex == sprites.size()) {
            endCompleteAnimation();
        }
        
        spriteComp->setSprite(sprites[spriteIndex]);
    }
}

void SpriteAnimationComponent::endCompleteAnimation() {
    spriteIndex--;
    showingCompleteAnim = false;
    animationTime = baseAnimationTime;
    if (callbackFunc != nullptr) {
        callbackFunc();
        callbackFunc = nullptr;
    }
    currentAnimation = State::Idle;
}

float SpriteAnimationComponent::getAnimationTime() const {
    return animationTime;
}

void SpriteAnimationComponent::setAnimationTime(float animationTime) {
    SpriteAnimationComponent::animationTime = animationTime;
}

void SpriteAnimationComponent::setBaseAnimationTime(float animationTime)
{
    baseAnimationTime = animationTime;
    SpriteAnimationComponent::animationTime = animationTime;
}

void SpriteAnimationComponent::addAnimationSequence(State state, std::vector<sre::Sprite> animation)
{
    animationSequences.insert({ state, animation });
}

void SpriteAnimationComponent::resetTime()
{
    time = 0;
}

// Show a complete animation sequence, it is not overriding old animations
// Return false if the animation won't be displayed
bool SpriteAnimationComponent::displayCompleteAnimation(State anim)
{
    // If I am already showing a complete animation don't start the next one
    if (showingCompleteAnim == true)
        return false;
    showingCompleteAnim = true;
    time = 0;
    spriteIndex = 0;
    sprites = animationSequences[anim];
    currentAnimation = anim;
    return true;
}

// Show a complete animation sequence and at the end call the callback, the time of the animation is specified in totalDuration seconds.
// It is not overriding old animations
// Return false if the animation won't be displayed
bool SpriteAnimationComponent::displayCompleteAnimation(State anim, float totalDuration, const std::function<void()>& callback)
{
    if (displayCompleteAnimation(anim, callback)) {
        float duration = totalDuration < _minDuration ? _minDuration : totalDuration;
        animationTime = duration / sprites.size();
        return true;
    }
    return false;
}

// Show a complete animation sequence and at the end call the callback, it is not overriding old animations
// Return false if the animation won't be displayed
bool SpriteAnimationComponent::displayCompleteAnimation(State anim, const std::function<void()>& callback)
{
    return displayCompleteAnimation(anim, callback, false);
}

// Show a complete animation sequence and at the end call the callback, it can override previous animations if urgent is set to true,
// Return false if the animation won't be displayed
// Return true if the animation is being already displayed
bool SpriteAnimationComponent::displayCompleteAnimation(State anim, const std::function<void()>& callback, bool urgent)
{
    if (currentAnimation == anim)
        return true;
    if (!urgent && showingCompleteAnim == true) {
        return false;
    }
    showingCompleteAnim = true;
    time = 0;
    spriteIndex = 0;
    sprites = animationSequences[anim];
    currentAnimation = anim;
    animationTime = baseAnimationTime;
    callbackFunc = callback;
    return true;
}

// Show a complete animation sequence that will last "totalDuration" seconds. It is not overriding old animations
// Return false if the animation won't be displayed
bool SpriteAnimationComponent::displayCompleteAnimation(State anim, float totalDuration) {
    
    if (!displayCompleteAnimation(anim))
        return false;
    float duration = totalDuration < _minDuration ? _minDuration : totalDuration;
    animationTime = duration / sprites.size();
    return true;
}

float SpriteAnimationComponent::getMinDuration()
{
    return _minDuration;
}

void SpriteAnimationComponent::onGui() {
    std::string name = gameObject->name;
    if (DreamGame::instance->doDebugDraw) {
        DreamInspector::instance->updateAnimationGui(gameObject->name, &animationTime);
    }
}
