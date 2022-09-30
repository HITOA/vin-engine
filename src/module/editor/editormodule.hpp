#pragma once

#include "core/application.hpp"
#include <imgui.h>
#include "editordebugconsole.hpp"
#include "editorassetexplorer.hpp"

namespace Vin {
	class EditorModule : public Module {
	public:
		void Init();
		void Stop();
		void Render();

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
		void DrawAssetExplorerWindow(bool* drawWindow);
	private:
		ImGuiContext* m_Ctx{ nullptr };

		bool drawEditor{ true };

		bool drawPreferencesWindow{ false };
		bool drawDebugConsoleWindow{ true };
		bool drawAssetExplorerWindow{ true };

		EditorDebugConsole m_DebugConsole{};
		EditorAssetExplorer m_AssetExplorer{};
	};
}
