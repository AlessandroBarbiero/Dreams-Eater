#include "LevelGuiComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

LevelGuiComponent::LevelGuiComponent(GameObject* gameObject) : Component(gameObject) {

    auto path = GuiHelper::getInstance()->GUI_PATH;

    mapTexture = sre::Texture::create().withFile(path + "map.png").withFilterSampling(false).build();
    bossTexture = sre::Texture::create().withFile(path + "FinalBoss.png").withFilterSampling(false).build();
    powerupTexture = sre::Texture::create().withFile(path + "boost.png").withFilterSampling(false).build();
    spawnTexture = sre::Texture::create().withFile(path + "home.png").withFilterSampling(false).build();
    
    menuSize = { mapTexture->getWidth() * mapScale, mapTexture->getHeight() * mapScale };
    menuPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset,windowOffset };
    
    internalOffset = {internalOffsetPercentage * mapTexture->getWidth(),internalOffsetPercentage * mapTexture->getHeight()};

    mapPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset + internalOffset.x, menuPosition.y + internalOffset.y};
    mapSize = { mapTexture->getWidth() * mapScale - 2 * internalOffset.x, mapTexture->getHeight() * mapScale - 2 * internalOffset.y  };
    }

LevelGuiComponent::~LevelGuiComponent(){
    level.reset();
}

void LevelGuiComponent::setLevel(std::shared_ptr<Level> level) {
    this->level = level;
}

//void LevelGuiComponent::setPlayer(std::shared_ptr<GameObject> player) {
//    this->player = player;
//}

void LevelGuiComponent::drawRoom(std::shared_ptr<RoomSettings> roomSettings, ImVec2 topLeft, ImVec2 bottomRight, ImVec2 size){

    auto halfSize = ImVec2{ size.x / 2.0f, size.y / 2.0f };
    auto center = ImVec2{ topLeft.x + halfSize.x, topLeft.y + halfSize.y };
    iconSize = { iconLength * scale, iconLength * scale};

    auto uv0 = GuiHelper::getInstance()->uv0;
    auto uv1 = GuiHelper::getInstance()->uv1;

    ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, borderColor);
    ImGui::GetWindowDrawList()->AddRectFilled({ topLeft.x + borderThickness, topLeft.y + borderThickness }, { bottomRight.x - borderThickness, bottomRight.y - borderThickness }, roomColor);

    ImGui::SetCursorScreenPos(ImVec2{center.x - iconSize.x / 2.0f, center.y - iconSize.x / 2.0f });
    switch (roomSettings->roomType) {
        case EnemyRoom:
            break;
        case PowerUpRoom:
            ImGui::Image(powerupTexture->getNativeTexturePtr(), iconSize, uv0, uv1,ImVec4(0,0,0,1));
            break;
        case BossRoom:
            ImGui::Image(bossTexture->getNativeTexturePtr(), iconSize, uv0, uv1, ImVec4(0, 0, 0, 1));
            break;
        case SpawnRoom:
            ImGui::Image(spawnTexture->getNativeTexturePtr(), iconSize, uv0, uv1, ImVec4(0, 0, 0, 1));
            break;
    }

    

    for (auto& door : roomSettings->doors) {
        auto point = center;
        switch (door.position) {
        case Top:
            point.y -= halfSize.y;
            break;
        case TopLeft:
            point.y -= halfSize.y;
            point.x -= halfSize.x / 2.0f;
            break;
        case TopRight:
            point.y -= halfSize.y;
            point.x += halfSize.x / 2.0f;
            break;
        case Bottom:
            point.y += halfSize.y;
            break;
        case BottomLeft:
            point.y += halfSize.y;
            point.x -= halfSize.x / 2.0f;
            break;
        case BottomRight:
            point.y += halfSize.y;
            point.x += halfSize.x / 2.0f;
            break;
        case Left:
            point.x -= halfSize.x;
            break;
        case LeftTop:
            point.x -= halfSize.x;
            point.y -= halfSize.y / 2.0f;
            break;
        case LeftBottom:
            point.x -= halfSize.x;
            point.y += halfSize.y / 2.0f;
            break;
        case Right:
            point.x += halfSize.x;
            break;
        case RightTop:
            point.x += halfSize.x;
            point.y -= halfSize.y / 2.0f;
            break;
        case RightBottom:
            point.x += halfSize.x;
            point.y += halfSize.y / 2.0f;
            break;
        }
        ImGui::GetWindowDrawList()->AddCircleFilled(point, doorRadius, doorColor);
    }
}

