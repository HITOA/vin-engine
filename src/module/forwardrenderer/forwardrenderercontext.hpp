#pragma once

#include "renderpipeline/rendercontext.hpp"
#include "renderpipeline/renderqueue.hpp"
#include "renderpipeline/rendertask.hpp"
#include "scene/material.hpp"
#include "scene/camera.hpp"
#include "scene/mesh.hpp"
#include "scene/meshrenderer.hpp"
#include "scene/light.hpp"

#define VIN_FORWARDRENDERER_MAX_LIGHT_COUNT 8

namespace Vin {
	
	struct ForwardRenderTaskData {
		std::shared_ptr<Camera> camera;
		Primitive* primitive;
		Matrix4x4<float> model;
	};

	template<>
	struct Identifier<ForwardRenderTaskData> {
		static unsigned long long GetId(ForwardRenderTaskData& v) {
			unsigned long long id{ 0 };

			unsigned char viewportId = v.camera->GetId();
			unsigned char transluencyType = v.primitive->material->GetTransparency() ? 0 : 1;
			unsigned int depth = 0; //Calculate depth
			unsigned int materialId = v.primitive->material->GetId();

			id += viewportId;
			id = (id << 4) + transluencyType;
			id = (id << 24) + (depth & 0xFFFFFF);
			id = (id << 32) + (materialId);
			
			return id;
		}
	};

	class ForwardRendererContext : public RenderContext {
	public:
		void RenderScene(Registry& registry, std::shared_ptr<Camera> camera);

		RenderQueue<ForwardRenderTaskData>& GetRenderQueue();

	private:
		RenderQueue<ForwardRenderTaskData> m_Queue{};

		Light m_MainLight{};

		int m_AdditionalLightsCount{ 0 };
		Light m_AdditionalLights[VIN_FORWARDRENDERER_MAX_LIGHT_COUNT]{};

		friend class ForwardRendererModule;

		friend void ForwardRendererSystem(Vin::Registry& registry, Vin::Query<Vin::Transform<float>, Vin::MeshRenderer> query,
			Vin::RenderQueue<Vin::ForwardRenderTaskData>& queue, std::shared_ptr<Vin::Camera> camera);
		friend void ForwardRendererLightSystem(Vin::Query<Vin::Light> query, Vin::ForwardRendererContext* ctx);
	};

}
