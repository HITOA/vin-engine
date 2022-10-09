#include "editordebugconsole.hpp"

#include "logger/logger.hpp"

#include "imgui.h"
#include "imgui_internal.h"

Vin::EditorDebugConsole::EditorDebugConsole() : m_Ostream{ this }, m_Items{}
{
	Logger::AddLogOutputStream(&m_Ostream);
}

Vin::EditorDebugConsole::~EditorDebugConsole()
{
	Logger::RemoveLogOutputStream(&m_Ostream);
}

std::streamsize Vin::EditorDebugConsole::xsputn(const char* s, std::streamsize n)
{
	std::string newstr{ s };
	EntryData::Type type{ EntryData::Info };

	if (newstr.find("[Warning]") != std::string::npos)
		type = EntryData::Warning;
	if (newstr.find("[ERROR]") != std::string::npos)
		type = EntryData::Error;

	if (m_Items.size() <= m_CurrentItemsPos) {
		m_Items.push_back(EntryData{ std::move(newstr), type });
	}
	else {
		m_Items[m_CurrentItemsPos] = EntryData{ std::move(newstr), type };
	}

	m_CurrentItemsPos = ((m_CurrentItemsPos + 1) % (m_Items.size() + 1)) % EDITOR_CONSOLE_MAX_ENTRY;

	return n;
}

void Vin::EditorDebugConsole::Draw(bool* drawWindow)
{
	static bool autoscroll = true;

	if (ImGui::Begin("Debug Console", drawWindow)) {

		ImGui::Checkbox("Auto-scroll", &autoscroll);
		ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) {
			m_Items.clear();
			m_CurrentItemsPos = 0;
		}
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
		for (int i = 0; i < m_Items.size(); ++i) {
			int currentIndex = (m_CurrentItemsPos + i) % m_Items.size();
			switch (m_Items[currentIndex].type) {
			case EntryData::Error: {
				ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), m_Items[currentIndex].str.c_str());
				break;
			}
			case EntryData::Warning: {
				ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), m_Items[currentIndex].str.c_str());
				break;
			}
			case EntryData::Info: {
				ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 1.00f), m_Items[currentIndex].str.c_str());
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
