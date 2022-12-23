#pragma once


#include "sre/SDLRenderer.hpp"
#include "RoomBuilder.hpp"



class DreamInspector {

public:

    explicit DreamInspector();

    void updateCharacterGui(float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback, GameObject* go);

    void updateRoomGui(std::string name, glm::vec2 roomSize, std::string wallsType);

    void updateSceneObjectsSize(int size);

    void DreamInspector::updateAnimationGui(std::string name, float* animationTime);

    static DreamInspector* instance;

private:

    int size;

    

};