#include "event.hpp"

bool Vin::EventListener::CheckCategory(int category)
{
	return true;
}

void Vin::EventDispatcher::RegisterListener(EventListener* listener)
{
	m_listeners.push_back(listener);
}

void Vin::EventDispatcher::Dispatch(const Event& e)
{
	for (auto listener : m_listeners) {
		if (listener->CheckCategory(e.category))
			listener->OnEvent(e);
	}
}
