#pragma once

#include "../editorwindow.hpp"

namespace Vin {
	class EntitiesWindow : public EditorWindow {
	public:
		virtual void Draw(bool* draw) final;
		virtual std::string GetName() final;
	};
}
