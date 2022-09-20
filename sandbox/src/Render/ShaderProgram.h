#pragma once

#include <string>

namespace sandbox
{
	class ShaderProgram
	{
	public:
		ShaderProgram(std::string vertexSourcePath, std::string fragmentSourcePath);
		~ShaderProgram();

		void Bind() const;
		
		void SetUniform(std::string name, float uniform);
		void SetUniform(std::string name, glm::vec2 uniform);
		void SetUniform(std::string name, glm::vec3 uniform);
		void SetUniform(std::string name, glm::vec4 uniform);
		void SetUniform(std::string name, glm::mat3 uniform);
		void SetUniform(std::string name, glm::mat4 uniform);
		void SetUniform(std::string name, int uniform);
		void SetUniform(std::string name, glm::i32vec2 uniform);
		void SetUniform(std::string name, glm::i32vec3 uniform);
		void SetUniform(std::string name, glm::i32vec4 uniform);
		void SetUniform(std::string name, bool uniform);
		

	private:
		unsigned int m_id;
	};
}