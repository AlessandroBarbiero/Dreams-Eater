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

    signTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Sign.png").withFilterSampling(false).build();
    barTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Bar.png").withFilterSampling(false).build();
    
    barSize = { (float)barTexture->getWidth(), (float)barTexture->getHeight() * 0.5f};
    
    menuSize = { mapTexture->getWidth() * mapScale, mapTexture->getHeight() * mapScale };
    menuPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset,windowOffset };
    
    internalOffset = {internalOffsetPercentage * mapTexture->getWidth(),internalOffsetPercentage * mapTexture->getHeight()};

    mapPosition = { sre::Renderer::instance->getWindowSize().x - menuSize.x - windowOffset + internalOffset.x, menuPosition.y + internalOffset.y};
    mapSize = { mapTexture->getWidth() * mapScale - 2 * internalOffset.x, mapTexture->getHeight() * mapScale - 2 * internalOffset.y  };

    sizeBossMenu = { (float)signTexture->getWidth() * scaleBossMenu, (float)signTexture->getHeight() * scaleBossMenu };
    auto r = sre::Renderer::instance;
    bossPositionMenu = { (float) r->getWindowSize().x - sizeBossMenu.x , r->getWindowSize().y - sizeBossMenu.y };

    ImGuiStyle& style = ImGui::GetStyle();
    itemSpacing = style.ItemSpacing;

    uv0 = GuiHelper::getInstance()->uv0;
    uv1 = GuiHelper::getInstance()->uv1;
    
    }

//LevelGuiComponent::~LevelGuiComponent(){
//    level.reset();
//}

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

    ImVec2 topLeftInternal = {topLeft.x + borderThickness, topLeft.y + borderThickness };
    ImVec2 bottomRightInternal = { bottomRight.x - borderThickness, bottomRight.y - borderThickness };

    ImGui::GetWindowDrawList()->AddRectFilled(topLeftInternal, bottomRightInternal, roomColor);

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

    //center = ImVec2{ ( topLeft.x - bottomRight.x ) / 2.0f, (topLeft.y - bottomRight.y) / 2.0f };

    //halfSize = ImVec2{ halfSize.x - borderThickness, halfSize.y -  borderThickness };


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

void LevelGuiComponent::showBoss(std::shared_ptr<GameObject> boss){

    auto bossComponent = boss->getComponent<CharacterComponent>();
    /*static auto file = CharacterTypeToString.at(bossComponent->type);
    static auto bossStatsTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + file + ".png").withFilterSampling(false).build();*/

    bool* open = nullptr;
    
    if (bossComponent->hp > 0.0) {

    auto maxLife = bossComponent->defaultHp;

    auto uv1Fill = ImVec2{ uv1.x * bossComponent->hp / maxLife, uv1.y };
    auto fillSize = ImVec2{ barSize.x * bossComponent->hp / maxLife, barSize.y };

    ImVec4 barColor = { 1, 0, 0, 1 };
        
    
    /*ImGui::SetNextWindowSize(sizeBossMenu, ImGuiCond_Always);
    ImGui::SetNextWindowPos(bossPositionMenu, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    GuiHelper::getInstance()->setZeroPadding();
    ImGui::Begin("Boss", open, menuFlags);
    ImGui::PushFont(GuiHelper::getInstance()->fontFunny20);

    ImGui::GetWindowDrawList()->AddImageRounded(signTexture->getNativeTexturePtr(), bossPositionMenu,
        { bossPositionMenu.x + sizeBossMenu.x, bossPositionMenu.y + sizeBossMenu.y }, uv0, uv1, IM_COL32(255, 255, 255, 255), rounding);

    auto offset = sizeBossMenu.x - barSize.x - size.x - itemSpacing.x * 2.0f;

    ImGui::SetCursorPos({offset / 2.0f , sizeBossMenu.y / 2.0f - size.y / 2.0f});

    ImGui::Image(bossStatsTexture->getNativeTexturePtr(), size,uv0,uv1);
    ImGui::SameLine();

    ImGui::Image(barTexture->getNativeTexturePtr(), barSize, uv0, uv1);

    ImGui::SameLine();

    

    ImGui::SetCursorPosX(offset / 2.0f + size.x + itemSpacing.x);

    ImGui::Image(barTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, barColor);

    ImGui::PopStyleVar();
    ImGui::PopFont();

    ImGui::End();*/

    auto position = DreamGame::instance->camera->getWindowCoordinates(glm::vec3(boss->getPosition(), 0.0));
    auto menuPos = ImVec2{ position.x - barSize.x / 2.0f, position.y - bossTexture->getHeight() / 5.0f };
    ImGui::SetNextWindowSize(barSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    GuiHelper::getInstance()->setZeroPadding();
    ImGui::Begin("Boss2", open, menuFlags);

    ImGui::Image(barTexture->getNativeTexturePtr(), barSize, uv0, uv1);

    ImGui::SameLine();

    ImGui::SetCursorPosX(0.0);

    ImGui::Image(barTexture->getNativeTexturePtr(), fillSize, uv0, uv1Fill, barColor);

    ImGui::PopStyleVar();

    ImGui::End();

    }

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

    if (level->roomSettings[level->currentRoomIndex]->roomType == RoomType::BossRoom && level->boss != nullptr) {
        showBoss(level->boss);
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

    /*auto lowerX = centerTopLeft.x;
    auto lowerY = centerTopLeft.y;

    auto higherX = centerBottomRight.x;
    auto higherY = centerBottomRight.y;*/
    

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


        /*lowerX = topLeft.x < lowerX ? topLeft.x : lowerX;
        higherX = bottomRight.x > higherX ? bottomRight.x : higherX;

        higherY = bottomRight.y > higherY ? bottomRight.y : higherY;
        lowerY = topLeft.y < lowerY ? topLeft.y : lowerY;*/
        
        
        drawRoom(room, topLeft, bottomRight, drawSize);
    }

    /*if (scale > minScale && (lowerX < mapPosition.x + internalOffset.x || higherY > mapPosition.y + menuSize.y - internalOffset.y || higherX > mapPosition.x + menuSize.x - internalOffset.x || lowerY < mapPosition.y + internalOffset.y)) {
        scale -= scaleDecrement;
    }*/
    

    ImGui::End();
    ImGui::PopStyleVar();
}

