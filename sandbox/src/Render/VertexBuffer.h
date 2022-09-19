#pragma once


namespace sandbox
{
	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, unsigned int size);
		~VertexBuffer();
		void Bind() const;
		unsigned int GetCount() const;

	private:
		unsigned int m_id;
		unsigned int m_count;
	};

}