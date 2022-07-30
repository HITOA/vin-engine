#pragma once

namespace Vin {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};

	std::unique_ptr<GraphicsContext> CreateGraphicsContext(void* window);

}
