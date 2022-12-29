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

	menuSize = { (float)paperTexture->getWidth(), (float)paperTexture->getHeight() };
	wraithSize = { (float)wraithTexture->getWidth() * scaleWraith, (float)wraithTexture->getHeight() * scaleWraith};

	itemSpacing = ImGui::GetStyle().ItemSpacing;

	//auto scaleWraith = 0.5f;

	auto r = sre::Renderer::instance;
	auto winsize = r->getWindowSize();
	menuPosition = { (winsize.x - menuSize.x) / 2.0f, winsize.y / 2.0f };
}

void StartMenuComponent::onGui(){
	wraithSize = { (float)wraithTexture->getWidth() * scaleWraith, (float)wraithTexture->getHeight() * scaleWraith };

	if (DreamGame::instance->doDebugDraw) {

		bool* open = nullptr;
		ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
		if (ImGui::CollapsingHeader("Start")) {
			ImGui::DragFloat("Laterat", &lateralOffset, 0.1f, 0, 100);
			ImGui::DragFloat("text", &textOffset, 0.1f, 0, 100);
	
		}
		ImGui::End();
	}
	GuiHelper::getInstance()->setZeroPadding();
	static auto uv0 = GuiHelper::getInstance()->uv0;
	static auto uv1 = GuiHelper::getInstance()->uv1;

	bool* open = nullptr;

	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(menuPosition, cond);
	ImGui::SetNextWindowSize(menuSize, cond);

	ImGui::PushFont(GuiHelper::getInstance()->font30);

	textSize = ImGui::CalcTextSize(loadingMessage);

	ImGui::Begin("StartMenu", open, menuFlags);

	ImGui::Image(paperTexture.get()->getNativeTexturePtr(), menuSize, uv0, uv1);
	
	
	//center buttons vertically
	ImGui::SetCursorPos(ImVec2(GuiHelper::getInstance()->centerCursorX(buttonSize.x), (menuSize.y - NUM_BUTTONS *(buttonSize.y) -(NUM_BUTTONS - 1)*itemSpacing.y - buttonSize.y/2.0f)/2.0f));
	

	
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space))  || ImGui::ImageButton(playTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {
			
		DreamGame::instance->startGame();

		ImGui::SetCursorPos({ GuiHelper::getInstance()->baseVec });
		ImGui::Image(paperTexture.get()->getNativeTexturePtr(), menuSize, uv0, uv1); //redraw background to cover button

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(wraithSize.x) + lateralOffset, 0.0f });
		ImGui::Image(wraithTexture->getNativeTexturePtr(), wraithSize, uv0, uv1, GuiHelper::getInstance()->BLACK);

		ImGui::SetCursorPos({ GuiHelper::getInstance()->centerCursorX(textSize.x) + lateralOffset, wraithSize.y - textOffset });
		ImGui::TextColored(GuiHelper::getInstance()->BLACK, loadingMessage);

	}
	else {
		ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(buttonSize.x));
		if (ImGui::ImageButton(settingsTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {}

		ImGui::SetCursorPosX(GuiHelper::getInstance()->centerCursorX(buttonSize.x));
		if (ImGui::ImageButton(historyTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {}
	}

	
    ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}





    