#pragma once

#include "core/application.hpp"

#define VIN_RENDERCONTEXT_BASEPATH "//Core/RenderContext"

namespace Vin {

	class RenderingModule : public Module {
	public:
		virtual void Init();
		virtual void OnEvent(EventHandler handler);
	};

}
