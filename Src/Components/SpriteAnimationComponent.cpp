#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include <memory>
#include "DreamGame.hpp"
#include "GuiHelper.hpp";
#include <Components/CharacterComponent.hpp>

SpriteAnimationComponent::SpriteAnimationComponent(GameObject *gameObject) : Component(gameObject) {
    spriteComp = gameObject->getComponent<SpriteComponent>();
    assert(spriteComp != nullptr);
}

void SpriteAnimationComponent::update(float deltaTime) {
    if (!active)
        return;

    time += deltaTime;
    
    if (time > animationTime){
        time = fmod(time, animationTime);
        spriteIndex++;
        if (!showingCompleteAnim) {
            // Retrieve state/direction to show from the character
            auto charComponent = gameObject->getComponent<CharacterComponent>();
            State c_state = charComponent->getState();
            // If the state is a movement state update the direction
            if(c_state == State::Walk)
                facingDirection = charComponent->getDirection();
            auto& animationSequences = getAnimationSequences(facingDirection);
            sprites = animationSequences[c_state];
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

float SpriteAnimationComponent::getBaseAnimationTime() const {
    return baseAnimationTime;
}

std::unordered_map<State, std::vector<sre::Sprite>>& SpriteAnimationComponent::getAnimationSequences(Direction direction) {
    if (direction == Direction::RIGHT)
        return rightAnimationSequences;
    return leftAnimationSequences;
}

void SpriteAnimationComponent::setAnimationTime(float animationTime) {
    SpriteAnimationComponent::animationTime = animationTime;
}

void SpriteAnimationComponent::setBaseAnimationTime(float animationTime)
{
    baseAnimationTime = animationTime;
    SpriteAnimationComponent::animationTime = animationTime;
}

// Add a new animation sequence to be displayed in the state passed, if the state is already linked to an animation, discard the old one and swap it with the new one
void SpriteAnimationComponent::addAnimationSequence(State state, Direction direction, std::vector<sre::Sprite> animation)
{
    auto& animationSequence = getAnimationSequences(direction);
    animationSequence[state] = animation;
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
    
    sprites = getAnimationSequences(facingDirection)[anim];
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
// it cannot override animations of the same type
// Return false if the animation won't be displayed
// Return true if the animation is being already displayed or if the animation will start
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
    sprites = getAnimationSequences(facingDirection)[anim];
    currentAnimation = anim;
    animationTime = baseAnimationTime;
    callbackFunc = callback;
    spriteComp->setSprite(sprites[0]);
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

// Display one time the animation and then disable the sprite, the object will not be displayed after the animation.
// If urgent is set the animation can override himself or other animations
// Return true if the animation is being displayed
bool SpriteAnimationComponent::displayOnce(State anim, bool urgent)
{
    if (!urgent && showingCompleteAnim == true) {
        return false;
    }
    showingCompleteAnim = true;
    time = 0;
    spriteIndex = 0;
    sprites = getAnimationSequences(facingDirection)[anim];
    currentAnimation = anim;
    animationTime = baseAnimationTime;
    // Deactivate everything at the end of the animation
    callbackFunc = [this]() { spriteComp->deactivate(); deactivate(); };
    spriteComp->setSprite(sprites[0]);

    spriteComp->activate();
    activate();
    return true;
}

float SpriteAnimationComponent::getMinDuration()
{
    return _minDuration;
}

// Change the direction the object is facing to in order to display the correct animation, set reload to true if you want to turn the current animation,
// otherwise the effects are visible only from the next one
void SpriteAnimationComponent::setFacingDirection(Direction newDirection, bool reload)
{
    facingDirection = newDirection;
    if (reload)
        sprites = getAnimationSequences(facingDirection)[currentAnimation];
}

void SpriteAnimationComponent::deactivate()
{
    active = false;
}

void SpriteAnimationComponent::activate()
{
    active = true;
}

void SpriteAnimationComponent::onGui() {
    if (DreamGame::instance->doDebugDraw && active) {
        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        auto title = "SpriteAnimationComponent - " + gameObject->name;
        //auto field = "AnimationTime##" + gameObject->name;
        if (ImGui::CollapsingHeader(title.c_str())) {
            //ImGui::DragFloat(field.c_str(, &animationTime, 0.05f, 0.05f, 5);
            ImGui::Text("AnimationTime : %1.3f", animationTime);
        }
        ImGui::End();
    }
}
