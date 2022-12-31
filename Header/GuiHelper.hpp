#pragma once


#include "sre/SDLRenderer.hpp"


class GuiHelper {

public:

    static GuiHelper* getInstance();
    static GuiHelper* instance;

private:

    void setupImGuiStyle();
    float centerCursorX(float width);
    void setupFont();
    void setupDebugGui();
    void setZeroPadding();


    char* DEBUG_NAME = "DEBUG";
    std::string GUI_PATH = "Sprites/Gui/";
    ImFont* fontMessage;
    ImFont* fontMessage20;
    ImFont* fontSettings20;
    ImFont* fontSettings30;
    ImFont* fontFunny20;
    ImFont* fontFunny30;
    ImFont* fontFunny35;
    ImFont* fontFunny40;

    const ImVec2 debugWindowSize = { 400,250 };
    const ImVec2 debugPosition = ImVec2{ sre::Renderer::instance->getWindowSize().x / 2.0f - debugWindowSize.x / 2.0f, sre::Renderer::instance->getWindowSize().y - debugWindowSize.y };
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