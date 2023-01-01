#include "LevelGuiComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"
#include "RoomComponent.hpp"
#include "CharacterComponent.hpp"


LevelGuiComponent::LevelGuiComponent(GameObject* gameObject) : Component(gameObject) {

    auto path = GuiHelper::getInstance()->GUI_PATH;

    mapTexture = sre::Texture::create().withFile(path + "map.png").withFilterSampling(false).build();
    bossTexture = sre::Texture::create().withFile(path + "FinalBoss.png").withFilterSampling(false).build();
    powerupTexture = sre::Texture::create().withFile(path + "boost.png").withFilterSampling(false).build();
    spawnTexture = sre::Texture::create().withFile(path + "home.png").withFilterSampling(false).build();

    barTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Bar.png").withFilterSampling(false).build();
    
    barSize = { (float)barTexture->getWidth(), (float)barTexture->getHeight() * 0.5f};
    
    menuSize = { mapTexture->getWidth() * mapScale, mapTexture->getHeight() * mapScale };
    menuPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset,windowOffset };
    
    internalOffset = {internalOffsetPercentage * mapTexture->getWidth(),internalOffsetPercentage * mapTexture->getHeight()};

    mapPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset + internalOffset.x, menuPosition.y + internalOffset.y};
    mapSize = { mapTexture->getWidth() * mapScale - 2 * internalOffset.x, mapTexture->getHeight() * mapScale - 2 * internalOffset.y  };

    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;

    uv0 = GuiHelper::getInstance()->uv0;
    uv1 = GuiHelper::getInstance()->uv1;

    auto r = sre::Renderer::instance;
    
    levelInfoPosition = { (float)r->getWindowSize().x / 2.0f - levelInfoSize.x / 2.0f, 0.0f };
    
}



void LevelGuiComponent::setLevel(std::shared_ptr<Level> level) {
    this->level = level;
}

void LevelGuiComponent::onGui() {


    if (DreamGame::instance->doDebugDraw) {


        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        if (ImGui::CollapsingHeader("Level")) {
            ImGui::DragFloat("room scale in minimap##", &scale, 0.1f, 0, 20);
            ImGui::Text("Difficulty: %d", level->difficulty);
            ImGui::Text("Number of rooms: %d", level->roomEntered.size());
        }
        ImGui::End();
    }

    drawMinimap();

    if (level->roomSettings[level->currentRoomIndex]->roomType == RoomType::BossRoom && level->boss != nullptr) {
        showBoss(level->boss);
    }


    showLevelInfo();

    

}


void LevelGuiComponent::showBoss(std::shared_ptr<GameObject> boss){

    auto bossComponent = boss->getComponent<CharacterComponent>();

    bool* open = nullptr;
    
    if (bossComponent->hp > 0.0) {

        auto maxLife = bossComponent->defaultHp;

        auto uv1Fill = ImVec2{ uv1.x * bossComponent->hp / maxLife, uv1.y };
        auto fillSize = ImVec2{ barSize.x * bossComponent->hp / maxLife, barSize.y };
  
        auto position = DreamGame::instance->camera->getWindowCoordinates(glm::vec3(boss->getPosition(), 0.0));
        auto menuPos = ImVec2{ position.x - barSize.x / 2.0f, position.y - bossTexture->getHeight() / 5.0f };
        ImGui::SetNextWindowSize(barSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);
        GuiHelper::getInstance()->setZeroPadding();
        ImGui::Begin("Boss", open, menuFlags);

        ImGui::Image(barTexture->getNativeTexturePtr(), barSize, uv0, uv1);

        ImGui::SameLine();

        ImGui::SetCursorPosX(0.0);

        ImGui::Image(barTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, barColor);

        ImGui::PopStyleVar();

        ImGui::End();

    }

}

