#pragma once


#include "sre/SDLRenderer.hpp"



class DreamInspector {

public:

    explicit DreamInspector();

    void updateCharacterGui(std::string name, float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback);

    void updateSceneObjectsSize(int size);

    static DreamInspector* instance;

private:

    int size;

    

};