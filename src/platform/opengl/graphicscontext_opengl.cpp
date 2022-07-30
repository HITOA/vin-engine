#include "graphicscontext_opengl.hpp"

#ifdef VIN_RENDERER_OPENGL

#include "core/assert.hpp"
#include <glad/gl.h>

Vin::OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Window{ window }
{
	VIN_ASSERT(window != nullptr, "Window for opengl context is null.")
}

void Vin::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);
	int status = gladLoadGL(glfwGetProcAddress);
	VIN_ASSERT(status, "Glad failed initialization.")
}

void Vin::OpenGLContext::SwapBuffer()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_Window);
}

std::unique_ptr<Vin::GraphicsContext> Vin::CreateGraphicsContext(void* window) {
	return std::make_unique<Vin::OpenGLContext>((GLFWwindow*)window);
}

#endif // VIN_RENDERER_OPENGL
