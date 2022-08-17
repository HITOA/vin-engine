#pragma once

#include "vinpch.hpp"

#include "renderer/vertexarray.hpp"

namespace Vin {
	enum class DrawingMode {
		None = 0,
		Triangle,
		Point
	};

	class Renderer {
	public:
		enum Api {
			None = 0,
			OpenGL
		};

	public:
		static void Init();
		static void InitApi();
		static void Terminate();
		static Api GetApi();

		static void SetViewport(int x, int y, int width, int height);
		static void Clear(float r, float g, float b, float a);
		static void DrawArrays(const eastl::shared_ptr<VertexArray>& vertexArray, size_t verticiesCount);
		static void DrawIndexed(const eastl::shared_ptr<VertexArray>& vertexArray, size_t indexCount = 0);
	private:
		static Api s_api;
	protected:
		friend class OpenGLRenderingApi;

		static class RenderingApi {
		public:
			virtual void Init() = 0;
			virtual void SetViewport(int x, int y, int width, int height) = 0;
			virtual void Clear(float r, float g, float b, float a) = 0;
			virtual void DrawArrays(const eastl::shared_ptr<VertexArray>& vertexArray, size_t verticiesCount) = 0;
			virtual void DrawIndexed(const eastl::shared_ptr<VertexArray>& vertexArray, size_t indexCount) = 0;
		} *s_RenderingApi;
	};
}
