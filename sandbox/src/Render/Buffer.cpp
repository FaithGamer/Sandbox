#include "pch.h"

#include <glad/glad.h>

#include "Buffer.h"
#include "Log.h"

namespace sandbox
{

	GLint ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return sizeof(float);			break;
		case ShaderDataType::Vec2f:	return sizeof(float) * 2;		break;
		case ShaderDataType::Vec3f:	return sizeof(float) * 3;		break;
		case ShaderDataType::Vec4f:	return sizeof(float) * 4;		break;
		case ShaderDataType::Mat3:	return sizeof(float) * 3 * 3;	break;
		case ShaderDataType::Mat4:	return sizeof(float) * 4 * 4;	break;
		case ShaderDataType::Int:	return sizeof(int);				break;
		case ShaderDataType::Vec2i:	return sizeof(int) * 2;			break;
		case ShaderDataType::Vec3i:	return sizeof(int) * 3;			break;
		case ShaderDataType::Vec4i:	return sizeof(int) * 4;			break;
		case ShaderDataType::Bool:	return sizeof(bool);			break;
		default:break;
		}
		ASSERT_LOG_ERROR(false, "Unknow ShaderDataType !");
		return 0;
	}

	GLint ShaderDataTypeCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return 1;		break;
		case ShaderDataType::Vec2f:	return 2;		break;
		case ShaderDataType::Vec3f:	return 3;		break;
		case ShaderDataType::Vec4f:	return 4;		break;
		case ShaderDataType::Mat3:	return 3 * 3;	break;
		case ShaderDataType::Mat4:	return 4 * 4;	break;
		case ShaderDataType::Int:	return 1;		break;
		case ShaderDataType::Vec2i:	return 2;		break;
		case ShaderDataType::Vec3i:	return 3;		break;
		case ShaderDataType::Vec4i:	return 4;		break;
		case ShaderDataType::Bool:	return 1;		break;
		default:break;
		}
		ASSERT_LOG_ERROR(false, "Unknow ShaderDataType !");
		return 0;
	}

	GLenum ShaderDataTypeGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return GL_FLOAT;	break;
		case ShaderDataType::Vec2f:	return GL_FLOAT;	break;
		case ShaderDataType::Vec3f:	return GL_FLOAT;	break;
		case ShaderDataType::Vec4f:	return GL_FLOAT;	break;
		case ShaderDataType::Mat3:	return GL_FLOAT;	break;
		case ShaderDataType::Mat4:	return GL_FLOAT;	break;
		case ShaderDataType::Int:	return GL_INT;		break;
		case ShaderDataType::Vec2i:	return GL_INT;		break;
		case ShaderDataType::Vec3i:	return GL_INT;		break;
		case ShaderDataType::Vec4i:	return GL_INT;		break;
		case ShaderDataType::Bool:	return GL_BOOL;		break;
		default:break;
		}
		ASSERT_LOG_ERROR(false, "Unknow ShaderDataType !");
		return 0;
	}

	/// AttributeLayout /////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	
	AttributeLayout::AttributeLayout(const std::initializer_list<AttributeElement>& elements)
		: m_elements(elements)
	{
		CalculateOffsetAndStride();
	}

	const std::vector<AttributeElement>& AttributeLayout::GetElements() const
	{
		return m_elements;
	}

	const GLsizei AttributeLayout::GetStride() const
	{
		return m_stride;
	}

	std::vector<AttributeElement>::const_iterator AttributeLayout::begin() const
	{
		return m_elements.begin();
	}

	std::vector<AttributeElement>::const_iterator AttributeLayout::end() const
	{
		return m_elements.end();
	}

	void AttributeLayout::CalculateOffsetAndStride()
	{
		GLsizei offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	}

	/// Vertex Buffer ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, GLsizeiptr size)
	{
		//Create one buffer
		glGenBuffers(1, &m_id);
		//Bind an array buffer to operate on in
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//Send the data in the buffer
		glBufferData(GL_ARRAY_BUFFER, size, (const void*)vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
	{
		GLsizeiptr size = vertices.size() * sizeof(Vertex);
		//Create one buffer
		glGenBuffers(1, &m_id);
		//Bind an array buffer to operate on in
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		//Send the data in the buffer
		glBufferData(GL_ARRAY_BUFFER, size, (const void*)&vertices[0], GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::SetLayout(const AttributeLayout& layout)
	{
		m_layout = layout;
	}

	const AttributeLayout& VertexBuffer::GetLayout() const
	{
		return m_layout;
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}


	/// Element Buffer ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, GLsizeiptr size)
	{
		m_count = (GLsizei)(size / sizeof(uint32_t));
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (const void*)indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	GLsizei IndexBuffer::GetCount() const
	{
		return m_count;
	}

}