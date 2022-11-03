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

	std::shared_ptr<Camera> currCamera{};

	Matrix4x4<float> projection{};
	Matrix4x4<float> view{};

	unsigned int currMatId{ 0 };

	for (RenderTask<ForwardRenderTaskData>& task : queue) {
		if (task.data.camera != currCamera) {
			currCamera = task.data.camera;
			currCamera->Bind();

			projection = currCamera->GetProjectionMatrix();
			view = currCamera->GetViewMatrix();

			Renderer::Clear(currCamera->clearColor.x, currCamera->clearColor.y, currCamera->clearColor.z, currCamera->clearColor.w);
		}

		Primitive* currPrimitive = task.data.primitive;

		if (currMatId != currPrimitive->material->GetId()) {
			currMatId = currPrimitive->material->GetId();
			currPrimitive->material->Bind();

			SetupMaterial(currPrimitive->material.get());
			SetupMainLight(currPrimitive->material.get(), ctx->m_MainLight);
			SetupAddtionalLight(currPrimitive->material.get(), ctx->m_AdditionalLights, ctx->m_AdditionalLightsCount);

			currPrimitive->material->GetProgram()->SetMat4("vin_matrix_view", view.data);
			currPrimitive->material->GetProgram()->SetMat4("vin_matrix_projection", projection.data);
		}

		currPrimitive->material->GetProgram()->SetMat4("vin_matrix_model", task.data.model.data);
		currPrimitive->material->GetProgram()->SetMat4("vin_matrix_mvp", (projection * view * task.data.model).data);

		Draw(currPrimitive);
	}

	if (currCamera)
		currCamera->Unbind();

	queue.Clear();
}
