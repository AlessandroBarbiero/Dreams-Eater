#pragma once
#include "Component.hpp"
#include "Level.hpp"
#include "RoomBuilder.hpp"



class LevelGuiComponent : public Component {

public:

    explicit LevelGuiComponent(GameObject* gameObject);
    void onGui() override;
    void setLevel(std::shared_ptr<Level>);
    void setPlayer(std::shared_ptr<GameObject>);

private:

    void drawRoom(std::shared_ptr<RoomSettings>, ImVec2, ImVec2, ImVec2);

    std::shared_ptr<Level> level;
    std::shared_ptr<GameObject> player;

    int menuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    int cond = ImGuiCond_Always;

    ImVec2 menuPosition;
    ImVec2 menuSize = { 250, 250 };
    ImVec2 buttonSize = { 200, 100 };

    ImU32 roomColor = IM_COL32(0, 0, 0, 255);
    ImU32 doorColor = IM_COL32(255, 0, 0, 255);

    float scale = 3.0f;

    std::shared_ptr<sre::Texture> roomTexture;
};