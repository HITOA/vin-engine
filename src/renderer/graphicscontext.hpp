#pragma once

#include "vinpch.hpp"

namespace Vin {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static eastl::unique_ptr<GraphicsContext> Create(void* window);
	};

}
