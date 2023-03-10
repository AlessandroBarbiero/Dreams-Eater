#pragma once
#include "Component.hpp"
#include "Level.hpp"
#include "RoomBuilder.hpp"



class LevelGuiComponent : public Component {

public:

    explicit LevelGuiComponent(GameObject* gameObject);


    void onGui() override;

    //Set the current level
    void setLevel(std::shared_ptr<Level>);
    

private:

    //Draw Room and Doors given topLeft and bottomRight vertices and room size.
    void drawRoom(std::shared_ptr<RoomSettings>, ImVec2, ImVec2, ImVec2);

    //Show Boss health in a bar hovering over it.
    void showBoss(std::shared_ptr<GameObject> boss);

    void showLevelInfo();
    void drawMinimap();

    std::shared_ptr<Level> level;

    int menuFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoInputs;

    int cond = ImGuiCond_Always;


    ImVec2 levelInfoPosition;
    ImVec2 levelInfoSize = { 250.0f, 50.0f };

    ImVec4 barColor = { 1, 0, 0, 1 };


    ImVec2 menuPosition;
    ImVec2 menuSize;

    ImVec2 mapPosition;
    ImVec2 mapSize;

    //distance between background image and the minimap in the menu
    ImVec2 internalOffset;
    float internalOffsetPercentage = 0.02f;

    ImVec2 iconSize;

    ImVec2 size = { 40.0,40.0 };

    ImU32 borderColor = IM_COL32(0, 0, 0, 255);
    ImU32 currentRoomColor = IM_COL32(255, 212, 92, 255);
    ImU32 otherRoomColor = IM_COL32(80, 62, 12, 255);
    ImU32 doorColor = IM_COL32(255, 0, 0, 255);
    ImU32 roomColor;

    ImVec2 uv0;
    ImVec2 uv1;
    float rounding = 5.0f;
    
    ImVec2 sizeBossMenu;
    ImVec2 itemSpacing;
    ImVec2 bossPositionMenu;

    //squares are double of this
    float doorLength = 4.0f; 
    float borderThickness = 3.0f;

    float levelInfoScale = 0.3f;
    float minScale = 6.0f;;
    float scale = 7.0f;
    float scaleDecrement = 0.7;
    float mapScale = 0.4;
    float windowOffset = 25.0f;
    float iconLength = 4.0f;

    std::shared_ptr<sre::Texture> mapTexture;
    std::shared_ptr<sre::Texture> bossTexture;
    std::shared_ptr<sre::Texture> powerupTexture;
    std::shared_ptr<sre::Texture> spawnTexture;
    std::shared_ptr<sre::Texture> levelInfoTexture;
    std::shared_ptr<sre::Texture> barTexture;

    ImVec2 barSize;
};