#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "SpriteComponent.hpp"
#include "State.hpp"
#include <unordered_map>

// Remember to add this component always after the SpriteComponent
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

    void addAnimationSequence(State state, Direction direction, std::vector<sre::Sprite> animation);
    void resetTime();
    bool displayCompleteAnimation(State anim);
    bool displayCompleteAnimation(State anim, float totalDuration, const std::function<void()> &callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback, bool urgent);
    bool displayCompleteAnimation(State anim, float totalDuration);
    bool displayOnce(State anim, float animTime = -1, bool urgent = true, const std::function<void()>& callback = nullptr);
    bool displayOnce(State anim, const std::function<void()>& callback);
    float getMinDuration();

    bool showFixedAnimation(std::vector<sre::Sprite> animVector, bool loop = true, bool urgent = false);

    void setFacingDirection(Direction newDirection, bool reload = false);

    // Stop the animation -> useful for objects to be displayed only in particular conditions
    void deactivate();
    void activate();
private:

    bool active = true;

    void endCompleteAnimation();
    std::unordered_map<State, std::vector<sre::Sprite>>& getAnimationSequences(Direction direction);
    std::unordered_map<State, std::vector<sre::Sprite>> rightAnimationSequences;
    std::unordered_map<State, std::vector<sre::Sprite>> leftAnimationSequences;
    std::vector<sre::Sprite> sprites;
    float _minDuration = 0.2f;

    // The animation currently displayed -> it is set to Idle if there are no complete animations being diplayed at the moment
    State currentAnimation = State::Idle;
    Direction facingDirection = Direction::RIGHT;

    std::shared_ptr<SpriteComponent> spriteComp;

    bool loopAnimation = false;
    bool showingCompleteAnim = false;
    std::function<void()> callbackFunc = nullptr;
    float animationTime = 0.300f;
    float baseAnimationTime = 0.300f;
    bool flipAnimation = false;
    float time = 0;
    int spriteIndex = 0;
};


