#pragma once

#include "vinpch.hpp"

namespace Vin {
	class Renderer {
	public:
		enum Api {
			None = 0,
			OpenGL
		};

	public:
		static void Init();
		static void Terminate();
		static Api GetApi();

		static void SetViewport(int x, int y, int width, int height);
		static void Clear(float r, float g, float b, float a);
	private:
		static Api s_api;
	protected:
		friend class OpenGLRenderingApi;

		static class RenderingApi {
		public:
			virtual void SetViewport(int x, int y, int width, int height) = 0;
			virtual void Clear(float r, float g, float b, float a) = 0;
		} *s_renderingapi;
	};
}
