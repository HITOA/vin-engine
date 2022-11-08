#pragma once

#include "renderpipeline/renderpipeline.hpp"
#include "ecs/registry.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "light.hpp"
#include "transform.hpp"
#include "meshrenderer.hpp"
#include "assets/asset.hpp"
#include "scene/camera.hpp"

#include <optick.h>

namespace Vin{

	class Scene {
	public:
		void Render(std::shared_ptr<Camera> camera) {
			OPTICK_CATEGORY(OPTICK_FUNC, Optick::Category::Scene);

			Asset<RenderContext> ctx = AssetDatabase::GetAsset<RenderContext>(VIN_RENDERCONTEXT_ASSETPATH);
			
			if (ctx.Get() == nullptr)
				return;

			ctx->RenderScene(m_Registry, camera);
		}

		Registry* operator->() {
			return &m_Registry;
		}
		
	private:
		Registry m_Registry{};
	};
}