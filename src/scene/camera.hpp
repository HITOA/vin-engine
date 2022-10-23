#pragma once

#include "core/identifiable.hpp"
#include "renderer/framebuffer.hpp"
#include "math/math.hpp"
#include "transform.hpp"

namespace Vin {

	class Camera : public Identifiable<Camera, unsigned char> {
	public:
		Camera() = delete;
		Camera(Vector2<uint32_t> size, uint32_t samples = 8, bool HDR = true);

		void SetNearPlane(float near);
		float GetNearPlane();

		void SetFarPlane(float far);
		float GetFarPlane();

		void SetFOV(float fov);
		float GetFOV();

		void Resize(Vector2<uint32_t> size);
		Vector2<uint32_t> GetSize();

		void Bind();
		void Unbind();

		Matrix4x4<float> GetProjectionMatrix();
		Matrix4x4<float> GetViewMatrix();

		std::shared_ptr<RenderTarget> GetRenderTarget();
		std::shared_ptr<RenderTexture> GetRenderTexture();
	private:
		std::shared_ptr<RenderTarget> m_RenderTarget{}; //viewport
		Vector2<uint32_t> m_Size{}; //Width, height
		float m_Near{};
		float m_Far{};
		float m_FOV{};

	public:
		Vector3<float> position{};
		Quaternion<float> rotation{};
		Vector4<float> clearColor{ 0.85f, 0.85f, 1.0f, 1.0f };
	};

}
