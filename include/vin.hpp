#pragma once

#include <vinpch.hpp>

#include <window.hpp>

int main(int argc, char* argv[]);

namespace Vin {
	struct ApplicationInfo {
		const char* name{ "Application" };
	};

	class Application : public EventListener {
	public:
		Application(const ApplicationInfo& info);
		virtual ~Application();

	public:
		void OnEvent(const Event& e);

		void Run();
		void Stop();

	private:
		ApplicationInfo m_ApplicationInfo;
		Window* m_Window;
		bool m_Running;
	};

	Application* CreateApp();
	void DestroyApp(Application* app);
}
