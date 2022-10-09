#include "renderer/vertexarray.hpp"

namespace Vin {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;
	private:
		unsigned int m_VertexArrayId;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
