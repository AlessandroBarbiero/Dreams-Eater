#pragma once


#include "sre/SDLRenderer.hpp"


class GuiHelper {

public:

    static GuiHelper* getInstance();
    static GuiHelper* instance;

private:

    //Setup custom GUI style
    void setupImGuiStyle();

    //Center the cursor given the width of an element based on the available space
    float centerCursorX(float width);

    //Load all fonts needed
    void setupFont();

    //Setup debug gui flags, position and size. All components can append content
    void setupDebugGui();

    //Remove padding in current window
    void setZeroPadding();

    //Draw a white and a black rectangles given vertices and the distance between them
    void drawBackgroundRectangles(ImVec2 topLeft, ImVec2 bottomRight, float border);


    const char* DEBUG_NAME = "DEBUG";
    const std::string GUI_PATH = "Sprites/Gui/";
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
    const ImVec4 backgroundColor = ImVec4(0.9, 0.8, 0.6, 1.0f);

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