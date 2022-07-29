#pragma once

#include "vinpch.hpp"

#include "core/events/event.hpp"

namespace Vin {
	struct WindowInfo {
		const char* title;
		int width;
		int height;

		WindowInfo(const char* title = "Vin Window", int width = 600, int height = 400) :
			title{ title }, width{ width }, height{ height } {};
	};

	class Window : public EventDispatcher {
	public:
		virtual ~Window() = default;
		
	public:
		virtual void OnUpdate() = 0;

		virtual const char* GetTitle() const = 0;
		virtual int GetWidth() const = 0;
		virtual int Getheight() const = 0;

		virtual void* GetNativeWindow() const = 0;
	};

	Window* CreateWindow(const WindowInfo& info = WindowInfo{});
	void DestroyWindow(Window* window);
}
