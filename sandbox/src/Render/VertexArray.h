#pragma once

#include "Buffer.h"

namespace sandbox
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const sptr<VertexBuffer>& vertexVuffer, const sptr<IndexBuffer>& indexBuffer);
		~VertexArray();
		void AddVertexBuffer(const sptr<VertexBuffer>& buffer);
		void SetIndexBuffer(const sptr<IndexBuffer>& buffer);
		void Bind();

		const sptr<IndexBuffer>& GetIndexBuffer() const;

	private:
		std::vector<sptr<VertexBuffer>> m_vertexBuffer;
		sptr<IndexBuffer> m_indexBuffer;
		GLuint m_id;
	};

}