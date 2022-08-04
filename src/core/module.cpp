#include "module.hpp"

Vin::ModuleList::~ModuleList()
{
	for (auto mod : m_Modules)
		delete mod;

	m_Modules.clear();
}

void Vin::ModuleList::OnStart() {
	for (auto mod : m_Modules)
		mod->OnStart();
}

void Vin::ModuleList::OnStop() {
	for (auto mod : m_Modules)
		mod->OnStop();
}

void Vin::ModuleList::OnProcess(TimeStep ts) {
	for (auto mod : m_Modules)
		mod->OnProcess(ts);
}

void Vin::ModuleList::OnUpdate(TimeStep ts) {
	for (auto mod : m_Modules)
		mod->OnUpdate(ts);
}

void Vin::ModuleList::OnRender(TimeStep ts) {
	for (auto mod : m_Modules)
		mod->OnRender(ts);
}

void Vin::ModuleList::AddModule(Module* mod) {
	m_Modules.push_back(mod);
}