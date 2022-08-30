#pragma once

#include "core/application.hpp"

namespace Vin {

	class RenderingModule : public Module {
	public:
		void Init() final;
		void OnEvent(EventHandler handler) final;
	};

}
