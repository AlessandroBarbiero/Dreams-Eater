#include "StartMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

StartMenuComponent::StartMenuComponent(GameObject* gameObject) : Component(gameObject) {

	auto path = GuiHelper::getInstance()->GUI_PATH;

	wraithTexture = sre::Texture::create().withFile(path + "Wraith.png").withFilterSampling(false).build();
	closeTexture = sre::Texture::create().withFile(path + "Close.png").withFilterSampling(false).build();


	auto r = sre::Renderer::instance;
	auto winsize = r->getWindowSize();

	startMenuSize = { baseSize * scaleStartX, baseSize * scaleStartY };
	startMenuPosition = { (winsize.x - startMenuSize.x) / 2.0f, winsize.y / 2.0f };

	settingsMenuSize = { startMenuSize.x * settingsScaleX , startMenuSize.y * settingsScaleY};
	settingsMenuPosition = { (winsize.x - settingsMenuSize.x) / 2.0f, winsize.y / 2.0f };

	wraithSize = { 520 * scaleWraith, 420 * scaleWraith}; //hardcoded sprite dimensions
	closeButtonSize = { (float)closeTexture->getWidth() * scaleCloseButton, (float)closeTexture->getHeight() * scaleCloseButton };

	buttonSize = { 302.0f * scaleButton, 120.0f * scaleButton };
	
	auto &styleSpacing = ImGui::GetStyle().ItemSpacing;
	itemSpacing = styleSpacing;

	if (NUM_BUTTONS < 3)
		itemSpacing = ImVec2{ styleSpacing.x, styleSpacing.y * 8.0f };

	internalStartMenuPosition = { startMenuPosition.x + borderBlack + borderWhite,startMenuPosition.y + borderBlack + borderWhite };
	internalStartMenuSize = { startMenuSize.x - 2 * (borderBlack + borderWhite) ,startMenuSize.y - 2 * (borderBlack + borderWhite) };

	startingButtonY = (internalStartMenuSize.y - NUM_BUTTONS * (buttonSize.y) - (NUM_BUTTONS - 1) * itemSpacing.y) / 2.0f;


	uv0 = GuiHelper::getInstance()->uv0;
	uv1 = GuiHelper::getInstance()->uv1;

}

void StartMenuComponent::onGui(){

	if (DreamGame::instance->gameState == GameState::Ready) {
		start();
	}
	else if(DreamGame::instance->gameState == GameState::Settings){
		settings();
	}

}


void StartMenuComponent::start() {

	ImGui::SetNextWindowPos(startMenuPosition, cond);
	ImGui::SetNextWindowSize(startMenuSize, cond);

	GuiHelper::getInstance()->setZeroPadding();

	bool* open = nullptr;

	ImGui::Begin("StartMenuOut", open, startMenuFlags | ImGuiWindowFlags_NoInputs);

	auto topLeft = startMenuPosition;
	auto bottomRight = ImVec2{ startMenuPosition.x + startMenuSize.x, startMenuPosition.y + startMenuSize.y };

	GuiHelper::getInstance()->drawBackgroundRectangles(topLeft, bottomRight,borderWhite);
	
	ImGui::PopStyleVar();
	ImGui::End();

	GuiHelper::getInstance()->setZeroPadding();

	ImGui::SetNextWindowPos(internalStartMenuPosition, cond);
	ImGui::SetNextWindowSize(internalStartMenuSize, cond);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, GuiHelper::getInstance()->backgroundColor);
	ImGui::Begin("StartMenuIn", open, startMenuFlags);

	ImGui::SetCursorPos(ImVec2(GuiHelper::getInstance()->centerCursorX(buttonSize.x), startingButtonY));
	
	//text is white so that it is visible in grey buttons
	ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32_WHITE);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.05f,0.05f,0.05f,1.0f});
	ImGui::PushFont(GuiHelper::getInstance()->fontFunny35);

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || ImGui::Button("PLAY", buttonSize)) {
		DreamGame::instance->startGame();
		
		//redraw background to cover buttons
		ImGui::GetWindowDrawList()->AddRectFilled(internalStartMenuPosition,
			{ internalStartMenuPosition.x + internalStartMenuSize.x,internalStartMenuPosition.y + internalStartMenuSize.y },
			IM_COL32(GuiHelper::getInstance()->backgroundColor.x * 255, GuiHelper::getInstance()->backgroundColor.y * 255, GuiHelper::getInstance()->backgroundColor.z * 255, GuiHelper::getInstance()->backgroundColor.w * 255)); 

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(wraithSize.x), 0.0f });
		ImGui::Image(wraithTexture->getNativeTexturePtr(), wraithSize, uv0, uv1, GuiHelper::getInstance()->BLACK);


		textSize = ImGui::CalcTextSize(loadingMessage);
		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize.x), wraithSize.y});
		ImGui::TextColored(GuiHelper::getInstance()->BLACK, loadingMessage);
		

	}
	else {
		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(buttonSize.x), buttonSize.y + startingButtonY + itemSpacing.y});
		if (ImGui::Button("SETTINGS", buttonSize)) {
			DreamGame::instance->gameState = GameState::Settings;
		}
	}

	ImGui::PopFont();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);
	
	ImGui::End();
	

}

