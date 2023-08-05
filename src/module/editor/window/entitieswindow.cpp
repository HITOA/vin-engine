#include "entitieswindow.hpp"

#include <imgui.h>

void Vin::EntitiesWindow::Draw(bool* draw)
{
	if (ImGui::Begin("Entities", draw)) {

	}
	ImGui::End();
}

std::string Vin::EntitiesWindow::GetName()
{
	return std::string{ "Entities" };
}
