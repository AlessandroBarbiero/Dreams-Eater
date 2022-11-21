#include "DreamInspector.hpp"
#include "DreamGame.hpp"
#include "CharacterComponent.hpp"


DreamInspector* DreamInspector::instance = nullptr;

DreamInspector::DreamInspector(){
    instance = this;
}


void DreamInspector::updateCharacterGui(std::string name,float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback){

    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::DragFloat("Hp", hp, 0.1f, 0, 5);
        ImGui::DragFloat("Armor", armor, 0.1f, 0, 5);
        ImGui::DragFloat("Damage", damage, 0.1f, 0, 5);
        ImGui::DragFloat("Rate of Fire", rateOfFire, 0.1f, 0, 10);
        ImGui::DragFloat("Shot Speed", shotSpeed, 0.1f, 0, 10);
        ImGui::DragFloat("Knockback", knockback, 0.1f, 0, 10);
    }
}

void DreamInspector::updateSceneObjectsSize(int size){
    ImGui::SetNextWindowPos(ImVec2(sre::Renderer::instance->getWindowSize().x / 2 - 100, .0f));
    ImGui::SetNextWindowSize(ImVec2(200, 500));

    ImGui::Text("TOTAL GAME OBJECTS: %i", size);
}
