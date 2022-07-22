#pragma once

#include <renderer.hpp>

namespace Vin {
	class GlRenderer : public Renderer {
	public:
		int Init(VINgetprocfunc getprocfunc);
		void SetViewport(int x, int y, int width, int height);
		void Clear();
	};
}
