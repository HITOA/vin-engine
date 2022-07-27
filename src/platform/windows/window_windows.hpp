#include <vinpch.hpp>
#include <core.hpp>

#ifdef VIN_PLATFORM_WINDOWS

#include <window.hpp>

#include <GLFW/glfw3.h>

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

	private:
		void Init(const WindowInfo& info);
		void Terminate();

	private:
		GLFWwindow* m_Window;

		struct WindowsWindowData {
			const char* title;
			int width;
			int height;
		} m_WindowData;
	};
}

#endif // VIN_PLATFORM_WINDOWS
