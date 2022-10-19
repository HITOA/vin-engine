#include "renderingmodule.hpp"

#include "renderer/renderer.hpp"
#include "module/windowing/windowmodule.hpp"

void Vin::RenderingModule::Init()
{
	Renderer::Init();

	ctx = AssetDatabase::AddAsset(RenderContext{}, VIN_RENDERCONTEXT_BASEPATH);
}

void Vin::RenderingModule::Render()
{
	RenderQueue& queue = ctx->queue;
	queue.Sort();

	std::shared_ptr<Camera> currCamera{};
	Matrix4x4<float> projection{};
	Matrix4x4<float> view{};

	unsigned int lastMatId{ 0 };

	for (RenderTask& task : queue) {
		if (task.GetCamera() != currCamera) {
			currCamera = task.GetCamera();
			currCamera->Bind();

			projection = currCamera->GetProjectionMatrix();
			view = currCamera->GetViewMatrix();

			Renderer::Clear(0.85, 0.85, 1.0f, 1.0f);
		}

		Primitive& primitive = task.GetPrimitive();

		if (lastMatId != primitive.material->GetId()) {
			primitive.material->Bind();
			primitive.material->SetMat4("vin_matrix_view", view.data);
			primitive.material->SetMat4("vin_matrix_projection", projection.data);
		}
		
		primitive.material->SetMat4("vin_matrix_model", task.GetModel().data);
		primitive.material->SetMat4("vin_matrix_mvp", (projection * view * task.GetModel()).data);
		
		if (primitive.indexed) {
			Renderer::DrawIndexed(primitive.vao);
		}
		else {
			Renderer::DrawArrays(primitive.vao, primitive.vertexCount);
		}

	}

	if (currCamera)
		currCamera->Unbind();

	queue.Clear();
}

void Vin::RenderingModule::OnEvent(EventHandler handler)
{
	if (WindowResizeEvent* event = handler.GetEvent<WindowResizeEvent>())
		Renderer::SetViewport(0, 0, event->width, event->height);
}
