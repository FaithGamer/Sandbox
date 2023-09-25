#pragma once
#include <glm/glm.hpp>

namespace Sandbox
{
	typedef glm::vec2 Vec2f;
	typedef glm::i32vec2 Vec2i;
	typedef glm::u32vec2 Vec2u;

	typedef glm::vec3 Vec3f;
	typedef glm::i32vec3 Vec3i;
	typedef glm::u32vec3 Vec3u;

	typedef glm::vec4 Vec4f;

	typedef glm::mat4 Mat4;
	typedef glm::mat3 Mat3;

	class Vec2
	{
	public:
		static std::string String(Vec2i vec)
		{
			std::string x = std::to_string(vec.x);
			std::string y = std::to_string(vec.y);
			return "[" + x + "], [" + y + "]";
		}
	};
}