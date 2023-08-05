#include "editormodule.hpp"
#include "vinpch.hpp"
#include "renderer/rendering.hpp"
#include "imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "module/windowing/windowmodule.hpp"
#include "assets/asset.hpp"
#include "editordebugconsole.hpp"

#include <GLFW/glfw3.h>

#include "inputsystem/input.hpp"

void Vin::EditorModule::Init()
{
	m_Ctx = ImGui::CreateContext();
	SetImGuiStyle();

	switch (Vin::Renderer::GetApi()) {
	case Vin::Renderer::OpenGL: {
		ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
		break;
	}
	}

	ImGui_ImplOpenGL3_Init("#version 410");
}

void Vin::EditorModule::Stop()
{
	ImGui::DestroyContext(m_Ctx);
}

void Vin::EditorModule::Process()
{
	if (Vin::Input::IsKeyPressed(Vin::Key::LeftControl) && Vin::Input::IsKeyDown(Vin::Key::Key_H)) {
		drawEditor = !drawEditor;
	}

	if (Vin::Input::IsKeyPressed(Vin::Key::LeftControl) && Vin::Input::IsKeyDown(Vin::Key::Key_R)) {
		//Hot reload
		size_t i = 0;
		auto itend = AssetDatabase::end();
		for (auto it = AssetDatabase::begin(); it != itend; ++it) {
			Asset<Program> currProgram = it->second.GetAsset<Program>(it->first);
			if (currProgram.Get() != nullptr) {
				++i;
				//Reload program
			}
		}
		Logger::Log("Shader reloaded : {}", i);
	}
}

void Vin::EditorModule::LateRender()
{
	Begin();

	DrawMainDockSpace();

	DrawMainMenuBar();
	DrawBottomBar();

	if (drawEditor) {
		if (drawPreferencesWindow)
			DrawPreferencesWindow(&drawPreferencesWindow);

		if (drawDebugConsoleWindow)
			DrawDebugConsoleWindow(&drawDebugConsoleWindow);

		for (auto& window : windows)
			window.first->Draw(&window.second);
	}

	End();
}

void Vin::EditorModule::OnEvent(EventHandler handler)
{
	//static char buff[256];

	if (WindowDropEvent* event = handler.GetEvent<WindowDropEvent>()) {
		for (int i = 0; i < event->count; ++i) {
			//ImportAsset
			/*cwk_path_normalize(event->paths[i], buff, sizeof(buff));

			{
				//std::string_view str{ buff };
				//std::replace(str.begin(), str.end(), '\\', '/');
			}

			const char* relativepath;
			cwk_path_get_basename(buff, &relativepath, nullptr);
			while (relativepath != buff && !GameFilesystem::Exists(relativepath))
				--relativepath;

			Logger::Log(relativepath);*/
		}
	}

	if (RegisterEditorWindowEvent* event = handler.GetEvent<RegisterEditorWindowEvent>())
		windows.emplace_back(std::move(event->window), event->show);
}

void Vin::EditorModule::SetImGuiStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.50f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.50f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.50f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 0.75f;
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(8.00f, 4.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 6;
	style.FrameRounding = 8;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}

void Vin::EditorModule::Begin()
{
	Asset<WindowInfo> windowInfo = AssetDatabase::GetAsset<WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)windowInfo->width, (float)windowInfo->height);
	io.DeltaTime = (float)GetApp()->GetDeltaTime().GetSecond();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Vin::EditorModule::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Vin::EditorModule::DrawMainDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Main DockSpace", (bool*)0, 
		ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
		ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

	ImGui::PopStyleVar(3);

	ImGui::DockSpace(ImGui::GetID("Main DockSpace"), ImVec2{ 0, 0 }, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
}

void Vin::EditorModule::DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			DrawMainMenuBarMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			DrawMainMenuBarMenuEdit();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			DrawMainMenuBarMenuWindow();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Vin::EditorModule::DrawBottomBar()
{
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	if (ImGui::BeginViewportSideBar("TestBar", viewport, ImGuiDir_Down, height, window_flags)) {
		if (ImGui::BeginMenuBar()) {

			double fps =  1 / GetApp()->GetDeltaTime().GetSecond();
			fps = round(fps * 2) / 2;
			ImGui::TextColored(ImVec4(0.50f, 0.50f, 1.00f, 1.00f), "Fps : %f (%fms)", fps, GetApp()->GetDeltaTime().GetMillisecond());

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
}

void Vin::EditorModule::DrawMainMenuBarMenuFile()
{
	if (ImGui::MenuItem("Quit", "Alt+F4")) {
		GetApp()->Stop();
	}
}

void Vin::EditorModule::DrawMainMenuBarMenuEdit()
{
	if (ImGui::MenuItem(drawEditor ? "Hide Editor" : "Show Editor", "Ctrl+H")) {
		drawEditor = !drawEditor;
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Preferences...")) {
		drawPreferencesWindow = true;
	}
}

void Vin::EditorModule::DrawMainMenuBarMenuWindow()
{
	if (ImGui::MenuItem("Debug Console")) {
		drawDebugConsoleWindow = true;
	}

	for (auto& window : windows) {
		if (ImGui::MenuItem(window.first->GetName().c_str())) {
			window.second = true;
		}
	}
}

void Vin::EditorModule::DrawPreferencesWindow(bool* drawWindow)
{
	if (ImGui::Begin("Preferences", drawWindow)) {
		if (ImGui::BeginTabBar("Preferences Tab Bar", ImGuiTabBarFlags_None)) {

			if (ImGui::BeginTabItem("Style")) {
				ImGuiStyle& style = ImGui::GetStyle();

				static float stylealpha = 0.75;
				ImGui::SliderFloat("Style Alpha", &stylealpha, 0.20f, 1.00f);

				style.Alpha = stylealpha;

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::Separator();
		ImGui::Button("Apply");
	}
	ImGui::End();
}

void Vin::EditorModule::DrawDebugConsoleWindow(bool* drawWindow)
{
	m_DebugConsole.Draw(drawWindow);
}