#include "renderer/vertexarray.hpp"

namespace Vin {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind();
		void Unbind();

		void AddVertexBuffer(const std::unique_ptr<VertexBuffer>& vertexBuffer);

	private:
		unsigned int m_VertexArrayId;
		size_t m_AttribIndex{ 0 };
	};

}
