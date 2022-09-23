#pragma once

#include "vinpch.hpp"
#include "core/application.hpp"
#include "renderer/graphicscontext.hpp"

#define VIN_WINDOWINFO_ASSETNAME "//WindowInfo"

struct GLFWwindow;

namespace Vin {

	enum class WindowEvent {
		Close,
		Resize
	};

	struct WindowResizeEvent {
		int width, height;
	};

	struct WindowCloseEvent {
		bool closed;
	};

	struct WindowInfo {
		const char* title{ "Application" };
		int width{ 600 };
		int height{ 400 };
	};

	class WindowModule : public Module {
	public:
		void Init() final;
		void EarlyUpdate() final;
		void Render() final;
	private:
		//Asset<WindowInfo> m_Info;
		GLFWwindow* m_Window;
		eastl::unique_ptr<GraphicsContext> m_Context;

		friend void WindowResizeCallback(GLFWwindow* window, int width, int height);
		friend void WindowCloseCallback(GLFWwindow* window);
	};

}
