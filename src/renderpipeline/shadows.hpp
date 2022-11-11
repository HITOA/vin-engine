#pragma once

#include "renderer/framebuffer.hpp"
#include "math/math.hpp"
#include "scene/light.hpp"
#include "scene/material.hpp"
#include "renderqueue.hpp"
#include "module/windowing/windowmodule.hpp"

namespace Vin {

	class ShadowRenderer {
	public:
		ShadowRenderer();

		template<typename T>
		void Render(Light srcLight, RenderQueue<T> queue) {
			RenderTargetSpecification shadowmapspec{
				(uint32_t)((int)srcLight.shadow.resolution.x * (srcLight.shadow.distance / 5)) ,
				(uint32_t)((int)srcLight.shadow.resolution.y * (srcLight.shadow.distance / 5)), 1 };
			shadowmapspec.AddRenderBuffer({ RenderBufferFormat::DEPTH_COMPONENT24, true });

			if (m_ShadowMap->GetSpecification().width != shadowmapspec.width ||
				m_ShadowMap->GetSpecification().height != shadowmapspec.height)
				m_ShadowMap->Resize(shadowmapspec.width, shadowmapspec.height);

			Renderer::SetViewport(0, 0, shadowmapspec.width, shadowmapspec.height);

			m_ShadowMap->Bind();

			Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
			Renderer::SetCullMode(srcLight.shadow.cullMode);

			Matrix4x4<float> projection = Orthographic<float>(
				-srcLight.shadow.distance, srcLight.shadow.distance,
				srcLight.shadow.distance, -srcLight.shadow.distance, 0.1f, 100.0f);
			Matrix4x4<float> view = Quaternion<float>::LookAt(-srcLight.direction, Vector3<float>{0, 0, 0}).GetRotationMatrix()*
				Translate<float>(-srcLight.direction * 25);

			m_ShadowMapMatrix = projection * view;

			m_ShadowMapMaterial.Bind();

			for (auto& task : queue) {
				m_ShadowMapMaterial.GetProgram()->SetMat4("vin_matrix_lightspace", (projection * view * task.data.model).data);
				if (task.data.primitive->indexed) {
					Renderer::DrawIndexed(task.data.primitive->vao);
				}
				else {
					Renderer::DrawArrays(task.data.primitive->vao, task.data.primitive->vertexCount);
				}
			}

			m_ShadowMapMaterial.Unbind();

			Renderer::SetCullMode(CullMode::Back);
			m_ShadowMap->Unbind();

			Asset<WindowInfo> windowInfo = AssetDatabase::GetAsset<WindowInfo>(VIN_WINDOWINFO_ASSETPATH);
			Renderer::SetViewport(0, 0, windowInfo->width, windowInfo->height);
		}

		Matrix4x4<float> GetShadowMapMatrix(Matrix4x4<float>& model);

		std::shared_ptr<RenderTexture> GetShadowMap();
	private:
		std::shared_ptr<RenderTarget> m_ShadowMap{};
		Matrix4x4<float> m_ShadowMapMatrix{};
		Material m_ShadowMapMaterial{};
	};

	class CascadeShadowRenderer {
		//Not implemented
	};

}
