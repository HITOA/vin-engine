#pragma once

namespace Vin {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};

	GraphicsContext* CreateGraphicsContext(void* window);
	void DestroyGraphicsContext(GraphicsContext* ctx);

}
