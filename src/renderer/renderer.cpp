#include "renderer.hpp"

#include "core/assert.hpp"

#include "opengl/renderer_opengl.hpp"
#include "scene/material.hpp"

#include <optick.h>

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

void Vin::Renderer::DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t verticiesCount)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->DrawArrays(vertexArray, verticiesCount);
}

void Vin::Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	indexCount = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
	s_RenderingApi->DrawIndexed(vertexArray, indexCount);
}

void Vin::Renderer::Blit(const std::shared_ptr<RenderTarget>& src, const std::shared_ptr<RenderTarget>& dst)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->Blit(src, dst);
}

void Vin::Renderer::SetCullMode(CullMode mode)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->SetCullMode(mode);
}

void Vin::Renderer::SetBlendMode(BlendMode mode)
{
	VIN_ASSERT(s_RenderingApi != nullptr, "Rendering api is not initialized.");
	s_RenderingApi->SetBlendMode(mode);
}

void Vin::Renderer::BlitMultiSample(const std::shared_ptr<RenderTexture>& src, const std::shared_ptr<RenderTarget>& dst, Material mat)
{
	static float vertices[] = {
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
	};

	static unsigned short indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	static std::shared_ptr<VertexBuffer> vbo{};
	static std::shared_ptr<IndexBuffer> ibo{};
	static std::shared_ptr<VertexArray> vao{};

	if (!vao) {
		vbo = VertexBuffer::Create(sizeof(vertices));
		vbo->SetBufferLayout({ {VertexAttribute::Position, VertexAttributeType::Float3} });
		vbo->SetData(vertices, sizeof(vertices), 0);

		ibo = IndexBuffer::Create(Vin::BufferIndexType::UnsignedInt16);
		ibo->SetData(indices, sizeof(indices) / sizeof(short));

		vao = VertexArray::Create();
		vao->AddVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);
	}

	if (dst)
		dst->Bind();

	mat.SetTexture("_MainTex", src);
	mat.SetInt("_MainTexSample", src->GetSampleCount());

	mat.Bind();

	DrawIndexed(vao);

	if (dst)
		dst->Unbind();
}

void Vin::Renderer::BlitMultiSample(const std::shared_ptr<RenderTexture>& src, const std::shared_ptr<RenderTarget>& dst) {
	static std::shared_ptr<Program> program{};
	static Material mat{};

	BlitMultiSample(src, dst, mat);
}
