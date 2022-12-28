#pragma once

#include <functional>
#include <string>
#include <sre/Sprite.hpp>
#include <GameObject.hpp>

class Powerup {
public:
    Powerup(const std::string& name, const std::string& message, std::function<void(GameObject*)> effect, sre::Sprite image)
        : name(name), message_(message), effect_(effect), image_(image) {}

    void apply(GameObject* player) { effect_(player); }

    std::string getMessage() { return message_; }


    sre::Sprite getSprite() {   return image_;  }

    std::string name;

private:


    std::function<void(GameObject*)> effect_;
    sre::Sprite image_;

    std::string message_;
};

// How to use:
// void speed_boost(GameObject* player) { player->getComponent<CharacterComponent>()->speed *= 2; }
//      ...
//      Powerup powerup = Powerup("speed boost", speed_boost);
//      powerup.apply(&player);