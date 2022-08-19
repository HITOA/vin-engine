#pragma once 

#include "renderer/renderer.hpp"

namespace Vin {
	class OpenGLRenderingApi : public Renderer::RenderingApi {
	public:
		void Init() final;
		void SetViewport(int x, int y, int width, int height) final;
		void Clear(float r, float g, float b, float a) final;
		void DrawArrays(const eastl::shared_ptr<VertexArray>& vertexArray, size_t verticiesCount) final;
		void DrawIndexed(const eastl::shared_ptr<VertexArray>& vertexArray, size_t indexCount) final;
	};
}
