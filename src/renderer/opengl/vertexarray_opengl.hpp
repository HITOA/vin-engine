#include "renderer/vertexarray.hpp"

namespace Vin {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const eastl::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const eastl::shared_ptr<IndexBuffer>& indexBuffer);
		void IgnoreAttribute(usize n);

		const eastl::shared_ptr<IndexBuffer>& GetIndexBuffer() const;
	private:
		unsigned int m_VertexArrayId;
		usize m_AttribIndex{ 0 };
		eastl::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
