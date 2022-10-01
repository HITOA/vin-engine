#pragma once

#include "vinpch.hpp"
#include "core/application.hpp"
#include "renderer/graphicscontext.hpp"
#include "assets/asset.hpp"

#define VIN_WINDOWINFO_ASSETID MAX_ASSET_ID_VALUE - 1

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

	struct GlfwWindowHolder {
		GLFWwindow* window;
	};

	class WindowModule : public Module {
	public:
		void Init() final;
		void EarlyUpdate() final;
		void Render() final;
	private:
		Asset<WindowInfo> m_Info;
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		friend void WindowResizeCallback(GLFWwindow* window, int width, int height);
		friend void WindowCloseCallback(GLFWwindow* window);
		friend void WindowDropCallback(GLFWwindow* window, int count, const char** paths);
	};

}
