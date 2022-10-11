#pragma once

#include "vinpch.hpp"

#include "renderqueue.hpp"
#include "scene/camera.hpp"

namespace Vin {
	class RenderContext {
	private:
		std::shared_ptr<Camera> m_Camera{};
		
	};
}
