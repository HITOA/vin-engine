#include "renderingmodule.hpp"

#include "renderer/renderer.hpp"
#include "module/windowing/windowmodule.hpp"
#include "assets/assetdatabase.hpp"
#include "rendercontext.hpp"

void Vin::RenderingModule::Init()
{
	Renderer::Init();

	AssetDatabase::AddAsset(RenderContext{}, VIN_RENDERCONTEXT_BASEPATH);
}

void Vin::RenderingModule::OnEvent(EventHandler handler)
{
	if (WindowResizeEvent* event = handler.GetEvent<WindowResizeEvent>())
		Renderer::SetViewport(0, 0, event->width, event->height);
}