void StartMenuComponent::settings() {

	bool* open = nullptr;

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(settingsMenuPosition, cond);
	ImGui::SetNextWindowSize(settingsMenuSize, cond);
	GuiHelper::getInstance()->setZeroPadding();
	ImGui::PushFont(GuiHelper::getInstance()->fontFunny35);

	ImGui::Begin("SettingsBack", open, settingsMenuFlags| ImGuiWindowFlags_NoInputs);

	auto topLeft = settingsMenuPosition;
	auto bottomRight = ImVec2{ settingsMenuPosition.x + settingsMenuSize.x, settingsMenuPosition.y + settingsMenuSize.y };

	GuiHelper::getInstance()->drawBackgroundRectangles(topLeft, bottomRight, borderWhite);

	ImGui::PopStyleVar();
	ImGui::End();

	ImGui::SetNextWindowPos({ settingsMenuPosition.x + borderWhite + borderBlack, settingsMenuPosition.y + borderWhite + borderBlack }, cond);
	ImGui::SetNextWindowSize({ settingsMenuSize.x - 2* (borderWhite +  borderBlack), settingsMenuSize.y - 2 * (borderWhite + borderBlack) }, cond);
	GuiHelper::getInstance()->setZeroPadding();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, GuiHelper::getInstance()->backgroundColor);
	ImGui::Begin("Settings", open, settingsMenuFlags);

	ImGui::SetCursorPos({ settingsMenuSize.x - closeButtonSize.x - internalOffset * settingsScaleX - 3 * ( borderWhite + borderBlack), internalOffset * settingsScaleY + borderWhite + borderBlack });
	
	//button is not colored so that no border is visible
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

	if (ImGui::ImageButton(closeTexture->getNativeTexturePtr(), closeButtonSize , uv0, uv1)) {
		DreamGame::instance->gameState = GameState::Ready;
	}

	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

	// increase padding and inner spacing
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10.0f, 3.0f});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 15.0f, 5.0f });

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack, ImGui::GetCursorPosY() + borderWhite + borderBlack });
	ImGui::InputText("Insert your name!", DreamGame::instance->playerName, 64);

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack ,  ImGui::GetCursorPosY() + borderWhite + borderBlack });
	ImGui::Combo("Difficulty", &DreamGame::instance->selectedDifficulty, DreamGame::instance->difficulties, IM_ARRAYSIZE(DreamGame::instance->difficulties));

	auto sizeX = ImGui::CalcTextSize(keybinds).x;
	ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(sizeX));
	ImGui::Text(keybinds);

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack ,  ImGui::GetCursorPosY() + borderWhite + borderBlack });

	ImGui::Combo("Move", &DreamGame::instance->selectedMoveKey, DreamGame::instance->possibleMoveKeys, IM_ARRAYSIZE(DreamGame::instance->possibleMoveKeys));

	auto sMK = DreamGame::instance->possibleMoveKeys[DreamGame::instance->selectedMoveKey];
	auto sSK = DreamGame::instance->possibleShootKeys[DreamGame::instance->selectedShootKey];

	//avoid having the same keybinds for shooting and moving. 
	if (StringToKeys.at(sMK) == StringToKeys.at(sSK)) {
		DreamGame::instance->selectedShootKey = (DreamGame::instance->selectedShootKey + 1) % DreamGame::instance->lengthShootKeys;
	}

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack ,  ImGui::GetCursorPosY() + borderWhite + borderBlack });

	ImGui::Combo("Shoot", &DreamGame::instance->selectedShootKey, DreamGame::instance->possibleShootKeys, IM_ARRAYSIZE(DreamGame::instance->possibleMoveKeys));

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack ,  ImGui::GetCursorPosY() + borderWhite + borderBlack });

	ImGui::Combo("Super Shoot", &DreamGame::instance->selectedSuperShootKey, DreamGame::instance->possibleSuperShootKeys, IM_ARRAYSIZE(DreamGame::instance->possibleSuperShootKeys));
	
	//placed here to have some space below the last combo
	ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5.0f);
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 0.0f), "");

	
	ImGui::PopFont();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(2);
	ImGui::End();

}



    