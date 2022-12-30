#include "StartMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

int i = 0;
StartMenuComponent::StartMenuComponent(GameObject* gameObject) : Component(gameObject) {

	auto path = GuiHelper::getInstance()->GUI_PATH;

	playTexture = sre::Texture::create().withFile(path + "Play.png").withFilterSampling(false).build();
	settingsTexture = sre::Texture::create().withFile(path + "Settings.png").withFilterSampling(false).build();
	historyTexture = sre::Texture::create().withFile(path + "History.png").withFilterSampling(false).build();
	wraithTexture = sre::Texture::create().withFile(path + "Wraith.png").withFilterSampling(false).build();
	closeTexture = sre::Texture::create().withFile(path + "Close.png").withFilterSampling(false).build();


	auto r = sre::Renderer::instance;
	auto winsize = r->getWindowSize();

	startMenuSize = { 240.0f * scaleStartX, 240.0f * scaleStartY };
	settingsMenuSize = { 240.0f * scaleStartX * settingsScaleX , 240.0f  * scaleStartY * settingsScaleY};
	settingsMenuPosition = { (winsize.x - settingsMenuSize.x) / 2.0f, winsize.y / 2.0f };


	wraithSize = { (float)wraithTexture->getWidth() * scaleWraith, (float)wraithTexture->getHeight() * scaleWraith};
	closeButtonSize = { (float)closeTexture->getWidth() * scaleCloseButton, (float)closeTexture->getHeight() * scaleCloseButton };

	
	buttonSize = { (float)playTexture->getWidth() * scaleButton, (float)playTexture->getHeight() * scaleButton };
	startMenuPosition = { (winsize.x - startMenuSize.x) / 2.0f, winsize.y / 2.0f };
	

	auto &styleSpacing = ImGui::GetStyle().ItemSpacing;
	itemSpacing = styleSpacing;

	if (NUM_BUTTONS < 3)
		itemSpacing = ImVec2{ styleSpacing.x, styleSpacing.y * 4.0f };


	uv0 = GuiHelper::getInstance()->uv0;
	uv1 = GuiHelper::getInstance()->uv1;

}

void StartMenuComponent::onGui(){

	

	if (DreamGame::instance->doDebugDraw) {
		bool* open = nullptr;
		ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
		if (ImGui::CollapsingHeader("Start")) {
			ImGui::DragFloat("scalePaperX", &scaleStartX, 0.1f, 0, 100);
			ImGui::DragFloat("scalePaperY", &scaleStartY, 0.1f, 0, 100);
			/*ImGui::DragFloat("text", &textOffset, 0.1f, 0, 100);*/
			//ImGui::DragFloat("sett", &settingsScale, 0.05f, 0, 100);
			ImGui::DragFloat("scale Button", &scaleButton, 0.05f, 0, 100);
			ImGui::DragFloat("BorderWhite", &borderWhite, 0.05f, 0, 100);
			ImGui::DragFloat("BorderBlack", &borderBlack, 0.05f, 0, 100);
	
		}
		ImGui::End();
	}


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

	ImGui::PushFont(GuiHelper::getInstance()->fontFunny40);

	bool* open = nullptr;

	ImGui::Begin("StartMenuOut", open, startMenuFlags);

	auto topLeft = startMenuPosition;
	auto bottomRight = ImVec2{ startMenuPosition.x + startMenuSize.x, startMenuPosition.y + startMenuSize.y };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(255, 255, 255, 255));

	topLeft = ImVec2{ startMenuPosition.x + borderWhite, startMenuPosition.y + borderWhite };
	bottomRight = ImVec2{ startMenuPosition.x + startMenuSize.x - borderWhite, startMenuPosition.y + startMenuSize.y - borderWhite };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(0, 0, 0, 255));
	
	ImGui::SetCursorPos({ borderWhite + borderBlack,borderWhite + borderBlack });

	ImGui::PopStyleVar();
	ImGui::End();


	GuiHelper::getInstance()->setZeroPadding();
	ImGui::SetNextWindowPos({startMenuPosition.x + borderBlack + borderWhite,startMenuPosition.y + borderBlack + borderWhite }, cond);
	ImGui::SetNextWindowSize({ startMenuSize.x - 2* ( borderBlack + borderWhite ) ,startMenuSize.y - 2 * (borderBlack + borderWhite) }, cond);
	textSize = ImGui::CalcTextSize(loadingMessage);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
	ImGui::Begin("StartMenuIn", open, startMenuFlags);

	
	auto pos = (startMenuSize.y - NUM_BUTTONS * (buttonSize.y) - (NUM_BUTTONS - 1) * itemSpacing.y - buttonSize.y / 2.0f) / 2.0f;

	ImGui::SetCursorPos(ImVec2(GuiHelper::getInstance()->centerCursorX(buttonSize.x), pos));
	

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || ImGui::ImageButton(playTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || ImGui::Button("PLAY", buttonSize)) {
		DreamGame::instance->startGame();


		//redraw background to cover button
		ImGui::GetWindowDrawList()->AddRectFilled({ startMenuPosition.x + borderBlack + borderWhite,startMenuPosition.y + borderBlack + borderWhite }, 
			{ startMenuPosition.x + startMenuSize.x - borderWhite - borderBlack,startMenuPosition.y + startMenuSize.y - borderWhite - borderBlack }, 
			IM_COL32(backgroundColor.x * 255, backgroundColor.y * 255, backgroundColor.z * 255, backgroundColor.w * 255)); 

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(wraithSize.x) + lateralOffset + borderWhite + borderBlack, borderWhite + borderBlack });
		ImGui::Image(wraithTexture->getNativeTexturePtr(), wraithSize, uv0, uv1, GuiHelper::getInstance()->BLACK);


		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize.x)+ lateralOffset, wraithSize.y + textOffset});
		ImGui::TextColored(GuiHelper::getInstance()->BLACK, loadingMessage);
		ImGui::PopStyleColor();

	}
	else {
		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(buttonSize.x), buttonSize.y + pos + 2*itemSpacing.y});
		if (ImGui::Button("SETTNGS", buttonSize)) {
			DreamGame::instance->gameState = GameState::Settings;
		}
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
	ImGui::PopFont();
	ImGui::End();
	

}

