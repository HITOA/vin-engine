#pragma once

#include <memory>

#include <GLFW/glfw3.h>

#define VIN_DEFAULT_WINDOW_WIDTH 600
#define VIN_DEFAULT_WINDOW_HEIGHT 400
#define VIN_DEFAULT_WINDOW_TITLE "vin engine"

namespace Vin {

	class Window {
	public:
		struct GlfwWindowDeleter {
			void operator()(GLFWwindow* p) {
				glfwDestroyWindow(p);
			}
		};
	public:
		Window();

		void SetWindowSize(int width, int height);
		int GetWindowWidth();
		int GetWindowHeight();

		void SetWindowTitle(const char* title);
		const char* GetWindowTitle();

		void SwapBuffer();

		bool ShouldClose();

	private:
		static void GlfwErrorCallback(int errcode, const char* description);
	private:
		int wWidth{ VIN_DEFAULT_WINDOW_WIDTH };
		int wHeight{ VIN_DEFAULT_WINDOW_HEIGHT };
		const char* wTitle{ VIN_DEFAULT_WINDOW_TITLE };

		std::unique_ptr<GLFWwindow, GlfwWindowDeleter> window{};
	};
}
