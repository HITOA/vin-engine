#include "vertexarray_opengl.hpp"

#include "glad/gl.h"

Vin::OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_VertexArrayId);
}

Vin::OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_VertexArrayId);
}

void Vin::OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_VertexArrayId);
}

void Vin::OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Vin::OpenGLVertexArray::AddVertexBuffer(const eastl::shared_ptr<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_VertexArrayId);
	vertexBuffer->Bind();

	const VertexBufferLayout& layout = vertexBuffer->GetBufferLayout();

	for (const auto& element : layout) {
		switch (element.type) {
		case VertexAttributeType::Float: {
			glVertexAttribPointer((GLuint)element.attribute, 1, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Float2: {
			glVertexAttribPointer((GLuint)element.attribute, 2, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Float3: {
			glVertexAttribPointer((GLuint)element.attribute, 3, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Float4: {
			glVertexAttribPointer((GLuint)element.attribute, 4, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}

		case VertexAttributeType::Int: {
			glVertexAttribIPointer((GLuint)element.attribute, 1, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Int2: {
			glVertexAttribIPointer((GLuint)element.attribute, 2, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Int3: {
			glVertexAttribIPointer((GLuint)element.attribute, 3, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		case VertexAttributeType::Int4: {
			glVertexAttribIPointer((GLuint)element.attribute, 4, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray((GLuint)element.attribute);
			break;
		}
		}
	}
}

void Vin::OpenGLVertexArray::SetIndexBuffer(const eastl::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_VertexArrayId);
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}

const eastl::shared_ptr<Vin::IndexBuffer>& Vin::OpenGLVertexArray::GetIndexBuffer() const
{
	return m_IndexBuffer;
}
