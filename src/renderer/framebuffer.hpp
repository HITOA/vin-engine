#pragma once

namespace Vin {
	class FrameBuffer {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(int width, int height) = 0;


	};
}
