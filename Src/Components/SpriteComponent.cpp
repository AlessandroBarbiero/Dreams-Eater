//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include <iostream>

SpriteComponent::SpriteComponent(GameObject *gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    if (noRender)
        return;
    glm::vec2 position = gameObject->getPosition();

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

void SpriteComponent::activate()
{
    noRender = false;
}

void SpriteComponent::deactivate()
{
    noRender = true;
}