void LevelGuiComponent::showLevelInfo(){

    GuiHelper::getInstance()->setZeroPadding();
    bool* open = nullptr;
    ImGui::SetNextWindowPos(levelInfoPosition, cond);
    ImGui::SetNextWindowSize(levelInfoSize, cond);
    ImGui::Begin("Background", open, menuFlags);

    auto topLeft = levelInfoPosition;
    auto bottomRight = ImVec2{ levelInfoPosition.x + levelInfoSize.x, levelInfoPosition.y + levelInfoSize.y };

    ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32_BLACK);

    ImGui::End();

    //ImVec4 backgroundColor = ImVec4(0.9, 0.8, 0.6, 1.0f);
    ImVec4 backgroundColor = ImVec4(0.8, 0.6, 0.2, 1.0f);
    auto border = 5.0f;

    ImGui::SetNextWindowPos({ levelInfoPosition.x + border, levelInfoPosition.y + border }, cond);
    ImGui::SetNextWindowSize({levelInfoSize.x - 2* border, levelInfoSize.y - 2 * border }, cond);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
    ImGui::PushFont(GuiHelper::getInstance()->fontFunny30);
    ImGui::Begin("LevelInfo", open, menuFlags);

    auto difficulty = DreamGame::instance->difficulties[DreamGame::instance->selectedDifficulty];

    auto text ="Level " +  std::to_string(level->difficulty - (int)StringToDifficulty.at(difficulty) + 1);
    auto textSize = ImGui::CalcTextSize(text.c_str());
    ImGui::SetCursorPos({GuiHelper::getInstance()->centerCursorX(textSize.x),( levelInfoSize.y - border - textSize.y) / 2.0f});
    ImGui::Text(text.c_str());
        

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::End();
}


void LevelGuiComponent::drawMinimap() {


    GuiHelper::getInstance()->setZeroPadding();
    ImGui::SetNextWindowBgAlpha(0.0f);

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


    auto roomSize = currentRoom->size;
    ImVec2 size = { roomSize.x * scale, roomSize.y * scale };

    //show current room
    ImVec2 centerTopLeft = { menuSize.x / 2.0f - size.x / 2.0f + menuPosition.x, menuSize.y / 2.0f - size.y / 2.0f + menuPosition.y };
    ImVec2 centerBottomRight = { menuSize.x / 2.0f + size.x / 2.0f + menuPosition.x , menuSize.y / 2.0f + size.y / 2.0f + menuPosition.y };
    roomColor = currentRoomColor;
    drawRoom(currentRoom, centerTopLeft, centerBottomRight, size);


    //show other rooms
    glm::vec2 currPos1 = currentRoom->positions[0];
    glm::vec2 currPos2 = currentRoom->positions[0];

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
    for (auto room : level->roomSettings)
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


        drawRoom(room, topLeft, bottomRight, drawSize);
    }

    ImGui::End();
    ImGui::PopStyleVar();
}





