#pragma once

#include <glad/glad.h>

namespace Sandbox
{
	enum class ShaderDataType
	{
		None = 0, Float, Vec2f, Vec3f, Vec4f, Mat3, Mat4, Int, Vec2i, Vec3i, Vec4i, Bool
	};
	GLint ShaderDataTypeSize(ShaderDataType type);
	GLint ShaderDataTypeCount(ShaderDataType type);
	GLenum ShaderDataTypeGLType(ShaderDataType type);

	/////////////////////////
	/// Buffer Element //////
	/////////////////////////

	/// @brief Basically a uniform with it's type, size, and offset within a Vertex Array
	struct AttributeElement
	{
		std::string name;
		ShaderDataType type;
		GLsizei size;
		uint32_t offset;
		GLboolean normalized;

		AttributeElement(ShaderDataType Type, const std::string& Name, GLboolean Normalized = GL_FALSE)
			: name(Name), type(Type), size(ShaderDataTypeSize(Type)), offset(0), normalized(Normalized)
		{

		}
	};

	/////////////////////////
	/// Buffer Layout ///////
	/////////////////////////

	/// @brief Helper to call glVertexAttribPointer depending on the attributes layout
	class AttributeLayout
	{
	public:
		AttributeLayout();
		AttributeLayout(const std::initializer_list<AttributeElement>& elements);
		const std::vector<AttributeElement>& GetElements() const;
		const GLsizei GetStride() const;
		std::vector<AttributeElement>::const_iterator begin() const;
		std::vector<AttributeElement>::const_iterator end() const;
	private:
		void CalculateOffsetAndStride();
		std::vector<AttributeElement> m_elements;
		GLsizei m_stride;

	};

	/////////////////////////
	/// Vertex Buffer ///////
	/////////////////////////

	/// @brief Wrapper around OpenGL Vertex Buffer
	class VertexBuffer
	{
	public:
		//To do: add an option for making it dynamic or static
		VertexBuffer() : m_id(0), m_verticesCount(0) {}
		/// @brief Manually set vertices and the according Attribute Layout
		/// This will make the buffer GL_STATIC_DRAW
		VertexBuffer(float* vertices, GLsizeiptr size, const AttributeLayout& layout = AttributeLayout());
		/// @brief This will make the buffer GL_DYNAMIC_DRAW
		VertexBuffer(GLsizeiptr size, const AttributeLayout& layout);
		/// @brief This will make the buffer GL_DYNAMIC_DRAW
		VertexBuffer(GLsizeiptr size);

		void SetData(const void* data, GLuint size);
		float* BeginAccessBuffer();
		void EndAccessBuffer();
		uint32_t GetVerticesCount();
		~VertexBuffer();

		void SetLayout(const AttributeLayout& layout);
		const AttributeLayout& GetLayout() const;

		void Bind() const;

	private:
		uint32_t m_verticesCount;
		AttributeLayout m_layout;
		GLuint m_id;
	};

	/////////////////////////
	/// Element Buffer //////
	/////////////////////////

	/// @brief Wrapper around OpenGL Element Array Buffer
	class IndexBuffer
	{
	public:
		/// @brief Make an index buffer with dynamic_draw
		/// @param indices
		/// @param count indices count
		IndexBuffer(uint32_t* indices, uint32_t count);
		IndexBuffer(uint32_t* indices, uint32_t count, GLenum type);
		~IndexBuffer();

		void Bind();
		GLsizei GetCount() const;
	private:
		GLuint m_id;
		GLsizei m_count;
	};


	/////////////////////////
	/// Uniform	Buffer //////
	/////////////////////////

	class UniformBuffer
	{
	public:
		//To do: something to calculate size based on the uniform buffer content
		UniformBuffer(GLsizeiptr size, GLuint binding);
		~UniformBuffer();
		void SetData(const void* data, GLsizeiptr size, GLuint offset);
	private:
		GLuint m_id;
	};

}