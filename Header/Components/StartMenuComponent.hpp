#pragma once
#include "Component.hpp"


class StartMenuComponent : public Component {

public:

    explicit StartMenuComponent(GameObject* gameObject);
    void onGui() override;



private:



    void start();
    void settings();

    const int startMenuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    const int settingsMenuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;

    const int cond = ImGuiCond_Always;

    float scaleWraith = 0.5;
    float scaleCloseButton = 0.3;
    float settingsScale = 3.0;

    float lateralOffset = 11.0f;
    float textOffset = 30.0f;
    float closeButtonOffset = 7.0f;
    
    const char* loadingMessage = "Loading...";

    ImVec2 closeButtonSize;
    ImVec2 textSize;
    ImVec2 itemSpacing;
    ImVec2 startMenuPosition;
    ImVec2 startMenuSize;   
    ImVec2 settingsMenuPosition;
    ImVec2 settingsMenuSize;
    ImVec2 wraithSize;
    ImVec2 initialCursorPosition;
    ImVec2 uv0;
    ImVec2 uv1;

    const ImVec2 buttonSize = { 100, 50 };

   
    const int NUM_BUTTONS = 2;

    std::shared_ptr<sre::Texture> playTexture;
    std::shared_ptr<sre::Texture> historyTexture;
    std::shared_ptr<sre::Texture> settingsTexture;
    std::shared_ptr<sre::Texture> paperTexture;

    std::shared_ptr<sre::Texture> wraithTexture;
    std::shared_ptr<sre::Texture> closeTexture;
    std::shared_ptr<sre::Texture> settingsPaperTexture;






};