//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include <iostream>

SpriteComponent::SpriteComponent(GameObject *gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    glm::vec2 position = gameObject->getPosition();
    
    // This was my attempt to fix the animation but it is not working 
    /*if (sprite.getFlip().x) {
        position = {    position.x - 2 * (sprite.getSpriteAnchor().x - 0.5) * sprite.getSpriteSize().x     , position.y};
    }*/

    sprite.setPosition(position);
    sprite.setRotation(glm::degrees(gameObject->getRotation()));
    sprite.setScale({ gameObject->getScale(), gameObject->getScale()});
    spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setSprite(const sre::Sprite &sprite) {
    this->sprite = sprite;
}

sre::Sprite SpriteComponent::getSprite() {
    return sprite;
}
