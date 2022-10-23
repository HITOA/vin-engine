#pragma once

#include "core/application.hpp"
#include "assets/asset.hpp"
#include "rendercontext.hpp"
#include "renderer/framebuffer.hpp"

namespace Vin {

	class RenderingModule : public Module {
	public:
		virtual void Init();
		virtual void Render();
		virtual void OnEvent(EventHandler handler);

		virtual void RenderScene(RenderQueue& queue);
		virtual void GenerateShadowMap(RenderQueue& queue);

	private:
		Asset<RenderContext> m_Ctx{};
		std::shared_ptr<RenderTarget> m_ShadowmapRenderTarget{};
		std::shared_ptr<Program> m_ShadowmapProgram{};
		Material m_ShadowmapMaterial{};
	};

}
