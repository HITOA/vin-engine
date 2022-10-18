#pragma once

#include "core/identifiable.hpp"
#include "renderer/framebuffer.hpp"
#include "math/math.hpp"

namespace Vin {

	class Camera : public Identifiable<Camera, unsigned char> {
	public:
		Camera() = delete;
		Camera(Vector2<int> size, int samples = 8, bool HDR = true);

		void SetNearPlane(float near);
		float GetNearPlane();

		void SetFarPlane(float far);
		float GetFarPlane();

		void SetFOV(float fov);
		float GetFOV();

		void Resize(Vector2<int> size);
		Vector2<int> GetSize();

		void Bind();
		void Unbind();

		Matrix4x4<float> GetProjectionMatrix();
		Matrix4x4<float> GetViewMatrix();

		std::shared_ptr<RenderTarget> GetRenderTarget();
		std::shared_ptr<RenderTexture> GetRenderTexture();
	private:
		std::shared_ptr<RenderTarget> m_RenderTarget{}; //viewport
		Vector2<int> m_Size{}; //Width, height
		float m_Near{};
		float m_Far{};
		float m_FOV{};
	};

}
