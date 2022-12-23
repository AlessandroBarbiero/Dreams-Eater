#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "SpriteComponent.hpp"
#include "CharacterComponent.hpp" // Contain definition of state


class SpriteAnimationComponent: public Component {
public:
    SpriteAnimationComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    void onGui() override;

    // time between images in animations (in seconds)
    float getAnimationTime() const;
    float getBaseAnimationTime() const;
    void setAnimationTime(float animationTime);
    void setBaseAnimationTime(float animationTime);

    void addAnimationSequence(State state, std::vector<sre::Sprite> animation);
    void resetTime();
    bool displayCompleteAnimation(State anim);
    bool displayCompleteAnimation(State anim, float totalDuration, const std::function<void()> &callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback, bool urgent);
    bool displayCompleteAnimation(State anim, float totalDuration);
    bool displayOnce(State anim, bool urgent = true);
    float getMinDuration();

    // Stop the animation -> useful for objects to be displayed only in particular conditions
    void deactivate();
    void activate();
private:

    bool active = true;

    void endCompleteAnimation();
    std::map<State, std::vector<sre::Sprite>> animationSequences;
    std::vector<sre::Sprite> sprites;
    float _minDuration = 0.2f;

    // The animation currently displayed -> it is set to Idle if there are no complete animations being diplayed at the moment
    State currentAnimation = State::IdleRight;

    std::shared_ptr<SpriteComponent> spriteComp;

    bool showingCompleteAnim = false;
    std::function<void()> callbackFunc = nullptr;
    float animationTime = 0.300f;
    float baseAnimationTime = 0.300f;
    bool flipAnimation = false;
    float time = 0;
    int spriteIndex = 0;
};


