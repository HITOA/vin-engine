#include "application.hpp"

void Vin::Module::DispatchEvent(Vin::EventHandler handler)
{
	m_App->DispatchEvent(handler);
}
