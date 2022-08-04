#pragma once

#include "vinpch.hpp"

#include "core/core.hpp"

#define EVENT(type, category) type, category, #type

namespace Vin {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyHold, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	struct Event {
	public:
		const EventType type;
		const int category;
		const char* eventName;
	private:
		const void* data;

	public:
		Event(EventType type, int category, const char* eventName) :
			type{ type },
			category{ category },
			eventName{ eventName },
			data{ nullptr } {};

		~Event() {
			DestroyData();
		}
	public:
		template<typename T>
		inline constexpr const T& GetData() const {
			return *(const T*)data;
		}

		template<typename T, typename... Args>
		inline void CreateData(Args&&... args) {
			data = new T{ args... };
		}

		inline void DestroyData() {
			if (data != nullptr)
				delete data;
		}
	};

	class EventListener {
	public:
		virtual void OnEvent(const Event& e) = 0;
		virtual bool CheckCategory(int category);
	};

	class EventDispatcher {
	public:
		void RegisterListener(EventListener* listener);
	protected:
		void Dispatch(const Event& e);
	private:
		std::vector<EventListener*> m_listeners{};
	};
}
