#pragma once

#include "vinpch.hpp"

namespace Vin {

	template<typename T>
	class RenderQueue {
	public:
		void Push(T&& v) {
			m_Queue.push_back(v);
		}

		void Sort() {
			std::sort(begin(), end(), [](T& a, T& b) {
				return a.id > b.id;
				});
		}

		void Clear() {
			m_Queue.clear();
		}

		typename std::vector<T>::iterator begin() { return m_Queue.begin(); }
		typename std::vector<T>::iterator end() { return m_Queue.end(); }
		typename std::vector<T>::const_iterator begin() const { return m_Queue.begin(); }
		typename std::vector<T>::const_iterator end() const { return m_Queue.end(); }
		typename std::vector<T>::const_iterator cbegin() const { return m_Queue.cbegin(); }
		typename std::vector<T>::const_iterator cend() const { return m_Queue.cend(); }

	private:
		std::vector<T> m_Queue{};
	};

}
