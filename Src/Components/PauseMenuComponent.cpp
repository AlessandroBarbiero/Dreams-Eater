#include "PauseMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";
#include "GuiHelper.hpp"

PauseMenuComponent::PauseMenuComponent(GameObject* gameObject) : Component(gameObject) {
    resumeTexture = sre::Texture::create().withFile(GuiHelper::getInstance()->GUI_PATH + "ResumeBlack.png").withFilterSampling(false).build();

    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();

    menuPosition = { (winsize.x - menuSize.x) / 2.0f, winsize.y / 2.0f };
}

void PauseMenuComponent::onGui(){

    bool* open = nullptr;

    ImGui::SetNextWindowPos(menuPosition, cond);
    ImGui::SetNextWindowSize(menuSize, cond);
    ImGui::Begin("PauseMenu", open, menuFlags);

    // Render buttons
    if (ImGui::ImageButton(resumeTexture->getNativeTexturePtr() , buttonSize, GuiHelper::getInstance()->uv0, GuiHelper::getInstance()->uv1)) {
        DreamGame::instance->resume();
    }
    ImGui::End();

}
