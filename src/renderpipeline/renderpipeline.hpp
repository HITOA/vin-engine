#pragma once

#include "vinpch.hpp"
#include "rendercontext.hpp"
#include "assets/asset.hpp"
#include "renderer/renderer.hpp"
#include "scene/mesh.hpp"
#include "scene/light.hpp"

#define VIN_RENDERCONTEXT_ASSETPATH "//Core/RenderContext"

namespace Vin {

	/**
	 * Base class for creating render pipeline / module.
	 */

	class RenderPipeline {
	protected:
		template<typename CtxT, typename... Args>
		void InitializeContext(Args... args) {
			Renderer::Init();

			m_Ctx = std::make_shared<CtxT>(args...);
			AssetDatabase::AddAsset<RenderContext>(m_Ctx, VIN_RENDERCONTEXT_ASSETPATH);
		}

		template<typename CtxT>
		std::shared_ptr<CtxT> GetContext() {
			return std::static_pointer_cast<CtxT, RenderContext>(m_Ctx);
		}
		
		void SetupMaterial(Material* mat);
		void SetupMainLight(Material* mat, Light light);
		void SetupAddtionalLight(Material* mat, Light* lights, int lightsCount);

		void Draw(Primitive* primitive);

	private:
		std::shared_ptr<RenderContext> m_Ctx{};
	};

}