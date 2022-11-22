#pragma once


#include "sre/SDLRenderer.hpp"
#include "RoomBuilder.hpp"



class DreamInspector {

public:

    explicit DreamInspector();

    void updateCharacterGui(std::string name, float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback);

    void updateRoomGui(std::string name, glm::vec2 roomSize, std::string wallsType);

    void updateSceneObjectsSize(int size);

    static DreamInspector* instance;

private:

    int size;

    

};