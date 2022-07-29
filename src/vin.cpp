#include "vin.hpp"

#include<events/input_event.hpp>

Vin::Application::Application(const ApplicationInfo& info) : m_ApplicationInfo{ info }, m_Running{ false }, m_Timer{}
{
	m_Window = CreateWindow(WindowInfo{ info.name });
	m_Window->RegisterListener(this);
}

Vin::Application::~Application()
{
	DestroyWindow(m_Window);
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

		m_Window->OnUpdate();
	}
}

void Vin::Application::Stop()
{
	m_Running = false;
}
