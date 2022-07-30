#pragma once

#include "vinpch.hpp"

#include "core/window.hpp"
#include "core/timer.hpp"
#include "core/events/event.hpp"

int main(int argc, char* argv[]);

namespace Vin {
	struct ApplicationInfo {
		const char* name{ "Application" };
	};

	class Application : public EventListener, EventDispatcher {
	public:
		Application(const ApplicationInfo& info);

	public:
		void OnEvent(const Event& e);

		void Run();
		void Stop();

	private:
		ApplicationInfo m_ApplicationInfo;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		VinTimer m_Timer;

	};

	Application* CreateApp();
	void DestroyApp(Application* app);
}
