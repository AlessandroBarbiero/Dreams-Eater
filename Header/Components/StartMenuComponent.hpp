#pragma once
#include "Component.hpp"


class StartMenuComponent : public Component {

public:

    explicit StartMenuComponent(GameObject* gameObject);
    void onGui() override;


private:

    const int menuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    const int cond = ImGuiCond_Always;

    std::string path;

    
    ImVec2 itemSpacing;
    ImVec2 menuPosition;
    ImVec2 menuSize;   
    const ImVec2 buttonSize = { 100, 50 };

   
    const int NUM_BUTTONS = 3;

    std::shared_ptr<sre::Texture> playTexture;
    std::shared_ptr<sre::Texture> historyTexture;
    std::shared_ptr<sre::Texture> settingsTexture;
    std::shared_ptr<sre::Texture> paperTexture;



};