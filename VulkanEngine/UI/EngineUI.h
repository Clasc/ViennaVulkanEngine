#include <imgui.h>

#ifndef ENGINE_UI_H
#define ENGINE_UI_H

namespace ui
{
    class EngineUI
    {
    private:
        bool m_is_active = true;

    public:
        EngineUI();
        ~EngineUI();
        void render();
    };

    EngineUI::EngineUI(/* args */)
    {
    }

    EngineUI::~EngineUI()
    {
    }

    void EngineUI::render()
    {
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("My First Tool", &m_is_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                { /* Do stuff */
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                { /* Do stuff */
                }
                if (ImGui::MenuItem("Close", "Ctrl+W"))
                {
                    m_is_active = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

} // namespace ui
#endif