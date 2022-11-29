#include "DreamInspector.hpp"
#include "DreamGame.hpp"
#include "CharacterComponent.hpp"


DreamInspector* DreamInspector::instance = nullptr;

DreamInspector::DreamInspector(){
    instance = this;
}

void DreamInspector::updateCharacterGui(std::string name, float* hp, float* armor, float* damage, float* rateOfFire, float* shotSpeed, float* knockback, GameObject* go) {
    
    if (ImGui::CollapsingHeader(std::string("Character Component - ").append(name).c_str())) {
        ImGui::DragFloat(std::string("HP ##").append(name).c_str(), hp, 0.1f, 0, 5);
        ImGui::DragFloat(std::string("Armor##").append(name).c_str(), armor, 0.1f, 0, 5);
        ImGui::DragFloat(std::string("Damage##").append(name).c_str(), damage, 0.1f, 0, 5);
        ImGui::DragFloat(std::string("Rate Of Fire##").append(name).c_str(), rateOfFire, 0.1f, 0.5f, 10);
        ImGui::DragFloat(std::string("Shoot Speed##").append(name).c_str(), shotSpeed, 0.1f, 0, 10);
        ImGui::DragFloat(std::string("Knockback##").append(name).c_str(), knockback, 0.1f, 0, 10);
        ImGui::Text("Scale % .2f", go->getScale());

        if (ImGui::Button(std::string("Scale+##").append(name).c_str(), { 100,25 })) {
            go->setScale(go->getScale() + 0.1f);
        }
        if (ImGui::Button(std::string("Scale-##").append(name).c_str(), { 100,25 })) {
            go->setScale(go->getScale() - 0.1f);
        }
        
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
    if (ImGui::CollapsingHeader(name.insert(0, "SpriteAnimationComponent - ").c_str())) {
        ImGui::DragFloat(name.insert(0, "AnimationTime##").c_str(), animationTime, 0.05f, 0.05f, 5);
    }
}