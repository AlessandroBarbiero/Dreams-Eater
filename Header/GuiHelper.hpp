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
    void centerCursor(float width);
    void setupFont();
    void setupDebugGui();
    void setZeroPadding();


    char* DEBUG_NAME = "DEBUG";
    std::string GUI_PATH = "Sprites/Gui/";
    ImFont* font20;
    ImFont* font30;

    GuiStyle guiStyle;

    const ImVec2 debugWindowSize = { 300,150 };

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
};