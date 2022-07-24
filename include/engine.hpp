#pragma once

#include <memory>

#include <renderer.hpp>
#include <window.hpp>
#include <functional>

namespace Vin {
	enum class RenderingAPI {
		NONE,
		OPENGL
	};

	class Engine {
	public:
		Engine() = delete;
		Engine(RenderingAPI renderingApi);

		void Run();

		Renderer* GetRenderer();
		Window* GetWindow();

		void SetProcessCallback(std::function<void()> callback);

	private:
		bool CreateRenderer(RenderingAPI renderingApi);
		void GameLoop();

	private:
		uint64_t frameDrawn{0};

		uint32_t targetFps{60};

		std::function<void()> processcallback{};

		std::unique_ptr<Renderer> renderer{};
		std::unique_ptr<Window> window{};
	};
}
