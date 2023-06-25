#include "buffer_opengl.hpp"

#include "logger/logger.hpp"

#include "glad/gl.h"

unsigned int GetGLUsageFromBufferUsage(Vin::BufferUsageType usage) {
	switch (usage)
	{
	case Vin::BufferUsageType::Stream:
		return GL_STREAM_READ;
	case Vin::BufferUsageType::Static:
		return GL_STATIC_DRAW;
	case Vin::BufferUsageType::Dynamic:
		return GL_DYNAMIC_DRAW;
	default:
		return GL_STATIC_DRAW;
	}
}

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

void Vin::OpenGLVertexBuffer::SetData(void* data, size_t size, size_t offset, bool resize, BufferUsageType usage)
{

	if (resize)
		glNamedBufferData(m_BufferId, size, data, GetGLUsageFromBufferUsage(usage));
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

void Vin::OpenGLIndexBuffer::SetData(void* data, uint32_t count, BufferUsageType usage)
{
	glNamedBufferData(m_BufferId, count * GetBufferIndexTypeSize(m_Type), data, GetGLUsageFromBufferUsage(usage));
	m_Count = count;
}
