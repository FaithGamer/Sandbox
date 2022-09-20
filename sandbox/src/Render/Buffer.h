#pragma once

#include "Log.h"
#include "Vertex.h"

namespace sandbox
{
	enum class ShaderDataType
	{
		None = 0, Float, Vec2f, Vec3f, Vec4f, Mat3, Mat4, Int, Vec2i, Vec3i, Vec4i, Bool
	};
	unsigned int ShaderDataTypeSize(ShaderDataType type);
	unsigned int ShaderDataTypeCount(ShaderDataType type);
	int ShaderDataTypeGLType(ShaderDataType type);
	/// Buffer Element ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	/// <summary>
	/// Basically a uniform with it's type, size, and offset within a Vertex Array
	/// </summary>
	struct AttributeElement
	{
		std::string name;
		ShaderDataType type;
		unsigned int size;
		unsigned int offset;
		bool normalized;

		AttributeElement(ShaderDataType Type, const std::string& Name, bool Normalized = false)
			: name(Name), type(Type), size(ShaderDataTypeSize(Type)), offset(0), normalized(Normalized)
		{

		}
	};

	/// Buffer Layout ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	/// <summary>
	/// Helper to call glVertexAttribPointer depending on the attributes layout
	/// </summary>
	class AttributeLayout
	{
	public:
		AttributeLayout() : m_stride(0) {};
		AttributeLayout(const std::initializer_list<AttributeElement>& elements);
		const std::vector<AttributeElement>& GetElements() const;
		const unsigned int GetStride() const;
		std::vector<AttributeElement>::const_iterator begin() const;
		std::vector<AttributeElement>::const_iterator end() const;
	private:
		void CalculateOffsetAndStride();
		std::vector<AttributeElement> m_elements;
		unsigned int m_stride;

	};

	/// Vertex Buffer ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Wrapper around OpenGL Vertex Buffer
	/// </summary>
	class VertexBuffer
	{
	public:
		VertexBuffer() : m_id(0) {}
		VertexBuffer(float* vertices, unsigned int size);
		VertexBuffer(const std::vector<Vertex>& vertices);
		~VertexBuffer();

		void SetLayout(const AttributeLayout& layout);
		const AttributeLayout& GetLayout() const;

		void Bind() const;

	private:
		AttributeLayout m_layout;
		unsigned int m_id;
	};

	/// Element Buffer ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	/// <summary>
	/// Wrapper around OpenGL Element Array Buffer
	/// </summary>
	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int* indices, unsigned int size);
		~IndexBuffer();

		void Bind();
		unsigned int GetCount() const;
	private:
		unsigned int m_id;
		unsigned int m_count;
	};

}