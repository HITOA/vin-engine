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
		void SetCamera(std::shared_ptr<Camera> camera) {
			m_Camera = camera;
		}

		void Render() {
			s_CurrentCamera = m_Camera;

			m_Registry.Process(BuildRenderQueue);
		}

		Registry<layout>* operator->() {
			return &m_Registry;
		}
		
	private:
		static void BuildRenderQueue(Query<layout, Transform<float>, MeshRenderer> query) {
			Asset<RenderContext> ctx = AssetDatabase::GetAsset<RenderContext>(VIN_RENDERCONTEXT_BASEPATH);

			for (auto& [transform, meshrenderer] : query) {
				if (!meshrenderer->isDynamicMesh) {
					StaticMesh* mesh = meshrenderer->staticmesh;
					for (auto& primitive : *mesh) {
						ctx->m_Queue.PushRenderTask(s_CurrentCamera, primitive, transform->GetModelMatrix());
					}
				}
			}
		}

	private:
		static std::shared_ptr<Camera> s_CurrentCamera;

		std::shared_ptr<Camera> m_Camera{};
		Registry<layout> m_Registry{};
	};

	template<ArchetypeMemoryLayout layout>
	std::shared_ptr<Camera> Scene<layout>::s_CurrentCamera{};
}