void LevelGuiComponent::onGui() {

    if (DreamGame::instance->doDebugDraw) {

        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        if (ImGui::CollapsingHeader("Minimap")) {
            ImGui::DragFloat("room scale ##", &scale, 0.1f, 0, 20);
            ImGui::DragFloat("map scale ##", &mapScale, 0.1f, 0, 1);
        }
        ImGui::End();
    }

    
    
    GuiHelper::getInstance()->setZeroPadding();
    ImGui::SetNextWindowBgAlpha(0.0f);

    static auto uv0 = GuiHelper::getInstance()->uv0;
    static auto uv1 = GuiHelper::getInstance()->uv1;

    auto currentRoom = level->roomSettings[level->currentRoomIndex];
    
    bool* open = nullptr;

    ImGui::SetNextWindowPos(menuPosition, cond);
    ImGui::SetNextWindowSize(menuSize, cond);
    ImGui::Begin("Back", open, menuFlags);

    ImGui::Image(mapTexture.get()->getNativeTexturePtr(), menuSize, uv0, uv1);

    ImGui::End();
    ImGui::PopStyleVar();

    GuiHelper::getInstance()->setZeroPadding();
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::SetNextWindowPos(mapPosition, cond);
    ImGui::SetNextWindowSize(mapSize, cond);
    ImGui::Begin("Minimap", open, menuFlags);

    //ImGui::SetCursorPos(GuiHelper::getInstance()->baseVec);

    auto roomSize = currentRoom->size;
    ImVec2 size = { roomSize.x * scale, roomSize.y * scale };

    //show current room
    ImVec2 centerTopLeft = { menuSize.x / 2.0f - size.x / 2.0f + menuPosition.x, menuSize.y / 2.0f - size.y / 2.0f + menuPosition.y };
    ImVec2 centerBottomRight = { menuSize.x / 2.0f + size.x / 2.0f + menuPosition.x , menuSize.y / 2.0f + size.y / 2.0f + menuPosition.y };
    roomColor = currentRoomColor;
    drawRoom(currentRoom, centerTopLeft, centerBottomRight, size);

    glm::vec2 currPos1 = currentRoom->positions[0];
    glm::vec2 currPos2 = currentRoom->positions[0];

    auto lowerX = centerTopLeft.x;
    auto lowerY = centerTopLeft.y;

    auto higherX = centerBottomRight.x;
    auto higherY = centerBottomRight.y;
    

    switch (currentRoom->roomSize)
    {
        case Medium:
            currPos2 = currentRoom->positions[0];
            break;
        case Wide:
        case Long:
            currPos2 = currentRoom->positions[1];
            break;
        case Large:
            currPos2 = currentRoom->positions[3];
            break;
    }
    roomColor = otherRoomColor;
    for (auto room : level->roomSettings )
    {
        if (room == currentRoom || !level->roomEntered[room->id]) {
            continue;
        }
        glm::vec2 pos1 = room->positions[0];
        glm::vec2 pos2 = room->positions[0];
        switch (room->roomSize)
        {
        case Medium:
            pos2 = room->positions[0];
            break;
        case Wide:
        case Long:
            pos2 = room->positions[1];
            break;
        case Large:
            pos2 = room->positions[3];
            break;
        }
        roomSize = room->size;
        ImVec2 drawSize = { roomSize.x * scale, roomSize.y * scale };
        auto squareSize = glm::vec2(7 * scale);
        auto offset1 = (pos1 - currPos1) * squareSize;
        auto offset2 = (pos2 - currPos2) * squareSize;
        ImVec2 topLeft = { centerTopLeft.x + offset1.x, centerTopLeft.y - offset1.y };
        ImVec2 bottomRight = { centerBottomRight.x + offset2.x, centerBottomRight.y - offset2.y };


        lowerX = topLeft.x < lowerX ? topLeft.x : lowerX;
        higherX = bottomRight.x > higherX ? bottomRight.x : higherX;

        higherY = bottomRight.y > higherY ? bottomRight.y : higherY;
        lowerY = topLeft.y < lowerY ? topLeft.y : lowerY;
        
        
        drawRoom(room, topLeft, bottomRight, drawSize);
    }

    if (scale > minScale && (lowerX < mapPosition.x + internalOffset.x || higherY > mapPosition.y + menuSize.y - internalOffset.y || higherX > mapPosition.x + menuSize.x - internalOffset.x || lowerY < mapPosition.y + internalOffset.y)) {
        scale -= scaleDecrement;
    }
    

    ImGui::End();
    ImGui::PopStyleVar();

}
