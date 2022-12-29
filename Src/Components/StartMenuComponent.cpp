#include "StartMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

int i = 0;
StartMenuComponent::StartMenuComponent(GameObject* gameObject) : Component(gameObject) {

	auto s = GuiHelper::getInstance()->guiStyle == GuiStyle::Dark ? "Black" : "";

	auto path = GuiHelper::getInstance()->GUI_PATH;

	playTexture = sre::Texture::create().withFile(path + "Play" + s + ".png").withFilterSampling(false).build();
	settingsTexture = sre::Texture::create().withFile(path + "Settings" + s + ".png").withFilterSampling(false).build();
	historyTexture = sre::Texture::create().withFile(path + "History" + s + ".png").withFilterSampling(false).build();
	paperTexture = sre::Texture::create().withFile(path + "Paper.png").withFilterSampling(false).build();
	wraithTexture = sre::Texture::create().withFile(path + "Wraith.png").withFilterSampling(false).build();
	closeTexture = sre::Texture::create().withFile(path + "Close.png").withFilterSampling(false).build();

	startMenuSize = { (float)paperTexture->getWidth(), (float)paperTexture->getHeight() };
	settingsMenuSize = { (float)paperTexture->getWidth() * settingsScale, (float)paperTexture->getHeight()};

	wraithSize = { (float)wraithTexture->getWidth() * scaleWraith, (float)wraithTexture->getHeight() * scaleWraith};
	closeButtonSize = { (float)closeTexture->getWidth() * scaleCloseButton, (float)closeTexture->getHeight() * scaleCloseButton };

	auto &styleSpacing = ImGui::GetStyle().ItemSpacing;
	itemSpacing = styleSpacing;

	if (NUM_BUTTONS < 3)
		itemSpacing = ImVec2{ styleSpacing.x, styleSpacing.y * 4.0f };


	auto r = sre::Renderer::instance;
	auto winsize = r->getWindowSize();

	startMenuPosition = { (winsize.x - startMenuSize.x) / 2.0f, winsize.y / 2.0f };
	settingsMenuPosition = { (winsize.x - settingsMenuSize.x) / 2.0f, winsize.y / 2.0f };

	uv0 = GuiHelper::getInstance()->uv0;
	uv1 = GuiHelper::getInstance()->uv1;

}

void StartMenuComponent::onGui(){
	

	if (DreamGame::instance->doDebugDraw) {
		bool* open = nullptr;
		ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
		if (ImGui::CollapsingHeader("Start")) {
			ImGui::DragFloat("Lateral", &lateralOffset, 0.1f, 0, 100);
			ImGui::DragFloat("text", &textOffset, 0.1f, 0, 100);
	
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

	GuiHelper::getInstance()->setZeroPadding();
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, GuiHelper::getInstance()->baseVec);


	bool* open = nullptr;

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(startMenuPosition, cond);
	ImGui::SetNextWindowSize(startMenuSize, cond);
	textSize = ImGui::CalcTextSize(loadingMessage);

	ImGui::Begin("StartMenu", open, startMenuFlags);

	ImGui::Image(paperTexture.get()->getNativeTexturePtr(), startMenuSize, uv0, uv1);

	auto pos = (startMenuSize.y - NUM_BUTTONS * (buttonSize.y) - (NUM_BUTTONS - 1) * itemSpacing.y - buttonSize.y / 2.0f) / 2.0f;
	ImGui::SetCursorPos(ImVec2(GuiHelper::getInstance()->centerCursorX(buttonSize.x), pos));


	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || ImGui::ImageButton(playTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {

		DreamGame::instance->startGame();

		ImGui::SetCursorPos({ GuiHelper::getInstance()->baseVec });
		ImGui::Image(paperTexture.get()->getNativeTexturePtr(), startMenuSize, uv0, uv1); //redraw background to cover button

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(wraithSize.x) + lateralOffset, 0.0f });
		ImGui::Image(wraithTexture->getNativeTexturePtr(), wraithSize, uv0, uv1, GuiHelper::getInstance()->BLACK);

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize.x) + lateralOffset, wraithSize.y - textOffset });
		ImGui::TextColored(GuiHelper::getInstance()->BLACK, loadingMessage);

	}
	else {

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(buttonSize.x), buttonSize.y + pos + 2*itemSpacing.y});
		if (ImGui::ImageButton(settingsTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {
			DreamGame::instance->gameState = GameState::Settings;
		}

		/*ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(buttonSize.x));
		if (ImGui::ImageButton(historyTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {}*/
	}

	ImGui::End();
	ImGui::PopStyleVar();


	

}

void StartMenuComponent::settings() {

	//GuiHelper::getInstance()->setZeroPadding();
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, GuiHelper::getInstance()->baseVec);


	bool* open = nullptr;

	//ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(settingsMenuPosition, cond);
	ImGui::SetNextWindowSize(settingsMenuSize, cond);

	ImGui::PushFont(GuiHelper::getInstance()->font30);

	textSize = ImGui::CalcTextSize(loadingMessage);

	ImGui::Begin("StartMenu", open, settingsMenuFlags);

	//ImGui::Image(settingsTexture.get()->getNativeTexturePtr(), settingsMenuSize, uv0, uv1);


	ImGui::SetCursorPos({ settingsMenuSize.x - closeButtonSize.x - closeButtonOffset * settingsScale, closeButtonOffset });
	if (ImGui::ImageButton(closeTexture->getNativeTexturePtr(), closeButtonSize , uv0, uv1)) {
		DreamGame::instance->gameState = GameState::Ready;
	}

	ImGui::SetCursorPosX({ closeButtonOffset * settingsScale });
	ImGui::InputText("Insert your name!", DreamGame::instance->playerName, 64);

	ImGui::SetCursorPosX({ closeButtonOffset * settingsScale });
	ImGui::InputInt("Game Difficulty", &DreamGame::instance->difficulty);

	

	ImGui::End();
	//ImGui::PopStyleVar();
	ImGui::PopFont();

}



    