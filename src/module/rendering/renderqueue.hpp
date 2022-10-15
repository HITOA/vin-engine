#pragma once

#include "vinpch.hpp"

#include "rendertask.hpp"

namespace Vin {
	class RenderQueue {
	public:
		void Sort();

		std::vector<RenderTask>::iterator begin() { return m_Queue.begin(); }
		std::vector<RenderTask>::iterator end() { return m_Queue.end(); }
		std::vector<RenderTask>::const_iterator begin() const { return m_Queue.begin(); }
		std::vector<RenderTask>::const_iterator end() const { return m_Queue.end(); }
		std::vector<RenderTask>::const_iterator cbegin() const { return m_Queue.cbegin(); }
		std::vector<RenderTask>::const_iterator cend() const { return m_Queue.cend(); }
	private:
		std::vector<RenderTask> m_Queue{};
	};
}
