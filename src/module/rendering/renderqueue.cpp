#include "renderqueue.hpp"

void Vin::RenderQueue::PushRenderTask(std::shared_ptr<Camera> camera, Primitive& primitive, Matrix4x4<float> model)
{
	queue.push_back(RenderTask{ camera, primitive, model });
}

void Vin::RenderQueue::Sort()
{
	std::sort(queue.begin(), queue.end(), [](RenderTask& a, RenderTask& b) {
		return a.GetId() > b.GetId();
		});
}

void Vin::RenderQueue::Clear()
{
	queue.clear();
}
