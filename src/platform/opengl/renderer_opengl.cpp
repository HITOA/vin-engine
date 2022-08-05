#include "renderer_opengl.hpp"

#include "core/core.hpp"
#include "core/assert.hpp"
#include "core/logger/logger.hpp"

#include <glad/gl.h>

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         Vin::Logger::Err(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       Vin::Logger::Warn(message); return;
	case GL_DEBUG_SEVERITY_LOW:          Vin::Logger::Log(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: Vin::Logger::Log(message); return;
	}

	VIN_ASSERT(false, "Unknown severity level!");
}

void Vin::OpenGLRenderingApi::Init()
{
#ifdef VIN_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif
}

void Vin::OpenGLRenderingApi::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Vin::OpenGLRenderingApi::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Vin::OpenGLRenderingApi::DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, size_t verticiesCount)
{
	vertexArray->Bind();
	glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
}

void Vin::OpenGLRenderingApi::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, size_t indexCount)
{
	vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, indexCount,
		vertexArray->GetIndexBuffer()->GetIndexType() == BufferIndexType::UnsignedInt16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, nullptr);
}
