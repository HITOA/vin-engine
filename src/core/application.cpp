#include "application.hpp"

#include "core/events/inputevent.hpp"
#include "core/logger/logger.hpp"
#include "renderer/renderer.hpp"
#include "core/filesystem/gamefilesystem.hpp"

Vin::Application::Application(const ApplicationInfo& info) : m_ApplicationInfo{ info }, m_Running{ false }, m_Timer{}
{
	Logger::Log("App \"{}\" started.", info.name);

	GameFilesystem::Init();
	Renderer::Init();

	m_Window = CreateWindow(WindowInfo{ info.name });
	m_Window->RegisterListener(this);

	Renderer::InitApi();
}

Vin::Application::~Application()
{
	Renderer::Terminate();
	GameFilesystem::Terminate();
}

void Vin::Application::OnEvent(const Event& e)
{
	Dispatch(e);

	if (e.type == EventType::WindowClose)
		Stop();
}

void Vin::Application::Run()
{
	m_Running = true;

	TimeStep last = m_Timer.GetTimeStep();
	TimeStep ulast = m_Timer.GetTimeStep();

	m_ModuleList.OnStart();

	while (m_Running) {
		TimeStep current = m_Timer.GetTimeStep();
		TimeStep elapsed = current - last;
		last = current;

		m_ModuleList.OnProcess(elapsed);

		if (((current - ulast).GetMillisecond() + elapsed.GetMillisecond()) > GetMsPerUpdate() * 1.05) {
			TimeStep uelapsed = current - ulast;
			ulast = current;
			m_ModuleList.OnUpdate(uelapsed);
			m_ModuleList.OnRender(uelapsed);
			m_Window->OnUpdate();
		}

		m_Timer.Wait(current.GetMillisecond() + GetMsPerProcess() - m_Timer.GetElapsedMillisecond());
	}

	m_ModuleList.OnStop();
}

void Vin::Application::Stop()
{
	m_Running = false;
}

void Vin::Application::SetProcessRate(double rate)
{
	m_ProcessRate = rate;
}

void Vin::Application::SetUpdateRate(double rate)
{
	m_UpdateRate = rate;
}

void Vin::Application::AddModule(eastl::unique_ptr<Module> mod)
{
	m_ModuleList.AddModule(eastl::move(mod));
}

double Vin::Application::GetMsPerProcess()
{
	return (1000 / m_ProcessRate);
}

double Vin::Application::GetMsPerUpdate()
{
	return (1000 / m_UpdateRate);
}
