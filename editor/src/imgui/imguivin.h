#ifndef VIN_EDITOR_IMGUIVIN_H
#define VIN_EDITOR_IMGUIVIN_H

#pragma once

#include <vin/vin.h>

namespace ImGui
{
    IMGUI_API bool  InputText(const char* label, Vin::String* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
    IMGUI_API bool  InputTextMultiline(const char* label, Vin::String* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
    IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, Vin::String* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}

#endif //VIN_EDITOR_IMGUIVIN_H
