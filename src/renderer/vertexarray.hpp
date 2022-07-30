#pragma once

namespace Vin {
	class VertexArray {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}
