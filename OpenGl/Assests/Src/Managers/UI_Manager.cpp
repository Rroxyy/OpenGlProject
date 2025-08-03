#include "UI_Manager.h"

#include "GodClass.h"

void UIManager::drawMainUI() const
{
    {
        if (ImGui::TreeNode("Lighting"))
        {
            ImGui::ColorEdit3("Light Color", (float*)&GodClass::getInstance().lightColor);
            ImGui::TreePop();
        }

        for (const auto& func : mainUI)
        {
            func();
        }
    }
}
