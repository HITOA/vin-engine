#include "renderingmodule.hpp"

#include "renderer/renderer.hpp"
#include "module/windowing/windowmodule.hpp"

void Vin::RenderingModule::Init()
{
	Renderer::Init();
}

void Vin::RenderingModule::OnEvent(EventHandler handler)
{
	if (WindowResizeEvent* event = handler.GetEvent<WindowResizeEvent>())
		Renderer::SetViewport(0, 0, event->width, event->height);
}
