#pragma once

#include <vinbase.hpp>
#include "renderer/graphicscontext.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Vin {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() final;
		void SwapBuffer() final;
	private:
		GLFWwindow* m_Window;
	};
}
