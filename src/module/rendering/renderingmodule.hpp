#pragma once

#include "core/application.hpp"
#include "assets/assetdatabase.hpp"
#include "rendercontext.hpp"

namespace Vin {

	class RenderingModule : public Module {
	public:
		virtual void Init();
		virtual void Render();
		virtual void OnEvent(EventHandler handler);

	private:
		Asset<RenderContext> ctx{};
	};

}
