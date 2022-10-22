#include "renderingmodule.hpp"

#include "renderer/renderer.hpp"
#include "module/windowing/windowmodule.hpp"

#include "vinshadowvs.hpp"
#include "vinshadowfs.hpp"

#include <optick.h>

void Vin::RenderingModule::Init()
{
	Renderer::Init();

	m_Ctx = AssetDatabase::AddAsset(RenderContext{}, VIN_RENDERCONTEXT_BASEPATH);

	m_ShadowmapProgram = Program::Create();
	m_ShadowmapProgram->AddShader(ShaderType::VertexShader, vinshadowvs);
	m_ShadowmapProgram->AddShader(ShaderType::FragmentShader, vinshadowfs);
	m_ShadowmapProgram->CompileProgram();

	m_ShadowmapMaterial = Material{ m_ShadowmapProgram };
}

void Vin::RenderingModule::Render()
{
	OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Rendering);

	RenderQueue& queue = m_Ctx->queue;
	queue.Sort();

	GenerateShadowMap(queue);
	RenderScene(queue);
	
	queue.Clear();
}

void Vin::RenderingModule::OnEvent(EventHandler handler)
{
	if (WindowResizeEvent* event = handler.GetEvent<WindowResizeEvent>())
		Renderer::SetViewport(0, 0, event->width, event->height);
}

void Vin::RenderingModule::RenderScene(RenderQueue& queue)
{
	OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::GPU_Scene);

	std::shared_ptr<Camera> currCamera{};
	Matrix4x4<float> projection{};
	Matrix4x4<float> view{};


	Matrix4x4<float> lightprojection = Orthographic<float>(
		-m_Ctx->mainLight.shadow.distance, m_Ctx->mainLight.shadow.distance,
		m_Ctx->mainLight.shadow.distance, -m_Ctx->mainLight.shadow.distance, 0.1f, 100.0f);
	Matrix4x4<float> lightview{};
	std::shared_ptr<RenderTexture> shadowMap = m_ShadowmapRenderTarget->GetTexture(0);

	unsigned int lastMatId{ 0 };

	for (RenderTask& task : queue) {
		if (task.GetCamera() != currCamera) {
			currCamera = task.GetCamera();
			currCamera->Bind();

			projection = currCamera->GetProjectionMatrix();
			view = currCamera->GetViewMatrix();

			lightview = Quaternion<float>::LookAt(-m_Ctx->mainLight.direction, Vector3<float>{0, 0, 0}).GetRotationMatrix()*
				Translate<float>(-m_Ctx->mainLight.direction * 25);

			Renderer::Clear(currCamera->clearColor.x, currCamera->clearColor.y, currCamera->clearColor.z, currCamera->clearColor.w);
		}

		Primitive& primitive = task.GetPrimitive();

		/* MATRIX INFORMATION */

		if (lastMatId != primitive.material->GetId()) {
			primitive.material->Bind();

			if (primitive.material->GetDoubleSided())
				Renderer::SetCullMode(CullMode::None);
			else
				Renderer::SetCullMode(CullMode::Back);

			if (primitive.material->GetTransparency())
				Renderer::SetBlendMode(BlendMode::Blend);
			else
				Renderer::SetBlendMode(BlendMode::None);

			primitive.material->SetMat4("vin_matrix_view", view);
			primitive.material->SetMat4("vin_matrix_projection", projection);
		}

		primitive.material->SetMat4("vin_matrix_model", task.GetModel());
		primitive.material->SetMat4("vin_matrix_mvp", (projection * view * task.GetModel()));
		//primitive.material->SetMat4("vin_matrix_mvp", (lightprojection * lightview * task.GetModel()));
		primitive.material->SetMat4("vin_matrix_lightspace", (lightprojection * lightview * task.GetModel()));

		/* MAINLIGHT INFORMATION */

		primitive.material->SetFloat3("_MainLight.direction", m_Ctx->mainLight.direction);
		primitive.material->SetFloat4("_MainLight.color", m_Ctx->mainLight.color);
		primitive.material->SetFloat("_MainLight.intensity", m_Ctx->mainLight.intensity);

		/* SHADOW INFORMATION */

		primitive.material->SetTexture("_ShadowMap", shadowMap);
		primitive.material->SetFloat("_ShadowBias", m_Ctx->mainLight.shadow.bias);

		/* RENDERING */

		if (primitive.indexed) {
			Renderer::DrawIndexed(primitive.vao);
		}
		else {
			Renderer::DrawArrays(primitive.vao, primitive.vertexCount);
		}

	}

	if (currCamera)
		currCamera->Unbind();
}

void Vin::RenderingModule::GenerateShadowMap(RenderQueue& queue)
{
	OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::GPU_Shadows);

	RenderTargetSpecification shadowmapspec{ 
		(size_t)((int)m_Ctx->mainLight.shadow.resolution.x * (m_Ctx->mainLight.shadow.distance / 5)) , 
		(size_t)((int)m_Ctx->mainLight.shadow.resolution.y * (m_Ctx->mainLight.shadow.distance / 5)), 1 };
	shadowmapspec.AddRenderBuffer({ RenderBufferFormat::DEPTH_COMPONENT24, true });

	if (m_ShadowmapRenderTarget.get() == nullptr)
		m_ShadowmapRenderTarget = RenderTarget::Create(shadowmapspec);
	
	if (m_ShadowmapRenderTarget->GetSpecification().width != shadowmapspec.width ||
		m_ShadowmapRenderTarget->GetSpecification().height != shadowmapspec.height)
		m_ShadowmapRenderTarget->Resize(shadowmapspec.width, shadowmapspec.height);

	Renderer::SetViewport(0, 0, shadowmapspec.width, shadowmapspec.height);

	m_ShadowmapRenderTarget->Bind();

	Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	Renderer::SetCullMode(m_Ctx->mainLight.shadow.cullMode);

	std::shared_ptr<Camera> currCamera{};
	Matrix4x4<float> projection = Orthographic<float>(
		-m_Ctx->mainLight.shadow.distance, m_Ctx->mainLight.shadow.distance,
		m_Ctx->mainLight.shadow.distance, -m_Ctx->mainLight.shadow.distance, 0.1f, 100.0f);
	Matrix4x4<float> view{};

	m_ShadowmapMaterial.Bind();

	for (RenderTask& task : queue) {
		if (task.GetCamera() != currCamera) {
			currCamera = task.GetCamera();

			view = Quaternion<float>::LookAt(-m_Ctx->mainLight.direction, Vector3<float>{0, 0, 0}).GetRotationMatrix() *
				Translate<float>(-m_Ctx->mainLight.direction * 25);
		}

		Primitive& primitive = task.GetPrimitive();

		/* MATERIAL */

		m_ShadowmapMaterial.SetMat4("vin_matrix_lightspace", projection * view * task.GetModel());

		/* RENDERING */

		if (primitive.indexed) {
			Renderer::DrawIndexed(primitive.vao);
		}
		else {
			Renderer::DrawArrays(primitive.vao, primitive.vertexCount);
		}

	}

	Renderer::SetCullMode(CullMode::Back);
	m_ShadowmapRenderTarget->Unbind();

	Asset<WindowInfo> windowInfo = AssetDatabase::GetAsset<WindowInfo>(VIN_WINDOWINFO_ASSETPATH);
	Renderer::SetViewport(0, 0, windowInfo->width, windowInfo->height);
}
