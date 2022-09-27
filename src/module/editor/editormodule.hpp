#pragma once

#include "core/application.hpp"
#include <imgui.h>

namespace Vin {
	class EditorModule : public Module {
	public:
		void Init();
		void Stop();
		void Render();
	private:
		ImGuiContext* m_Ctx{ nullptr };
	};
}
