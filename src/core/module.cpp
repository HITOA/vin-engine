#include "module.hpp"

Vin::ModuleList::~ModuleList()
{
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		it->~unique_ptr();

	m_Modules.clear();
}

void Vin::ModuleList::OnStart() {
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		(*it)->OnStart();
}

void Vin::ModuleList::OnStop() {
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		(*it)->OnStop();
}

void Vin::ModuleList::OnProcess(TimeStep ts) {
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		(*it)->OnProcess(ts);
}

void Vin::ModuleList::OnUpdate(TimeStep ts) {
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		(*it)->OnUpdate(ts);
}

void Vin::ModuleList::OnRender(TimeStep ts) {
	for (auto it = m_Modules.begin(), itend = m_Modules.end(); it < m_Modules.end(); ++it)
		(*it)->OnRender(ts);
}

void Vin::ModuleList::AddModule(eastl::unique_ptr<Module> mod) {
	m_Modules.push_back(eastl::move(mod));
}