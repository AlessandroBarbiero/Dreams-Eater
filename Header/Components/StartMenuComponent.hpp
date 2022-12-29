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

    float scaleWraith = 0.5;

    float lateralOffset = 11.0f;
    float textOffset = 30.0f;
    const char* loadingMessage = "Loading...";

    ImVec2 textSize;
    ImVec2 itemSpacing;
    ImVec2 menuPosition;
    ImVec2 menuSize;   
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


    void start();
    void settings();





};