void StartMenuComponent::settings() {

	ImGui::SetNextWindowBgAlpha(0.0f);

	
	bool* open = nullptr;
	ImGui::SetNextWindowPos(settingsMenuPosition, cond);
	ImGui::SetNextWindowSize(settingsMenuSize, cond);
	GuiHelper::getInstance()->setZeroPadding();
	ImGui::PushFont(GuiHelper::getInstance()->fontFunny35);

	ImGui::Begin("WhiteBack", open, settingsMenuFlags| ImGuiWindowFlags_NoInputs);

	auto topLeft = settingsMenuPosition;
	auto bottomRight = ImVec2{ settingsMenuPosition.x + settingsMenuSize.x, settingsMenuPosition.y + settingsMenuSize.y };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(255, 255, 255, 255));

	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos({settingsMenuPosition.x + borderWhite, settingsMenuPosition.y + borderWhite}, cond);
	ImGui::SetNextWindowSize({ settingsMenuSize.x - 2 * borderWhite, settingsMenuSize.y + 2 *borderWhite }, cond);
	GuiHelper::getInstance()->setZeroPadding();
	ImGui::Begin("BlackBack", open, settingsMenuFlags | ImGuiWindowFlags_NoInputs);

	topLeft = ImVec2{ settingsMenuPosition.x + borderWhite, settingsMenuPosition.y + borderWhite };
	bottomRight = ImVec2{ settingsMenuPosition.x + settingsMenuSize.x - borderWhite, settingsMenuPosition.y + settingsMenuSize.y - borderWhite };

	ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(0, 0, 0, 255));

	ImGui::PopStyleVar();
	ImGui::End();

	
	ImGui::SetNextWindowPos({ settingsMenuPosition.x + borderWhite + borderBlack, settingsMenuPosition.y + borderWhite + borderBlack }, cond);
	ImGui::SetNextWindowSize({ settingsMenuSize.x - 2* (borderWhite +  borderBlack), settingsMenuSize.y - 2 * (borderWhite + borderBlack) }, cond);
	GuiHelper::getInstance()->setZeroPadding();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9, 0.8, 0.6, 1.0f));
	ImGui::Begin("Settings", open, settingsMenuFlags);

	//ImGui::SetCursorPos({ borderWhite + borderBlack, borderWhite + borderBlack });

	//ImGui::Image(paperTexture.get()->getNativeTexturePtr(), {settingsMenuSize.x - 2*(borderWhite + borderBlack),settingsMenuSize.y - 2 * (borderWhite + borderBlack) }, uv0, uv1);

	ImGui::SetCursorPos({ settingsMenuSize.x - closeButtonSize.x - internalOffset * settingsScaleX - 3 * ( borderWhite + borderBlack), internalOffset * settingsScaleY + borderWhite + borderBlack });
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));


	if (ImGui::ImageButton(closeTexture->getNativeTexturePtr(), closeButtonSize , uv0, uv1)) {
		DreamGame::instance->gameState = GameState::Ready;
	}
	ImGui::PopStyleColor(2);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10.0f, 3.0f});
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 15.0f, 5.0f });

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack, ImGui::GetCursorPosY() + borderWhite + borderBlack });
	ImGui::InputText("Insert your name!", DreamGame::instance->playerName, 64);

	ImGui::SetCursorPos({ internalOffset * settingsScaleX + borderWhite + borderBlack ,  ImGui::GetCursorPosY() + borderWhite + borderBlack });
	ImGui::InputInt("Game Difficulty", &DreamGame::instance->difficulty);

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
	
	

	//placed here to have smoother end 
	ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5.0f);
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 0.0f), "");

	
	ImGui::PopFont();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);
	ImGui::End();

}



    