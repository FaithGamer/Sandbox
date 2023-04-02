#include "pch.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "Sandbox/Files.h"
#include "Sandbox/Log.h"
#include "Sandbox/Render/ShaderProgram.h"


#define SET_UNIFORM(c) GLint location = glGetUniformLocation(m_id, (const GLchar*)name.c_str());\
if (location == -1)\
{\
	LOG_ERROR("The following uniform cannot be found: " + name);\
}\
else\
{\
	glUseProgram(m_id);\
	c;\
}\

namespace Sandbox
{
	std::string loadShaderSourceFromFile(std::string path)
	{
		std::ifstream shaderFile;
		shaderFile.open(path);
		if (!shaderFile.is_open())
		{
			LOG_ERROR("Unable to open the shader: " + path);
		}
		return IO::ifstreamToString(shaderFile);
	}

	void shaderCompilationError(uint32_t shader)
	{
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compilation failed " + std::string(&infoLog[0]));
		}
	}

	void programLinkageError(uint32_t program)
	{
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader program linkage failed " + std::string(&infoLog[0]));

		}
	}

	ShaderProgram::ShaderProgram(std::string vertexSourcePath, std::string fragmentSourcePath)
	{
		//Load shader source files
		std::string vertStr = loadShaderSourceFromFile(vertexSourcePath);
		const GLchar* vertexSource = (const GLchar*)vertStr.c_str();
		std::string  fragStr = loadShaderSourceFromFile(fragmentSourcePath).c_str();
		const GLchar* fragmentSource = (const GLchar*)fragStr.c_str();

		//Creates shaders and compile
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		shaderCompilationError(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		shaderCompilationError(fragmentShader);

		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);
		glLinkProgram(m_id);
		programLinkageError(m_id);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::SetUniform(std::string name, const GLfloat& uniform)
	{
		SET_UNIFORM(glUniform1f(location, uniform));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::vec2& uniform)
	{
		SET_UNIFORM(glUniform2f(location, (GLfloat)uniform.x, (GLfloat)uniform.y));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::vec3& uniform)
	{
		SET_UNIFORM(glUniform3f(location, (GLfloat)uniform.x, (GLfloat)uniform.y, (GLfloat)uniform.z));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::vec4& uniform)
	{
		SET_UNIFORM(glUniform4f(location, (GLfloat)uniform.x, (GLfloat)uniform.y, (GLfloat)uniform.z, (GLfloat)uniform.w));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::mat3& uniform)
	{
		SET_UNIFORM(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(uniform)));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::mat4& uniform)
	{
		SET_UNIFORM(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform)));
	}

	void ShaderProgram::SetUniform(std::string name, const GLint& uniform)
	{
		SET_UNIFORM(glUniform1i(location, uniform));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::i32vec2& uniform)
	{
		SET_UNIFORM(glUniform2i(location, (GLint)uniform.x, (GLint)uniform.y));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::i32vec3& uniform)
	{
		SET_UNIFORM(glUniform3i(location, (GLint)uniform.x, (GLint)uniform.y, (GLint)uniform.z));
	}

	void ShaderProgram::SetUniform(std::string name, const glm::i32vec4& uniform)
	{
		SET_UNIFORM(glUniform4i(location, (GLint)uniform.x, (GLint)uniform.y, (GLint)uniform.z, (GLint)uniform.w));
	}

	void ShaderProgram::SetUniformArray(std::string name, const int* uniform, GLsizei count)
	{
		SET_UNIFORM(glUniform1iv(location, count, uniform));
	}

	void ShaderProgram::BindUniformBlock(std::string uniformName, GLint bindingPoint)
	{
		GLuint location = glGetUniformBlockIndex(m_id, (const GLchar*)uniformName.c_str());
		if (location == -1)
		{
			LOG_ERROR("The following uniform block cannot be found: " + uniformName);
		}
		else
		{
			glUniformBlockBinding(m_id, location, bindingPoint);
		}
	}

	GLint ShaderProgram::GetUniformLocation(std::string name)
	{
		return glGetUniformLocation(m_id, (const GLchar*)name.c_str());
	}

	GLuint ShaderProgram::GetID()
	{
		return m_id;
	}

}
