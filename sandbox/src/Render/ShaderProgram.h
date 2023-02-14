#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sandbox
{
	class ShaderProgram
	{
	public:
		ShaderProgram(std::string vertexSourcePath, std::string fragmentSourcePath);
		~ShaderProgram();

		void Bind() const;
		
		//TO DO: add uniform arrays
		void SetUniform(std::string name, const GLfloat& uniform);
		void SetUniform(std::string name, const glm::vec2& uniform);
		void SetUniform(std::string name, const glm::vec3& uniform);
		void SetUniform(std::string name, const glm::vec4& uniform);
		void SetUniform(std::string name, const glm::mat3& uniform);
		void SetUniform(std::string name, const glm::mat4& uniform);
		void SetUniform(std::string name, const GLint& uniform);
		void SetUniform(std::string name, const glm::i32vec2& uniform);
		void SetUniform(std::string name, const glm::i32vec3& uniform);
		void SetUniform(std::string name, const glm::i32vec4& uniform);

		void SetUniform(GLint location, const GLfloat& uniform);
		void SetUniform(GLint location, const glm::vec2& uniform);
		void SetUniform(GLint location, const glm::vec3& uniform);
		void SetUniform(GLint location, const glm::vec4& uniform);
		void SetUniform(GLint location, const glm::mat3& uniform);
		void SetUniform(GLint location, const glm::mat4& uniform);
		void SetUniform(GLint location, const GLint& uniform);
		void SetUniform(GLint location, const glm::i32vec2& uniform);
		void SetUniform(GLint location, const glm::i32vec3& uniform);
		void SetUniform(GLint location, const glm::i32vec4& uniform);

		void SetUniformArray(std::string name, const int* uniform, GLsizei count);

		GLint GetUniformLocation(std::string name);
		void BindUniformBlock(std::string uniformName, GLint bindingPoint);

		GLuint GetID();

	private:
		GLuint m_id;
	};
}