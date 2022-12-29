#pragma once


#include "sre/SDLRenderer.hpp"

enum class GuiStyle {
    Dark,
    Light
};


class GuiHelper {

public:

    static GuiHelper* getInstance();
    static GuiHelper* instance;

private:

    void setupImGuiStyle(GuiStyle);
    float centerCursorX(float width);
    void setupFont();
    void setupDebugGui();
    void setZeroPadding();


    char* DEBUG_NAME = "DEBUG";
    std::string GUI_PATH = "Sprites/Gui/";
    ImFont* fontMessage;
    ImFont* fontMessage20;/*
    ImFont* fontMessage30;*/
    ImFont* fontSettings20;
    ImFont* fontSettings30;
    ImFont* fontFunny20;
    ImFont* fontFunny30;

    GuiStyle guiStyle;

    const ImVec2 debugWindowSize = { 300,150 };
    const ImVec4 BLACK = ImVec4(0, 0, 0, 1);

    const ImVec2 uv0 = { 0, 1 };
    const ImVec2 uv1 = { 1, 0 };

    const ImVec2 baseVec = { 0, 0 };

    friend class CharacterComponent;
    friend class StartMenuComponent;
    friend class PauseMenuComponent;
    friend class SpriteAnimationComponent;
    friend class RoomComponent;
    friend class LevelGuiComponent;
    friend class DreamGame;
    friend class PlayerController;
};