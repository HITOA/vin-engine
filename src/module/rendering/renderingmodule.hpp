#pragma once

#include "core/application.hpp"

namespace Vin {

	class RenderingModule : public Module {
	public:
		virtual void Init();
		virtual void Render();
		virtual void OnEvent(EventHandler handler);
	};

}
