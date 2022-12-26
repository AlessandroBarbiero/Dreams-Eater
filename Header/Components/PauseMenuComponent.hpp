#pragma once
#include "Component.hpp"


class PauseMenuComponent : public Component {

public:

    explicit PauseMenuComponent(GameObject* gameObject);
    void onGui() override;

private:
    int menuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    int cond = ImGuiCond_Always;

    ImVec2 menuPosition;
    ImVec2 menuSize = { 220, 120 };
    ImVec2 buttonSize = { 200, 100 };

    std::shared_ptr<sre::Texture> resumeTexture;
};