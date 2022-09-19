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
	private:
		unsigned int m_id;
	};
}