void LevelGuiComponent::drawRoom(std::shared_ptr<RoomSettings> roomSettings, ImVec2 topLeft, ImVec2 bottomRight, ImVec2 size) {

    auto halfSize = ImVec2{ size.x / 2.0f, size.y / 2.0f };
    auto center = ImVec2{ topLeft.x + halfSize.x, topLeft.y + halfSize.y };

    iconSize = { iconLength * scale, iconLength * scale };

    auto uv0 = GuiHelper::getInstance()->uv0;
    auto uv1 = GuiHelper::getInstance()->uv1;

    ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, borderColor);

    ImVec2 topLeftInternal = { topLeft.x + borderThickness, topLeft.y + borderThickness };
    ImVec2 bottomRightInternal = { bottomRight.x - borderThickness, bottomRight.y - borderThickness };

    ImGui::GetWindowDrawList()->AddRectFilled(topLeftInternal, bottomRightInternal, roomColor);

    ImGui::SetCursorScreenPos(ImVec2{ center.x - iconSize.x / 2.0f, center.y - iconSize.x / 2.0f });
    switch (roomSettings->roomType) {
    case EnemyRoom:
        break;
    case PowerUpRoom:
        ImGui::Image(powerupTexture->getNativeTexturePtr(), iconSize, uv0, uv1, ImVec4(0, 0, 0, 1));
        break;
    case BossRoom:
        ImGui::Image(bossTexture->getNativeTexturePtr(), iconSize, uv0, uv1, ImVec4(0, 0, 0, 1));
        break;
    case SpawnRoom:
        ImGui::Image(spawnTexture->getNativeTexturePtr(), iconSize, uv0, uv1, ImVec4(0, 0, 0, 1));
        break;
    }



    for (auto& door : roomSettings->doors) {
        auto topLeftDoor = center;
        auto bottomRightDoor = center;
        switch (door.position) {
        case Top:
            topLeftDoor.y -= halfSize.y - doorLength;
            topLeftDoor.x -= doorLength;
            bottomRightDoor.y -= halfSize.y + doorLength;
            bottomRightDoor.x += doorLength;
            break;
        case TopLeft:
            topLeftDoor.y -= halfSize.y - doorLength;
            topLeftDoor.x -= halfSize.x / 2.0f - doorLength;
            bottomRightDoor.y -= halfSize.y + doorLength;
            bottomRightDoor.x -= halfSize.x / 2.0f + doorLength;
            break;
        case TopRight:
            topLeftDoor.y -= halfSize.y - doorLength;
            topLeftDoor.x += halfSize.x / 2.0f - doorLength;
            bottomRightDoor.y -= halfSize.y + doorLength;
            bottomRightDoor.x += halfSize.x / 2.0f + doorLength;
            break;
        case Bottom:
            topLeftDoor.y += halfSize.y - doorLength;
            topLeftDoor.x -= doorLength;
            bottomRightDoor.y += halfSize.y + doorLength;
            bottomRightDoor.x += doorLength;
            break;
        case BottomLeft:
            topLeftDoor.y += halfSize.y - doorLength;
            topLeftDoor.x -= halfSize.x / 2.0f - doorLength;
            bottomRightDoor.y += halfSize.y + doorLength;
            bottomRightDoor.x -= halfSize.x / 2.0f + doorLength;
            break;
        case BottomRight:
            topLeftDoor.y += halfSize.y - doorLength;
            topLeftDoor.x += halfSize.x / 2.0f - doorLength;
            bottomRightDoor.y += halfSize.y + doorLength;
            bottomRightDoor.x += halfSize.x / 2.0f + doorLength;
            break;
        case Left:
            topLeftDoor.x -= halfSize.x - doorLength;
            topLeftDoor.y -= doorLength;
            bottomRightDoor.x -= halfSize.x + doorLength;
            bottomRightDoor.y += doorLength;
            break;
        case LeftTop:
            topLeftDoor.x -= halfSize.x - doorLength;
            topLeftDoor.y -= halfSize.y / 2.0f - doorLength;
            bottomRightDoor.x -= halfSize.x + doorLength;
            bottomRightDoor.y -= halfSize.y / 2.0f + doorLength;
            break;
        case LeftBottom:
            topLeftDoor.x -= halfSize.x - doorLength;
            topLeftDoor.y += halfSize.y / 2.0f - doorLength;
            bottomRightDoor.x -= halfSize.x + doorLength;
            bottomRightDoor.y += halfSize.y / 2.0f + doorLength;
            break;
        case Right:
            topLeftDoor.x += halfSize.x - doorLength;
            topLeftDoor.y -= doorLength;
            bottomRightDoor.x += halfSize.x + doorLength;
            bottomRightDoor.y += doorLength;
            break;
        case RightTop:
            topLeftDoor.x += halfSize.x - doorLength;
            topLeftDoor.y -= halfSize.y / 2.0f - doorLength;
            bottomRightDoor.x += halfSize.x + doorLength;
            bottomRightDoor.y -= halfSize.y / 2.0f + doorLength;
            break;
        case RightBottom:
            topLeftDoor.x += halfSize.x - doorLength;
            topLeftDoor.y += halfSize.y / 2.0f - doorLength;
            bottomRightDoor.x += halfSize.x + doorLength;
            bottomRightDoor.y += halfSize.y / 2.0f + doorLength;
            break;
        }
        ImGui::GetWindowDrawList()->AddRectFilled(topLeftDoor, bottomRightDoor, doorColor);;
    }
}
