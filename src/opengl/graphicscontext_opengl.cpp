#include "graphicscontext_opengl.hpp"

#ifdef VIN_RENDERER_OPENGL

#include "assert.hpp"
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
	VIN_ASSERT(!status, "Glad failed initialization.")
}

void Vin::OpenGLContext::SwapBuffer()
{
	glfwSwapBuffers(m_Window);
}

Vin::GraphicsContext* Vin::CreateGraphicsContext(void* window) {
	return new OpenGLContext((GLFWwindow*)window);
}

void Vin::DestroyGraphicsContext(GraphicsContext* ctx) {
	delete ctx;
}

#endif // VIN_RENDERER_OPENGL
