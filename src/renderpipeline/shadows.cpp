#include "shadows.hpp"

#include "vinshadowvsshader.hpp"
#include "vinshadowfsshader.hpp"

Vin::ShadowRenderer::ShadowRenderer()
{
	RenderTargetSpecification shadowmapspec{
		(uint32_t)(1024) ,
		(uint32_t)(1024), 1 };
	shadowmapspec.AddRenderBuffer({ RenderBufferFormat::DEPTH_COMPONENT24, true });

	m_ShadowMap = RenderTarget::Create(shadowmapspec);
	m_ShadowMapMatrix = Matrix4x4<float>{};

	std::shared_ptr<Program> shadowMapProgram = Program::Create();
	shadowMapProgram->AddShader(ShaderType::VertexShader, vinshadowvsshader);
	shadowMapProgram->AddShader(ShaderType::FragmentShader, vinshadowfsshader);
	shadowMapProgram->CompileProgram();

	m_ShadowMapMaterial = Material{ shadowMapProgram };
}

Vin::Matrix4x4<float> Vin::ShadowRenderer::GetShadowMapMatrix(Matrix4x4<float>& model)
{
	return m_ShadowMapMatrix * model;
}

std::shared_ptr<Vin::RenderTexture> Vin::ShadowRenderer::GetShadowMap()
{
	return m_ShadowMap->GetTexture(0);
}
