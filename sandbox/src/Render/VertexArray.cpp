#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace sandbox
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::AddVertexBuffer(const sptr<VertexBuffer>& buffer)
	{
		glBindVertexArray(m_id);
		buffer->Bind();

		ASSERT_LOG_ERROR(buffer->GetLayout().GetElements().size(), "VertexBuffer layout is empty");
		//We need to tell OpenGL how to interpret the data in the Vertex Array Buffer
		GLuint layoutIndex = 0;
		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(layoutIndex);
			glVertexAttribPointer(layoutIndex,
				ShaderDataTypeCount(element.type),
				ShaderDataTypeGLType(element.type),
				element.normalized,
				layout.GetStride(),
				(const void*)element.offset);

			layoutIndex++;
		}
		m_vertexBuffers.push_back(buffer);
	}

	void VertexArray::SetIndexBuffer(const sptr<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_id);
		buffer->Bind();
		m_indexBuffer = buffer;
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_id);
	}
}