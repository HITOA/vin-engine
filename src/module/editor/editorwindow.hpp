#pragma once

#include "vinpch.hpp"

namespace Vin {
	class EditorWindow {
	public:
		virtual ~EditorWindow() {};

		virtual void Draw(bool* draw) = 0;
		virtual std::string GetName() = 0;
	};

	struct RegisterEditorWindowEvent {
		std::unique_ptr<EditorWindow> window{};
		bool show;
	};
}
