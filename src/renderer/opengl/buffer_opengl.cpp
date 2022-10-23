#include "buffer_opengl.hpp"

#include "logger/logger.hpp"

#include "glad/gl.h"

Vin::OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
{
	glCreateBuffers(1, &m_BufferId);
	glNamedBufferData(m_BufferId, size, nullptr, GL_STATIC_DRAW);
}

Vin::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void Vin::OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

void Vin::OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vin::OpenGLVertexBuffer::SetBufferLayout(const VertexBufferLayout& layout)
{
	m_Layout = layout;
}

const Vin::VertexBufferLayout& Vin::OpenGLVertexBuffer::GetBufferLayout() const
{
	return m_Layout;
}

void Vin::OpenGLVertexBuffer::SetData(void* data, size_t size, size_t offset, bool resize)
{
	if (resize)
		glNamedBufferData(m_BufferId, size, data, GL_STATIC_DRAW);
	else
		glNamedBufferSubData(m_BufferId, offset, size, data);
}

Vin::OpenGLIndexBuffer::OpenGLIndexBuffer(BufferIndexType type) : m_Type{ type }
{
	glCreateBuffers(1, &m_BufferId);
}

Vin::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void Vin::OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
}

void Vin::OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t Vin::OpenGLIndexBuffer::GetCount() const
{
	return m_Count;
}

Vin::BufferIndexType Vin::OpenGLIndexBuffer::GetIndexType() const
{
	return m_Type;
}

void Vin::OpenGLIndexBuffer::SetData(void* data, uint32_t count)
{
	glNamedBufferData(m_BufferId, count * GetBufferIndexTypeSize(m_Type), data, GL_STATIC_DRAW);
	m_Count = count;
}
