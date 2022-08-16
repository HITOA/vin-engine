#pragma once

#include "vinpch.hpp"

#include "core/timer.hpp"

namespace Vin {

	class Module {
	public:
		virtual ~Module() {};

		virtual void OnStart() {};
		virtual void OnStop() {};
		virtual void OnProcess(TimeStep ts) {};
		virtual void OnUpdate(TimeStep ts) {};
		virtual void OnRender(TimeStep ts) {};
	};
	
	class ModuleList {
	public:
		~ModuleList();

		void OnStart();
		void OnStop();
		void OnProcess(TimeStep ts);
		void OnUpdate(TimeStep ts);
		void OnRender(TimeStep ts);

		void AddModule(eastl::unique_ptr<Module> mod);
	private:
		eastl::fixed_vector<eastl::unique_ptr<Module>, 32, false> m_Modules{};
	};
}
