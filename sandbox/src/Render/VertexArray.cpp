#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace sandbox
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	void VertexArray::AddVertexBuffer(const sptr<VertexBuffer>& buffer)
	{
	}

	void VertexArray::SetIndexBuffer(const sptr<IndexBuffer>& buffer)
	{
	}

	void VertexArray::Bind()
	{
	}
}