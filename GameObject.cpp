#include "GameObject.hpp"
#include <cassert>
#include <algorithm>
#include "Component.hpp"
#include <iostream>

GameObject::~GameObject(){
    // remove reference to this in components
    for (auto& c : components){
        c->gameObject = nullptr;
    }
    for (auto& c : children) {
        c->parent = nullptr;
    }
    if (parent != nullptr) {
        parent->removeChild(this);
    }
}

bool GameObject::removeComponent(std::shared_ptr<Component> component) {
    auto comp = std::find(components.begin(), components.end(),component);
    if (comp != components.end()){
        components.erase(comp);
    }
    return false;
}

const glm::vec2 &GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec2 &position) {
    GameObject::position = position;
}

float GameObject::getRotation() const {
    return rotation;
}

void GameObject::setRotation(float rotation) {
    GameObject::rotation = rotation;
}

void GameObject::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    for (auto& comp : components){
        comp->renderSprite(spriteBatchBuilder);
    }
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components){
        comp->update(deltaTime);
    }
}

const std::vector<std::shared_ptr<Component>> &GameObject::getComponents() {
    return components;
}

void GameObject::addChild(GameObject* child) {
    if (child->parent != nullptr) {
        std::cout << "Reparenting" << std::endl;
        child->parent->removeChild(child);
    }
    children.push_back(child);
    child->parent = this;
}

void GameObject::removeChild(GameObject* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    child->parent = nullptr;
}


void GameObject::destroy() {
    destroyed = true;
    for (auto& c : children) {
        c->destroy();
    }
}