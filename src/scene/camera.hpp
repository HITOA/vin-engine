#pragma once

#include "core/identifiable.hpp"
#include "renderer/framebuffer.hpp"

namespace Vin {

	class Camera : public Identifiable<Camera, unsigned char> {
		//FrameBuffer
		//Camera data
	private:
		std::shared_ptr<RenderTarget> m_RenderTarget{}; //viewport
	};

}
