#pragma once

#include "Buffer.h"

namespace sandbox
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void AddVertexBuffer(const sptr<VertexBuffer>& buffer);
		void SetIndexBuffer(const sptr<IndexBuffer>& buffer);
		void Bind();

	private:
		std::vector<sptr<VertexBuffer>> m_vertexBuffers;
		sptr<IndexBuffer> m_indexBuffer;
		GLuint m_id;
	};

}