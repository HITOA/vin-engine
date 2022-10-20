#include "renderer_opengl.hpp"

#include "core/base.hpp">
#include "core/assert.hpp"
#include "logger/logger.hpp"

#include "renderer/opengl/framebuffer_opengl.hpp"

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
	//case GL_DEBUG_SEVERITY_NOTIFICATION: Vin::Logger::Log(message); return;
	}
}

void Vin::OpenGLRenderingApi::Init()
{
#ifdef VIN_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
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

void Vin::OpenGLRenderingApi::Blit(const std::shared_ptr<RenderTarget>& src, const std::shared_ptr<RenderTarget>& dst)
{
	std::shared_ptr<OpenGLRenderTarget> glsrc = std::static_pointer_cast<OpenGLRenderTarget>(src);
	std::shared_ptr<OpenGLRenderTarget> gldst = std::static_pointer_cast<OpenGLRenderTarget>(dst);

	unsigned int srcid = glsrc ? glsrc->GetFrameBufferId() : 0;
	unsigned int dstid = gldst ? gldst->GetFrameBufferId() : 0;

	int srcwidth = src ? src->GetSpecification().width : 0;
	int srcheight = src ? src->GetSpecification().height : 0;

	int dstwidth = dst ? dst->GetSpecification().width : src->GetSpecification().width;
	int dstheight = dst ? dst->GetSpecification().height : src->GetSpecification().height;
	
	glBlitNamedFramebuffer(srcid, dstid, 0, 0, srcwidth, srcheight, 0, 0, dstwidth, dstheight,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
}

void Vin::OpenGLRenderingApi::SetCullMode(CullMode mode)
{
	switch (mode) {
	case CullMode::None: {
		glDisable(GL_CULL_FACE);
		break;
	}
	case CullMode::Front: {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	}
	case CullMode::Back: {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	}
	}
}
