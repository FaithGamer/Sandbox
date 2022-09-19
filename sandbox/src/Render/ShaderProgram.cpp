#include "pch.h"
#include "ShaderProgram.h"
#include "glad/glad.h"
#include "Log.h"
#include "Tools/files.h"

namespace sandbox
{
	std::string loadShaderSourceFromFile(std::string path)
	{
		std::ifstream shaderFile;
		shaderFile.open(path);
		if (!shaderFile.is_open())
		{
			LOG_ERROR("Unable to open the shader: " + path);
		}
		return files::ifstreamToString(shaderFile);
	}

	void shaderCompilationError(unsigned int shader)
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

	void programLinkageError(unsigned int program)
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

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_id);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}
}
