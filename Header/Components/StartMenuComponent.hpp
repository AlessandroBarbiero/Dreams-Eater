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
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove;

    const int cond = ImGuiCond_Always;

    float scaleWraith = 0.4;
    float scaleCloseButton = 0.4;
    float settingsScaleX = 2.5;
    float settingsScaleY = 1.2;

    float lateralOffset = 11.0f;
    float textOffset = 2.0f;
    float internalOffset = 10.0f;

    float scaleButton = 0.5f;

    float scaleStartX = 1.3f;
    float scaleStartY = 0.95f;
    
    const char* loadingMessage = "Loading...";
    const char* keybinds = "KEYBINDS";

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

    float borderBlack = 3.0f;
    float borderWhite = 3.0f;

    ImVec2 buttonSize = { 150, 50 };

   
    const int NUM_BUTTONS = 2;

    std::shared_ptr<sre::Texture> playTexture;
    std::shared_ptr<sre::Texture> historyTexture;
    std::shared_ptr<sre::Texture> settingsTexture;

    std::shared_ptr<sre::Texture> wraithTexture;
    std::shared_ptr<sre::Texture> closeTexture;

    ImVec4 backgroundColor = ImVec4(0.9, 0.8, 0.6, 1.0f);






};