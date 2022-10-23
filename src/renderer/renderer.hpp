#pragma once

#include "vinpch.hpp"

#include "renderer/vertexarray.hpp"
#include "renderer/framebuffer.hpp"
#include "scene/material.hpp"

namespace Vin {
	enum class DrawingMode {
		None = 0,
		Triangle,
		Point
	};

	enum class CullMode {
		None,
		Front,
		Back
	};

	enum class BlendMode {
		None,
		Blend
	};

	class Renderer {
	public:
		enum Api {
			None = 0,
			OpenGL,
			Count
		};

	public:
		static void Init();
		static void Terminate();
		static void SetApi(Api api);
		static Api GetApi();

		static void SetViewport(int x, int y, int width, int height);
		static void Clear(float r, float g, float b, float a);
		static void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t verticiesCount);
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);

		static void Blit(const std::shared_ptr<RenderTarget>& src, const std::shared_ptr<RenderTarget>& dst);

		static void SetCullMode(CullMode mode);
		static void SetBlendMode(BlendMode mode);

		static void BlitMultiSample(const std::shared_ptr<RenderTexture>& src, const std::shared_ptr<RenderTarget>& dst, Material mat);
		static void BlitMultiSample(const std::shared_ptr<RenderTexture>& src, const std::shared_ptr<RenderTarget>& dst);
	private:
		static Api s_api;
	protected:
		friend class NoneRenderingApi;
		friend class OpenGLRenderingApi;

		static class RenderingApi {
		public:
			virtual ~RenderingApi() {};

			virtual void Init() = 0;
			virtual void SetViewport(int x, int y, int width, int height) = 0;
			virtual void Clear(float r, float g, float b, float a) = 0;
			virtual void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t verticiesCount) = 0;
			virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;
			virtual void Blit(const std::shared_ptr<RenderTarget>& src, const std::shared_ptr<RenderTarget>& dst) = 0;
			virtual void SetCullMode(CullMode mode) = 0;
			virtual void SetBlendMode(BlendMode mode) = 0;
		} *s_RenderingApi;
	};

	class NoneRenderingApi : public Renderer::RenderingApi {
		virtual void Init() {};
		virtual void SetViewport(int, int, int, int) {};
		virtual void Clear(float, float, float, float) {};
		virtual void DrawArrays(const std::shared_ptr<VertexArray>&, uint32_t) {};
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>&, uint32_t) {};
		virtual void Blit(const std::shared_ptr<RenderTarget>&, const std::shared_ptr<RenderTarget>&) {};
		virtual void SetCullMode(CullMode) {};
		virtual void SetBlendMode(BlendMode) {};
	};
}
