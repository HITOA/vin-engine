#include "editormodule.hpp"

#include "renderer/rendering.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "module/windowing/windowmodule.hpp"
#include "assets/assetdatabase.hpp"

void Vin::EditorModule::Init()
{
	Asset<GlfwWindowHolder> glfwWindow = AssetDatabase::GetAsset<GlfwWindowHolder>(VIN_GLFWWINDOW_ASSETID);

	m_Ctx = ImGui::CreateContext();
	ImGui::StyleColorsDark();

	switch (Vin::Renderer::GetApi()) {
	case Vin::Renderer::OpenGL: {
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow->window, true);
		break;
	}
	}

	ImGui_ImplOpenGL3_Init("#version 410");
}

void Vin::EditorModule::Stop()
{
	ImGui::DestroyContext(m_Ctx);
}

void Vin::EditorModule::Render()
{
	Asset<WindowInfo> windowInfo = AssetDatabase::GetAsset<WindowInfo>(VIN_WINDOWINFO_ASSETID);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowInfo->width, windowInfo->height);
	io.DeltaTime = GetApp()->GetDeltaTime().GetSecond();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
