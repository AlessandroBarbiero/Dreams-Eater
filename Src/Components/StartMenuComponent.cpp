#include "StartMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"


StartMenuComponent::StartMenuComponent(GameObject* gameObject) : Component(gameObject) {

	auto s = GuiHelper::getInstance()->guiStyle == GuiStyle::Dark ? "Black" : "";

	playTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Play" + s + ".png").withFilterSampling(false).build();
	settingsTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Settings" + s + ".png").withFilterSampling(false).build();
	historyTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "History" + s + ".png").withFilterSampling(false).build();
	paperTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "Paper.png").withFilterSampling(false).build();

	menuSize = { (float)paperTexture->getWidth(), (float)paperTexture->getHeight() };

	itemSpacing = ImGui::GetStyle().ItemSpacing;

	auto r = sre::Renderer::instance;
	auto winsize = r->getWindowSize();
	menuPosition = { (winsize.x - menuSize.x) / 2.0f, winsize.y / 2.0f };

}

void StartMenuComponent::onGui(){

	GuiHelper::getInstance()->setZeroPadding();
	static auto uv0 = GuiHelper::getInstance()->uv0;
	static auto uv1 = GuiHelper::getInstance()->uv1;

	bool* open = nullptr;
	
	ImGui::SetNextWindowPos(menuPosition, cond);
	ImGui::SetNextWindowSize(menuSize, cond);

	
	ImGui::Begin("StartMenu", open, menuFlags);

	ImGui::Image(paperTexture.get()->getNativeTexturePtr(), menuSize, uv0, uv1);

	ImGui::SetCursorPos(ImVec2(0.0f, (menuSize.y - NUM_BUTTONS *(buttonSize.y) -(NUM_BUTTONS - 1)*itemSpacing.y - buttonSize.y/2.0f)/2.0f));

    // Render buttons
	GuiHelper::getInstance()->centerCursor(buttonSize.x);
    if (ImGui::ImageButton(playTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {
        DreamGame::instance->play();
    }

	GuiHelper::getInstance()->centerCursor(buttonSize.x);
    if (ImGui::ImageButton(settingsTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {}

	GuiHelper::getInstance()->centerCursor(buttonSize.x);
    if (ImGui::ImageButton(historyTexture.get()->getNativeTexturePtr(), buttonSize, uv0, uv1)) {}

    ImGui::End();
	ImGui::PopStyleVar();
}





    