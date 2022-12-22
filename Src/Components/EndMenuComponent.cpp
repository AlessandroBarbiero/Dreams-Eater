#include "EndMenuComponent.hpp"
#include "sre/Renderer.hpp"
#include "DreamGame.hpp";



EndMenuComponent::EndMenuComponent(GameObject* gameObject) : Component(gameObject) {}

void EndMenuComponent::onGui() {
    auto r = sre::Renderer::instance;

    auto flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;
    bool* open = nullptr;

    auto winsize = r->getWindowSize();
    ImVec2 iWinSize(200, 200);
    ImVec2 buttonSize(75, 75);

    ImVec2 pos((winsize.x - iWinSize.x) / 2.0f, winsize.y / 2.0f);
    auto cond = ImGuiCond_Always;
    ImGui::SetNextWindowPos(pos, cond);
    ImGui::SetNextWindowSize(iWinSize, cond);
    ImGui::Begin("", open, flags);

    // Render buttons
    if (ImGui::Button("Restart", buttonSize)) {
        DreamGame::instance->init();
        DreamGame::instance->play();
    }

    const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::SameLine(0.0f, spacing);

    if (ImGui::Button("Lorem Ipsum", buttonSize)) {
        //
    }
    ImGui::End();
}

