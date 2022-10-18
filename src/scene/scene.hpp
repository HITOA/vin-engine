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
		}

		Registry<layout>* operator->() {
			return &m_Registry;
		}
		
	private:
		static void MeshRendererSystem(Query<layout, Transform<float>, MeshRenderer> query) {
			Asset<RenderContext> ctx = AssetDatabase::GetAsset<RenderContext>(VIN_RENDERCONTEXT_BASEPATH);

			for (auto& [transform, meshrenderer] : query) {
				if (!meshrenderer->isDynamicMesh) {
					StaticMesh* mesh = meshrenderer->staticmesh;
					for (auto& primitive : *mesh) {
						ctx->queue.PushRenderTask(s_CurrentCamera, primitive, transform->GetModelMatrix());
					}
				}
			}
		}

	private:
		static std::shared_ptr<Camera> s_CurrentCamera;

		Registry<layout> m_Registry{};
	};

	template<ArchetypeMemoryLayout layout>
	std::shared_ptr<Camera> Scene<layout>::s_CurrentCamera{};
}