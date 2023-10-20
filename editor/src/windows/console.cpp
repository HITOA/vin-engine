#include "console.h"
#include <imgui.h>

Console::Console() {
    Vin::Logger::AddLogOutput(&ostream);
}

std::streamsize Console::xsputn(const char *s, std::streamsize n) {
    Vin::String newstr{ s };
    EntryData::Type type{ EntryData::Info };

    if (newstr.find("[Warning]") != std::string::npos)
        type = EntryData::Warning;
    if (newstr.find("[ERROR]") != std::string::npos)
        type = EntryData::Error;

    if (items.size() <= currentItemsPos) {
        items.push_back(EntryData{ std::move(newstr), type });
    }
    else {
        items[currentItemsPos] = EntryData{ std::move(newstr), type };
    }

    currentItemsPos = ((currentItemsPos + 1) % (items.size() + 1)) % EDITOR_CONSOLE_MAX_ENTRY;

    return n;
}

void Console::Draw(bool *open) {
    static bool autoscroll = true;

    if (ImGui::Begin("Debug Console", open)) {

        ImGui::Checkbox("Auto-scroll", &autoscroll);
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) {
            items.clear();
            currentItemsPos = 0;
        }
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
        for (int i = 0; i < items.size(); ++i) {
            int currentIndex = (currentItemsPos + i) % items.size();
            switch (items[currentIndex].type) {
                case EntryData::Error: {
                    ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
                case EntryData::Warning: {
                    ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
                case EntryData::Info: {
                    ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
            }
        }

        if ((autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
    }
    ImGui::End();
}