#pragma once

#include "vinpch.hpp"

#include "core/window.hpp"
#include "core/timer.hpp"
#include "events/event.hpp"
#include "core/module.hpp"

int main(int argc, char* argv[]);

namespace Vin {
	struct ApplicationInfo {
		const char* name{ "Application" };
	};

	class Application : public EventListener, public EventDispatcher {
	public:
		Application(const ApplicationInfo& info);
		~Application();

	public:
		void OnEvent(const Event& e);

		void Run();
		void Stop();

		void SetProcessRate(double rate);
		void SetUpdateRate(double rate);

		void AddModule(eastl::unique_ptr<Module> mod);
		
	private:
		double GetMsPerProcess();
		double GetMsPerUpdate();

	private:
		ApplicationInfo m_ApplicationInfo;
		std::unique_ptr<Window> m_Window;
		ModuleList m_ModuleList{};
		bool m_Running;
		VinTimer m_Timer;
		
		double m_ProcessRate{120};
		double m_UpdateRate{60};

		double m_lastProcessDelay;
		double m_lastUpdateDelay;
	};

	Application* CreateApp();
	void DestroyApp(Application* app);
}
