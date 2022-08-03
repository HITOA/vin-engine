#include "buffer_opengl.hpp"

#include "core/logger.hpp"

#include "glad/gl.h"

Vin::OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
{
	glCreateBuffers(1, &m_BufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

Vin::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void Vin::OpenGLVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

void Vin::OpenGLVertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vin::OpenGLVertexBuffer::SetData(void* data, size_t size, size_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
