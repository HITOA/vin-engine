#include "forwardrenderercontext.hpp"

void Vin::ForwardRendererContext::RenderScene(Registry& registry, std::shared_ptr<Camera> camera)
{

}

Vin::RenderQueue<Vin::RenderTask<Vin::ForwardRenderTaskData>>& Vin::ForwardRendererContext::GetRenderQueue()
{
	return m_Queue;
}
