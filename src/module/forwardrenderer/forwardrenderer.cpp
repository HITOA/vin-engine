#include "forwardrenderer.hpp"

#include "forwardrenderercontext.hpp"

void Vin::ForwardRendererModule::Init()
{
	InitializeContext<ForwardRendererContext>();
}

void Vin::ForwardRendererModule::Render()
{
	std::shared_ptr<ForwardRendererContext> ctx = GetContext<ForwardRendererContext>();
	RenderQueue<ForwardRenderTaskData>& queue = ctx->GetRenderQueue();

	queue.Sort();

	m_ShadowRenderer.Render(ctx->m_MainLight, queue);

	std::shared_ptr<RenderTexture> shadowMap = m_ShadowRenderer.GetShadowMap();

	std::shared_ptr<Camera> currCamera{};

	Matrix4x4<float> projection{};
	Matrix4x4<float> view{};

	std::shared_ptr<Material> currMat{};

	for (RenderTask<ForwardRenderTaskData>& task : queue) {
		if (task.data.camera != currCamera) {
			currCamera = task.data.camera;
			currCamera->Bind();

			projection = currCamera->GetProjectionMatrix();
			view = currCamera->GetViewMatrix();

			Renderer::Clear(currCamera->clearColor.x, currCamera->clearColor.y, currCamera->clearColor.z, currCamera->clearColor.w);
		}

		Primitive* currPrimitive = task.data.primitive;

		if (currMat.get() == nullptr || currMat->GetId() != currPrimitive->material->GetId()) {
			if (currMat)
				currMat->Unbind();
			currMat = currPrimitive->material;
			currPrimitive->material->Bind();

			SetupMaterial(currPrimitive->material.get());
			SetupMainLight(currPrimitive->material.get(), ctx->m_MainLight);
			SetupAddtionalLight(currPrimitive->material.get(), ctx->m_AdditionalLights, ctx->m_AdditionalLightsCount);

			currPrimitive->material->GetProgram()->SetMat4("vin_matrix_view", view.data);
			currPrimitive->material->GetProgram()->SetMat4("vin_matrix_projection", projection.data);

			currPrimitive->material->GetProgram()->SetFloat3("_WorldSpaceCameraPosition", currCamera->position.data);
			
			int shadowMapLoc = currPrimitive->material->GetProgram()->GetField("_ShadowMap");
			if (shadowMapLoc != -1) {
				currPrimitive->material->GetProgram()->SetInt(shadowMapLoc, shadowMapLoc);
				shadowMap->Bind(shadowMapLoc);
			}
		}

		currPrimitive->material->GetProgram()->SetMat4("vin_matrix_model", task.data.model.data);
		currPrimitive->material->GetProgram()->SetMat4("vin_matrix_mvp", (projection * view * task.data.model).data);
		currPrimitive->material->GetProgram()->SetMat4("vin_matrix_lightspace", m_ShadowRenderer.GetShadowMapMatrix(task.data.model).data);

		Draw(currPrimitive);
	}

	if (currMat)
		currMat->Unbind();

	if (currCamera)
		currCamera->Unbind();

	ctx->Clear();
}
