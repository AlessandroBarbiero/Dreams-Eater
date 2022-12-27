#include "LevelGuiComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"
#include "PhysicsComponent.hpp"

LevelGuiComponent::LevelGuiComponent(GameObject* gameObject) : Component(gameObject) {
    roomTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Room.png").withFilterSampling(false).build(); 
    menuPosition = ImVec2{sre::Renderer::instance->getWindowSize().x - menuSize.x,0.0f};
}

void LevelGuiComponent::setLevel(std::shared_ptr<Level> level) {
    this->level = level;
}
void LevelGuiComponent::setPlayer(std::shared_ptr<GameObject> player) {
    this->player = player;
}

void LevelGuiComponent::drawRoom(std::shared_ptr<RoomSettings> roomSettings, ImVec2 topLeft, ImVec2 bottomRight, ImVec2 size){

    ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, borderColor);
    ImGui::GetWindowDrawList()->AddRectFilled({ topLeft.x + 1, topLeft.y + 1 }, { bottomRight.x - 1, bottomRight.y - 1 }, roomColor);
    float radius = 2.0f;

    auto halfSize = ImVec2{ size.x / 2.0f, size.y / 2.0f };

    auto center = ImVec2{ topLeft.x + halfSize.x, topLeft.y + halfSize.y };

   

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
        ImGui::GetWindowDrawList()->AddCircleFilled(point, radius, doorColor);
    }
}

void LevelGuiComponent::onGui() {

    GuiHelper::getInstance()->setZeroPadding();
    //ImGui::SetNextWindowBgAlpha(0.0f);

    /*auto uv0 = GuiHelper::getInstance()->uv0;
    auto uv1 = GuiHelper::getInstance()->uv1;*/

    
    

    /*auto roomSizePixel = roomComp->getRoomSizeInPixels();
    std::cout << "ROOM PIX " << std::endl;
    std::cout << roomSizePixel.x << " " << roomSizePixel.y << std::endl;

    std::cout << "POS PLAYER PHYS" << std::endl;
    auto playerPos = player->getComponent<PhysicsComponent>()->getPosition();
    std::cout << playerPos.x << " " << playerPos.y << std::endl;

    auto center = ImVec2{ topLeft.x + size.x + playerPos.x / roomSizePixel.x * size.x, topLeft.y + size.y + playerPos.y / roomSizePixel.y * size.y };
    float radius = 2.0f;
    ImU32 colorPlayer = IM_COL32(255, 0, 0, 255);

    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, colorPlayer);*/

    auto currentRoom = level->roomSettings[level->currentRoomIndex];
    
    bool* open = nullptr;

    ImGui::SetNextWindowPos(menuPosition, cond);
    ImGui::SetNextWindowSize(menuSize, cond);
    ImGui::Begin("Minimap", open, menuFlags);

    auto roomSize = currentRoom->size;
    ImVec2 size = { roomSize.x * scale, roomSize.y * scale };
    
    /*for (auto it = level->roomSettings.begin(); it != level->roomSettings.end(); ++it)
    {}*/
    //if (level->roomEntered[i]) {}

    //show current room
    ImVec2 centerTopLeft = { menuSize.x / 2.0f - size.x / 2.0f + menuPosition.x, menuSize.y / 2.0f - size.y / 2.0f + menuPosition.y };
    ImVec2 centerBottomRight = { menuSize.x / 2.0f + size.x / 2.0f + menuPosition.x , menuSize.y / 2.0f + size.y / 2.0f + menuPosition.y };
    roomColor = IM_COL32(0, 0, 128, 255);
    drawRoom(currentRoom, centerTopLeft, centerBottomRight, size);

    int i = -1;
    //explore doors

    auto border = 3.0f;


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
    roomColor = IM_COL32(0, 0, 0, 255);
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
        drawRoom(room, topLeft, bottomRight, drawSize);
    }

    ImGui::End();
    ImGui::PopStyleVar();

}
