#include "editorassetexplorer.hpp"

#include <imgui.h>
#include "editordebugconsole.hpp"
#include "editorassetexplorer.hpp"

#include "assets/asset.hpp"

#include "logger/logger.hpp"

void Vin::EditorAssetExplorer::Draw(bool* drawWindow)
{
	if (ImGui::Begin("Asset Explorer", drawWindow))
		DrawAssetExplorer();
	ImGui::End();
}

void Vin::EditorAssetExplorer::DrawAssetExplorer()
{

}
