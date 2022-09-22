#include "pch.h"

#include <glad/glad.h>
#include <string>
#include "Vec.h"
#include "Texture.h"
#include "Log.h"

#include <stb/stb_image.h>

namespace sandbox
{
	TextureImportSettings::TextureImportSettings(GLint Filtering, GLint Wrapping, bool UseMipmaps, bool KeepData) 
		: filtering(Filtering), wrapping(Wrapping), useMipmaps(UseMipmaps), keepData(KeepData)
	{

	}


	Texture::Texture(std::string path, TextureImportSettings importSettings) 
		: m_size(0, 0), m_nbChannels(0), m_pixels(nullptr), m_id(0), m_importSettings(importSettings)
	{
		//Load image data
		m_pixels = stbi_load(path.c_str(), &m_size.x, &m_size.y, &m_nbChannels, 0);

		ASSERT_LOG_ERROR(m_pixels, "Failed to load texture: " + path);

		//Generate and bind an OpenGL texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		//Send the texture data to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

		//Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, importSettings.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, importSettings.wrapping);
		GLfloat borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//Texture filtering
		GLint minFilter = importSettings.useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : importSettings.filtering;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, importSettings.filtering);

		if (importSettings.useMipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		if (!importSettings.keepData)
		{
			stbi_image_free(m_pixels);
		}

		//Unbind since we are done configuring this texture
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
		if (m_importSettings.keepData)
		{
			stbi_image_free(m_pixels);
		}
	}
	Vec2i Texture::GetSize()
	{
		return m_size;
	}
}