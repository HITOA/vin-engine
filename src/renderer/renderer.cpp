#include "renderer.hpp"

#include "core/assert.hpp"

#include "opengl/renderer_opengl.hpp"

Vin::Renderer::Api Vin::Renderer::s_api = Vin::Renderer::None;
Vin::Renderer::RenderingApi* Vin::Renderer::s_RenderingApi = new NoneRenderingApi{};

void Vin::Renderer::Init()
{
	switch (s_api) {
	case Api::OpenGL:
		s_RenderingApi = new OpenGLRenderingApi{};
		break;
	}

	s_RenderingApi->Init();
}

void Vin::Renderer::Terminate()
{
	delete s_RenderingApi;
	s_api = Renderer::None;
}

void Vin::Renderer::SetApi(Api api)
{
	if (api == Count)
		s_api = OpenGL;
	else
		s_api = api;
}

Vin::Renderer::Api Vin::Renderer::GetApi()
{
	return s_api;
}

void Vin::Renderer::SetViewport(int x, int y, int width, int height)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->SetViewport(x, y, width, height);
}

void Vin::Renderer::Clear(float r, float g, float b, float a)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->Clear(r, g, b, a);
}

void Vin::Renderer::DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, size_t verticiesCount)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->DrawArrays(vertexArray, verticiesCount);
}

void Vin::Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, size_t indexCount)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	indexCount = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
	s_RenderingApi->DrawIndexed(vertexArray, indexCount);
}
