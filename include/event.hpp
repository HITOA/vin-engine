#pragma once

#include <core.hpp>
#include <vinpch.hpp>

namespace Vin {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyHold
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
		Event(EventType type, int category, const char* eventName, void* data) :
			type{ type },
			category{ category },
			eventName{ eventName },
			data{ data } {};
	public:
		template<typename T>
		inline constexpr const T& GetData() const {
			return *(const T*)data;
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
		std::vector<EventListener*> listeners{};
	};
}
