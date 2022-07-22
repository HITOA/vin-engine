#include "glrenderer.hpp"
#include <glad/gl.h>

int Vin::GlRenderer::Init(VINgetprocfunc getprocfunc)
{
    return gladLoadGL(getprocfunc);
}

void Vin::GlRenderer::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Vin::GlRenderer::Clear()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
