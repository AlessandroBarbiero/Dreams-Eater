#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "SpriteComponent.hpp"
#include "CharacterComponent.hpp"

enum class Animation {

};

class SpriteAnimationComponent: public Component {
public:
    SpriteAnimationComponent(GameObject *gameObject);

    void update(float deltaTime) override;
    void onGui() override;

    float getAnimationTime() const;                         // time between animations (in seconds)
    void setAnimationTime(float animationTime);
    void setBaseAnimationTime(float animationTime);
    void addAnimationSequence(State state, std::vector<sre::Sprite> animation);
    void resetTime();
    bool displayCompleteAnimation(State anim);
    bool displayCompleteAnimation(State anim, float totalDuration, const std::function<void()> &callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback);
    bool displayCompleteAnimation(State anim, const std::function<void()>& callback, bool urgent);
    bool displayCompleteAnimation(State anim, float totalDuration);

private:

    std::map<State, std::vector<sre::Sprite>> animationSequences;
    std::vector<sre::Sprite> sprites;

    std::shared_ptr<SpriteComponent> spriteComp;

    bool showingCompleteAnim = false;
    std::function<void()> callbackFunc = nullptr;
    float animationTime = 0.300f;
    float baseAnimationTime = 0.300f;
    bool flipAnimation = false;
    float time = 0;
    int spriteIndex = 0;
};

