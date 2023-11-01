#include "inspector.h"
#include <imgui.h>

Vin::Ref<Inspector> InspectorWindow::inspectorWindow{ nullptr, nullptr };

void InspectorWindow::Initialize() {

}

void InspectorWindow::Draw(bool *open) {
    if (ImGui::Begin("Inspector", open)) {
        if (inspectorWindow)
            inspectorWindow->Draw();
    }
    ImGui::End();
}

void InspectorWindow::SetInspector(Vin::Ref<Inspector> window) {
    inspectorWindow = window;
}