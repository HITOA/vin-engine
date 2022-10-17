#pragma once

#include "core/application.hpp"

namespace Vin {

	class RenderingModule : public Module {
	public:
		virtual void Init();
		virtual void LateRender();
		virtual void OnEvent(EventHandler handler);
	};

}
