#include "forwardrenderercontext.hpp"

#include "scene/meshrenderer.hpp"
#include "logger/logger.hpp"

void Vin::ForwardRendererSystem(Vin::Registry& registry, Vin::Query<Vin::Transform<float>, Vin::MeshRenderer> query,
	Vin::RenderQueue<Vin::ForwardRenderTaskData>& queue, std::shared_ptr<Vin::Camera> camera)
{
	for (auto [transform, meshrenderer] : query) {
		if (!meshrenderer->isDynamicMesh) {
			Vin::StaticMesh* mesh = meshrenderer->staticmesh;
			for (auto& primitive : *mesh) {
				if (primitive.material)
					queue.Push(Vin::ForwardRenderTaskData{ camera, &primitive, transform->GetModelMatrix(registry) });
			}
		}
	}
}

void Vin::ForwardRendererLightSystem(Vin::Query<Vin::Light> query, Vin::ForwardRendererContext* ctx)
{
	bool mainLightAdded{ false };

	for (auto [light] : query) {
		if (light->mainLight) {
			if (mainLightAdded) {
				Logger::Log("You can't have >1 main light.");
				continue;
			}
			if (light->type != LightType::Directional) {
				Logger::Log("Main light should only be directional.");
				continue;
			}
			ctx->m_MainLight = *light;
		}
		else {
			if (ctx->m_AdditionalLightsCount >= VIN_FORWARDRENDERER_MAX_LIGHT_COUNT)
				continue;

			ctx->m_AdditionalLights[ctx->m_AdditionalLightsCount] = *light;
			++ctx->m_AdditionalLightsCount;
		}
	}
}

void Vin::ForwardRendererContext::RenderScene(Registry& registry, std::shared_ptr<Camera> camera)
{
	registry.Process(Vin::ForwardRendererSystem, m_Queue, camera);
	registry.Process(Vin::ForwardRendererLightSystem, this);
}

Vin::RenderQueue<Vin::ForwardRenderTaskData>& Vin::ForwardRendererContext::GetRenderQueue()
{
	return m_Queue;
}

void Vin::ForwardRendererContext::Clear()
{
	m_AdditionalLightsCount = 0;
	m_Queue.Clear();
}
