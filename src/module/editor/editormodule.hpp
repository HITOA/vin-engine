#pragma once

#include "core/application.hpp"
#include <imgui.h>
#include "editordebugconsole.hpp"
#include "editorassetexplorer.hpp"
#include "editorwindow.hpp"

namespace Vin {
	class EditorModule : public Module {
	public:
		void Init();
		void Stop();
		void Process();
		void LateRender();

		void OnEvent(EventHandler handler);
	private:
		void SetImGuiStyle();

		void Begin();
		void End();

		void DrawMainDockSpace();

		void DrawMainMenuBar();
		void DrawBottomBar();

		void DrawMainMenuBarMenuFile();
		void DrawMainMenuBarMenuEdit();
		void DrawMainMenuBarMenuWindow();

		void DrawPreferencesWindow(bool* drawWindow);
		void DrawDebugConsoleWindow(bool* drawWindow);
	private:
		ImGuiContext* m_Ctx{ nullptr };

		bool drawEditor{ true };

		bool drawPreferencesWindow{ false };
		bool drawDebugConsoleWindow{ true };

		EditorDebugConsole m_DebugConsole{};

		std::vector<std::pair<std::unique_ptr<EditorWindow>, bool>> windows{};
	};
}
