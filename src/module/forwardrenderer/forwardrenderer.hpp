#pragma once

#include "core/application.hpp"
#include "renderpipeline/renderpipeline.hpp"

namespace Vin {

	class ForwardRendererModule : public Module, public RenderPipeline {
	public:
		void Init();
		void Render();
	};

}
