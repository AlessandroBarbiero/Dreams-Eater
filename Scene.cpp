#include <iostream>
#include "Scene.hpp"



std::shared_ptr<GameObject> Scene::createGameObject() {
    auto obj = std::shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

std::vector<std::shared_ptr<GameObject>>* Scene::getSceneObjects() {
    return &sceneObjects;
}

void Scene::setSceneObjects(std::vector<std::shared_ptr<GameObject>> sceneObjects){
    this->sceneObjects = sceneObjects;
}

void Scene::cleanSceneObjects(){
    sceneObjects.clear();
}

