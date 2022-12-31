#pragma once
#include "Component.hpp"
#include "CharacterComponent.hpp"


class PauseMenuComponent : public Component {

public:

    explicit PauseMenuComponent(GameObject* gameObject);
    void onGui() override;

private:

    std::shared_ptr<CharacterComponent> playerCharacter;
    std::string playerName;

    int menuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    int cond = ImGuiCond_Always;

    ImVec2 itemSpacing;
    float buttonPositionX;
    ImVec2 internalMenuPosition;
    ImVec2 internalMenuSize;
    ImVec2 menuPosition;
    ImVec2 menuSize;
    ImVec2 buttonSize = { 120, 60 };

    float verticalButtonOffset = 10.0f;
    float borderWhite = 10.0f;
    float borderBlack = 10.0f;
};