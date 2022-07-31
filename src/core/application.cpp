#include "application.hpp"

#include "core/events/inputevent.hpp"
#include "core/logger.hpp"
#include "renderer/renderer.hpp"

Vin::Application::Application(const ApplicationInfo& info) : m_ApplicationInfo{ info }, m_Running{ false }, m_Timer{}
{
	Logger::Log("App \"%s\" started.", info.name);

	Renderer::Init();

	m_Window = CreateWindow(WindowInfo{ info.name });
	m_Window->RegisterListener(this);
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

	while (m_Running) {
		double start = m_Timer.GetElapsedMiliSecond();

		Renderer::Clear(0.5, 0.2, 0.2, 1);
		m_Window->OnUpdate();
	}
}

void Vin::Application::Stop()
{
	m_Running = false;
}
