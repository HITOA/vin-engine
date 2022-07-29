#include <vinpch.hpp>
#include <core.hpp>

#ifdef VIN_PLATFORM_WINDOWS

#include <window.hpp>

#include <GLFW/glfw3.h>
#include <renderer/graphicscontext.hpp>

namespace Vin {


	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowInfo& info);
		~WindowsWindow() final;

	public:
		void OnUpdate() final;

		const char* GetTitle() const final;
		int GetWidth() const final;
		int Getheight() const final;

		void* GetNativeWindow() const final;

		friend void WindowResizeCallback(GLFWwindow* window, int width, int height);
		friend void WindowCloseCallback(GLFWwindow* window);
		friend void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		friend void WindowMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		friend void WindowMouseMoveCallback(GLFWwindow* window, double xPos, double yPos);

	private:
		void Init(const WindowInfo& info);
		void Terminate();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowsWindowData {
			const char* title;
			int width;
			int height;
		} m_WindowData;
	};
}

#endif // VIN_PLATFORM_WINDOWS
