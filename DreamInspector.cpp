#include "DreamInspector.hpp"
#include "DreamGame.hpp"
#include "CharacterComponent.hpp"


DreamInspector* DreamInspector::instance = nullptr;

DreamInspector::DreamInspector(){
    instance = this;
}

void DreamInspector::updateCharacterGui(std::string name, float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback) {

    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::DragFloat(name.insert(0, "Hp##").c_str(), hp, 0.1f, 0, 5);
        ImGui::DragFloat(name.insert(0, "Armor##").c_str(), armor, 0.1f, 0, 5);
        ImGui::DragFloat(name.insert(0, "Damage##").c_str(), damage, 0.1f, 0, 5);
        ImGui::DragFloat(name.insert(0, "Rate of Fire##").c_str(), rateOfFire, 0.1f, 0, 10);
        ImGui::DragFloat(name.insert(0, "Shoot Speed##").c_str(), shotSpeed, 0.1f, 0, 10);
        ImGui::DragFloat(name.insert(0, "Knockback##").c_str(), knockback, 0.1f, 0, 10);
    }
}

void DreamInspector::updateRoomGui(std::string name, glm::vec2 roomSize, std::string wallsType){
    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::Text("Room size: %.3f, %.3f", roomSize.x, roomSize.y);
        ImGui::Text(wallsType.insert(0, "Wall type: ").c_str());
    }
}

void DreamInspector::updateSceneObjectsSize(int size){
    ImGui::Text("TOTAL GAME OBJECTS: %i", size);
}

void DreamInspector::updateAnimationGui(std::string name, float* animationTime) {
    if (ImGui::CollapsingHeader(name.append(" Animation##sprite").c_str())) {
        ImGui::DragFloat(name.insert(0, "AnimationTime##").c_str(), animationTime, 0.05f, 0.05f, 5);
    }
}