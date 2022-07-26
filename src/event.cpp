#include "event.hpp"

bool Vin::EventListener::CheckCategory(int category)
{
	return true;
}

void Vin::EventDispatcher::RegisterListener(EventListener* listener)
{
	listeners.push_back(listener);
}

void Vin::EventDispatcher::Dispatch(const Event& e)
{
	for (auto listener : listeners) {
		if (listener->CheckCategory(e.category))
			listener->OnEvent(e);
	}
}
