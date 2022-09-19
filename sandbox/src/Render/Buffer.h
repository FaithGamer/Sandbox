#pragma once

namespace sandbox
{

	/// Vertex Buffer ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

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

	/// Element Buffer ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	class ElementBuffer
	{
	public:
		ElementBuffer(unsigned int* indices, unsigned int size);
		~ElementBuffer();

		void Bind();
		unsigned int GetCount() const;
	private:
		unsigned int m_id;
		size_t m_count;
	};

}