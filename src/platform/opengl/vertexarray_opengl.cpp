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

void Vin::OpenGLVertexArray::Bind()
{
	glBindVertexArray(m_VertexArrayId);
}

void Vin::OpenGLVertexArray::Unbind()
{
	glBindVertexArray(0);
}

void Vin::OpenGLVertexArray::AddVertexBuffer(const std::unique_ptr<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_VertexArrayId);
	vertexBuffer->Bind();

	const BufferLayout& layout = vertexBuffer->GetBufferLayout();

	for (const auto& element : layout) {
		switch (element.type) {
		case BufferElementType::Float: {
			glVertexAttribPointer(m_AttribIndex, 1, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Float2: {
			glVertexAttribPointer(m_AttribIndex, 2, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Float3: {
			glVertexAttribPointer(m_AttribIndex, 3, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Float4: {
			glVertexAttribPointer(m_AttribIndex, 4, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}

		case BufferElementType::Int: {
			glVertexAttribIPointer(m_AttribIndex, 1, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Int2: {
			glVertexAttribIPointer(m_AttribIndex, 2, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Int3: {
			glVertexAttribIPointer(m_AttribIndex, 3, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		case BufferElementType::Int4: {
			glVertexAttribIPointer(m_AttribIndex, 4, GL_INT, layout.GetStride(), (void*)element.offset);
			glEnableVertexAttribArray(m_AttribIndex);
			m_AttribIndex++;
			break;
		}
		}
	}
}
