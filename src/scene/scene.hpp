#pragma once

#include "ecs/registry.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "light.hpp"
#include "transform.hpp"
#include "meshrenderer.hpp"
#include "assets/assetdatabase.hpp"
#include "module/rendering/rendercontext.hpp"

namespace Vin{
	template<ArchetypeMemoryLayout layout>
	class Scene {
	public:
		void Render(std::shared_ptr<Camera> camera) {
			s_CurrentCamera = camera;

			m_Registry.Process(MeshRendererSystem);

			m_Registry.Process(LightRendererSystem);
		}

		Registry<layout>* operator->() {
			return &m_Registry;
		}
		
	private:
		static void MeshRendererSystem(Query<layout, Transform<float>, MeshRenderer> query) {
			Asset<RenderContext> m_Ctx = AssetDatabase::GetAsset<RenderContext>(VIN_RENDERCONTEXT_BASEPATH);

			for (auto& [transform, meshrenderer] : query) {
				if (!meshrenderer->isDynamicMesh) {
					StaticMesh* mesh = meshrenderer->staticmesh;
					for (auto& primitive : *mesh) {
						m_Ctx->queue.PushRenderTask(s_CurrentCamera, primitive, transform->GetModelMatrix());
					}
				}
			}
		}

		static void LightRendererSystem(Query<layout, Light> query) {
			Asset<RenderContext> m_Ctx = AssetDatabase::GetAsset<RenderContext>(VIN_RENDERCONTEXT_BASEPATH);

			for (auto& [light] : query) {
				if (light->type == LightType::Directional) {
					m_Ctx->mainLight = *light;
					continue;
				}

				//Additional Light
			}
		}

	private:
		static std::shared_ptr<Camera> s_CurrentCamera;

		Registry<layout> m_Registry{};
	};

	template<ArchetypeMemoryLayout layout>
	std::shared_ptr<Camera> Scene<layout>::s_CurrentCamera{};
}