#pragma once

#include "vinpch.hpp"

#include "core/base.hpp">
#include "core/allocator.hpp"

#include "core/event.hpp"
#include "core/timer.hpp"

//#ifndef VINAPP_MAX_MODULE_COUNT
//#define VINAPP_MAX_MODULE_COUNT 128
//#endif

int main(int argc, char* argv[]);

namespace Vin {
	class Module;

	struct AppInfo {
		const char* name{ "Application" };
	};

	class App {
	public:
		virtual ~App() {
			ClearModule();
		};

		template<typename T, typename... Args>
		void AddModule(Args... args) {
			T* ptr = new T{ args... };
			ptr->m_App = this;
			m_Modules.insert(m_Modules.begin(), ptr);
			m_Modules[0]->Init();
		}

		template<typename T, typename... Args>
		void AddModuleLast(Args... args) {
			T* ptr = new T{ args... };
			ptr->m_App = this;
			m_Modules.push_back(ptr);
			m_Modules[m_Modules.size() - 1]->Init();
		}

		void ClearModule();

		void DispatchEvent(EventHandler handler);

		virtual void Build() = 0;

		void Run();
		void Stop();

		void SetAppInfo(AppInfo appInfo);
		AppInfo GetAppInfo();

		void SetProcessRate(double rate);
		void SetUpdateRate(double rate);
		double GetProcessRate();
		double GetUpdateRate();

		TimeStep GetDeltaTime();

	private:
		double GetMsPerProcess();
		double GetMsPerUpdate();

	private:
		AppInfo m_AppInfo{};
		std::vector<Module*> m_Modules{};

		bool m_Running;
		VinTimer m_Timer;

		TimeStep m_CurrentDeltaTime{ 0 };

		double m_ProcessRate{ 120 };
		double m_UpdateRate{ 60 };
		double m_lastProcessDelay;
		double m_lastUpdateDelay;
	};

	class Module {
	public:
		virtual ~Module() {};

		virtual void Init() {};

		virtual void Start() {};
		virtual void Stop() {};

		virtual void Process() {};
		virtual void EarlyUpdate() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() {};

		virtual void OnEvent(EventHandler handler) {};

	public:
		void DispatchEvent(EventHandler handler);
		inline App* GetApp() {
			return m_App;
		}
	private:
		App* m_App;
		friend class App;
	};

	App* CreateApp();
	void DestroyApp(App* app);
}
