#pragma once

#include "vinpch.hpp"

#include "rendertask.hpp"

namespace Vin {
	class RenderQueue {
	public:
		void PushRenderTask(std::shared_ptr<Camera> camera, Primitive& primitive, Matrix4x4<float> model);

		void Sort();
		void Clear();

		std::vector<RenderTask>::iterator begin() { return queue.begin(); }
		std::vector<RenderTask>::iterator end() { return queue.end(); }
		std::vector<RenderTask>::const_iterator begin() const { return queue.begin(); }
		std::vector<RenderTask>::const_iterator end() const { return queue.end(); }
		std::vector<RenderTask>::const_iterator cbegin() const { return queue.cbegin(); }
		std::vector<RenderTask>::const_iterator cend() const { return queue.cend(); }
	private:
		std::vector<RenderTask> queue{};
	};
}
