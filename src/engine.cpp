#include "engine.hpp"

#include <glrenderer.hpp>
#include <logger.hpp>
#include <GLFW/glfw3.h>

Vin::Engine::Engine(RenderingAPI renderingApi)
{
	window = std::unique_ptr<Window>{ new Window{} };

	if (!CreateRenderer(renderingApi))
		Logger::Err("No valid rendering api.");

	if (!renderer->Init(glfwGetProcAddress))
		Logger::Err("Rendering api could not initialize.");

	renderer->SetViewport(0, 0, window->GetWindowWidth(), window->GetWindowHeight());
}

void Vin::Engine::Run()
{
	GameLoop();
}

bool Vin::Engine::CreateRenderer(RenderingAPI renderingApi)
{
	switch (renderingApi)
	{
	case RenderingAPI::OPENGL:
		renderer = std::unique_ptr<Renderer>{ new GlRenderer{} };
		break;
	default:
		return false;
	}

	return true;
}

void Vin::Engine::GameLoop()
{
	while (window->ShouldClose()) {
		renderer->Clear();

		window->SwapBuffer();
		glfwPollEvents();
	}
}
