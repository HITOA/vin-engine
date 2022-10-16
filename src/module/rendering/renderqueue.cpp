#include "D:/VIN/vin-engine/build/CMakeFiles/vin_engine.dir/Debug/cmake_pch.hxx"
#include "renderqueue.hpp"

void Vin::RenderQueue::PushRenderTask(std::shared_ptr<Camera> camera, Primitive& primitive)
{
	m_Queue.push_back(RenderTask{ camera, primitive });
}

void Vin::RenderQueue::Sort()
{
	std::sort(m_Queue.begin(), m_Queue.end(), [](RenderTask& a, RenderTask& b) {
		return a.GetId() > b.GetId();
		});
}
