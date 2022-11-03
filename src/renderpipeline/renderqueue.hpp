#pragma once

#include "vinpch.hpp"
#include "rendertask.hpp"

namespace Vin {

	template<typename T>
	class RenderQueue {
	public:
		void Push(T&& v) {
			m_Queue.push_back(RenderTask{ v });
		}

		void Sort() {
			std::sort(begin(), end(), [](RenderTask<T>& a, RenderTask<T>& b) {
				return a.id > b.id;
				});
		}

		void Clear() {
			m_Queue.clear();
		}

		typename std::vector<RenderTask<T>>::iterator begin() { return m_Queue.begin(); }
		typename std::vector<RenderTask<T>>::iterator end() { return m_Queue.end(); }
		typename std::vector<RenderTask<T>>::const_iterator begin() const { return m_Queue.begin(); }
		typename std::vector<RenderTask<T>>::const_iterator end() const { return m_Queue.end(); }
		typename std::vector<RenderTask<T>>::const_iterator cbegin() const { return m_Queue.cbegin(); }
		typename std::vector<RenderTask<T>>::const_iterator cend() const { return m_Queue.cend(); }

	private:
		std::vector<RenderTask<T>> m_Queue{};
	};

}
