#pragma once

#include "vinpch.hpp"
#include "core/application.hpp"
#include "renderer/graphicscontext.hpp"
#include "assets/asset.hpp"

#define VIN_WINDOWINFO_ASSETPATH "//Core/WindowInfo"

struct GLFWwindow;

namespace Vin {
	struct WindowResizeEvent {
		int width, height;
	};

	struct WindowCloseEvent {
		bool closed;
	};

	struct WindowDropEvent {
		int count;
		const char** paths;
	};

	struct WindowInfo {
		const char* title{ "Application" };
		int width{ 1600 };
		int height{ 900 };
	};

	struct WindowMouseState {
		enum State {
			Normal,
			Lock
		} state;
	};

	struct GlfwWindowHolder {
		GLFWwindow* window;
	};

	class WindowModule : public Module {
	public:
		void Init() final;
		void EarlyUpdate() final;
		void LateRender() final;
		void OnEvent(EventHandler handler);
	private:
		Asset<WindowInfo> m_Info;
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		friend void WindowResizeCallback(GLFWwindow* window, int width, int height);
		friend void WindowCloseCallback(GLFWwindow* window);
		friend void WindowDropCallback(GLFWwindow* window, int count, const char** paths);
		friend void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void WindowCursordPosCallback(GLFWwindow* window, double xpos, double ypos);
	};

}
