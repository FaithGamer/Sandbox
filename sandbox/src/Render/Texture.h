#pragma once

#include <string>
#include "Vec2.h"

namespace sandbox
{
	class Texture
	{
	public:
		Texture(std::string path);
		~Texture();
		Vec2i GetSize();
	private:
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
	};
}