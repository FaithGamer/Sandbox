#pragma once
#include "Sandbox/std_macros.h"

namespace Sandbox
{
	class Shader;

	class Material
	{
	public:
		Material();
		void SetShader(sptr<Shader> shader);
		void Bind();

	private:

		//To do uniforms
	};
}