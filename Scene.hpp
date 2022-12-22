#pragma once

#include "GameObject.hpp"

class Scene {

public:
    std::shared_ptr<GameObject> createGameObject();
    std::vector<std::shared_ptr<GameObject>>* getSceneObjects();
    void setSceneObjects(std::vector<std::shared_ptr<GameObject>>);

    void cleanSceneObjects();

private:

    std::vector<std::shared_ptr<GameObject>> sceneObjects;
};
