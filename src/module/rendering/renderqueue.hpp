#pragma once

#include "vinpch.hpp"

#include "rendertask.hpp"

namespace Vin {
	class RenderQueue {
	private:
		std::vector<RenderTask> m_Queue{};
	};
}
