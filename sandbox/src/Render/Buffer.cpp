#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>
namespace sandbox
{
	/// Vertex Buffer ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, unsigned int size)
	{
		m_count = size / sizeof(float);
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	unsigned int VertexBuffer::GetCount() const
	{
		return m_count;
	}

	/// Element Buffer ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	ElementBuffer::ElementBuffer(unsigned int* indices, unsigned int size)
	{
		m_count = size / sizeof(unsigned int);
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void ElementBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	unsigned int ElementBuffer::GetCount() const
	{
		return m_count;
	}
}