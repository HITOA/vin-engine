#include "renderer_opengl.hpp"

#include <glad/gl.h>

void Vin::OpenGLRenderingApi::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Vin::OpenGLRenderingApi::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
