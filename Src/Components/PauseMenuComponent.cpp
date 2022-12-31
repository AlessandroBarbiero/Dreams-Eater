#include "PauseMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

PauseMenuComponent::PauseMenuComponent(GameObject* gameObject) : Component(gameObject) {

    menuSize = { 400, 210 };

    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();

    menuPosition = { (winsize.x - menuSize.x) / 2.0f, (winsize.y - menuSize.y) / 2.0f };

    internalMenuPosition = { menuPosition.x + borderWhite + borderBlack, menuPosition.y + borderWhite + borderBlack };
    internalMenuSize = { menuSize.x - 2 * (borderWhite + borderBlack), menuSize.y - 2 * (borderWhite + borderBlack) };

    auto& styleSpacing = ImGui::GetStyle().ItemSpacing;
    //itemSpacing = { styleSpacing.x,styleSpacing.y * 2 };
    itemSpacing = styleSpacing;

    buttonPositionX = (internalMenuSize.x - buttonSize.x * 2 - itemSpacing.x) / 2.0f;

    
    
    playerCharacter = DreamGame::instance->player->getComponent<CharacterComponent>();
    playerName = DreamGame::instance->player->name;
}

void PauseMenuComponent::onGui(){




    bool* open = nullptr;

    ImGui::SetNextWindowPos(menuPosition, cond);
    ImGui::SetNextWindowSize(menuSize, cond);
    GuiHelper::getInstance()->setZeroPadding();
    ImGui::Begin("PauseMenuBg", open, menuFlags | ImGuiWindowFlags_NoInputs);

	auto topLeft = menuPosition;
	auto bottomRight = ImVec2{ menuPosition.x + menuSize.x, menuPosition.y + menuSize.y };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32_WHITE);

	topLeft = ImVec2{ menuPosition.x + borderWhite, menuPosition.y + borderWhite };
	bottomRight = ImVec2{ menuPosition.x + menuSize.x - borderWhite, menuPosition.y + menuSize.y - borderWhite };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32_BLACK);

	ImGui::PopStyleVar();
	ImGui::End();


    ImGui::SetNextWindowPos(internalMenuPosition, cond);
    ImGui::SetNextWindowSize(internalMenuSize, cond);
    GuiHelper::getInstance()->setZeroPadding();
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {itemSpacing.x, itemSpacing.y*2});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, GuiHelper::getInstance()->backgroundColor);
    ImGui::Begin("Pause", open, menuFlags);


    ImGui::PushFont(GuiHelper::getInstance()->fontFunny40);

    auto textSize1 = ImGui::CalcTextSize(playerName.c_str());

    ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(textSize1.x));

    ImGui::Text(playerName.c_str());

    auto text = "Enemies Killed: " + std::to_string(DreamGame::instance->countEnemiesKilled);
    auto textChar = text.c_str();
    auto textSize2 = ImGui::CalcTextSize(textChar);
    ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(textSize2.x));
    ImGui::Text(textChar);

    ImGui::PopFont();
    ImGui::SetCursorPosX(buttonPositionX);

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32_WHITE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.05f,0.05f,0.05f,1.0f });
    

    ImGui::PushFont(GuiHelper::getInstance()->fontFunny30);

    // Buttons
    if (ImGui::Button("RESUME", buttonSize)) {
        DreamGame::instance->resume();
    }
    
    ImGui::SameLine();
    ImGui::SetCursorPosX(buttonPositionX + itemSpacing.x + buttonSize.x);

    if (ImGui::Button("RESTART", buttonSize)) {
        DreamGame::instance->resume();
        DreamGame::instance->gameOver();
    }


    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();

}
