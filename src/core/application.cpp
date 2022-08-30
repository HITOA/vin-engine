#include "application.hpp"

void Vin::App::ClearModule()
{
	for (auto mod : m_Modules)
		delete mod;
	m_Modules.clear();
}

void Vin::App::DispatchEvent(EventHandler handler)
{
	for (auto mod : m_Modules)
		mod->OnEvent(handler);
	handler.Unbind();
}

void Vin::App::Run()
{
	m_Running = true;

	TimeStep last = m_Timer.GetTimeStep();
	TimeStep ulast = m_Timer.GetTimeStep();

	for (auto mod : m_Modules)
		mod->Start();

	while (m_Running) {
		TimeStep current = m_Timer.GetTimeStep();
		TimeStep elapsed = current - last;
		last = current;

		m_CurrentDeltaTime = elapsed;
		for (auto mod : m_Modules)
			mod->Process();

		if (((current - ulast).GetMillisecond() + elapsed.GetMillisecond()) > GetMsPerUpdate() * 1.05) {
			TimeStep uelapsed = current - ulast;
			ulast = current;
			m_CurrentDeltaTime = uelapsed;
			for (auto mod : m_Modules)
				mod->EarlyUpdate();
			for (auto mod : m_Modules)
				mod->Update();
			for (auto mod : m_Modules)
				mod->LateUpdate();
			for (auto mod : m_Modules)
				mod->Render();
		}

		m_Timer.Wait(current.GetMillisecond() + GetMsPerProcess() - m_Timer.GetElapsedMillisecond());
	}

	for (auto mod : m_Modules)
		mod->Stop();
}

void Vin::App::Stop()
{
	m_Running = false;
}

Vin::AssetDatabase* Vin::App::GetAssetDatabase() {
	return &m_AssetDatabase;
}

void Vin::App::SetAppInfo(AppInfo appInfo)
{
	m_AppInfo = appInfo;
}

Vin::AppInfo Vin::App::GetAppInfo()
{
	return m_AppInfo;
}

void Vin::App::SetProcessRate(double rate)
{
	m_ProcessRate = rate;
}

void Vin::App::SetUpdateRate(double rate)
{
	m_UpdateRate = rate;
}

double Vin::App::GetProcessRate()
{
	return m_ProcessRate;
}

double Vin::App::GetUpdateRate()
{
	return m_UpdateRate;
}

Vin::TimeStep Vin::App::GetDeltaTime() {
	return m_CurrentDeltaTime;
}

double Vin::App::GetMsPerProcess()
{
	return (1000 / m_ProcessRate);
}

double Vin::App::GetMsPerUpdate()
{
	return (1000 / m_UpdateRate);
}
