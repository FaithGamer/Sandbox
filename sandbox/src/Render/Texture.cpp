#include "pch.h"
#include "Texture.h"

#include <stb_image.h>

namespace sandbox
{
	Texture::Texture(std::string path) : m_size(0, 0), m_nbChannels(0), m_pixels(nullptr)
	{
		m_pixels = stbi_load(path.c_str(), &m_size.x, &m_size.y, &m_nbChannels, 0);
	}
	Texture::~Texture()
	{
		delete m_pixels;
	}
	Vec2i Texture::GetSize()
	{
		return m_size;
	}
}