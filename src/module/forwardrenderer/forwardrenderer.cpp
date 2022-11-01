#include "forwardrenderer.hpp"

#include "forwardrenderercontext.hpp"

void Vin::ForwardRendererModule::Init()
{
	InitializeContext<ForwardRendererContext>();
}

void Vin::ForwardRendererModule::Render()
{
	std::shared_ptr<ForwardRendererContext> ctx = GetContext<ForwardRendererContext>();
	RenderQueue<RenderTask<ForwardRenderTaskData>>& queue = ctx->GetRenderQueue();

	queue.Sort();



	queue.Clear();
}
