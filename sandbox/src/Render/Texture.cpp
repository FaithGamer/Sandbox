#include "pch.h"

#include "Sandbox/Log.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Render/Texture.h"

#include <stb/stb_image.h>

namespace Sandbox
{
	TextureImportSettings::TextureImportSettings(GLint Filtering, GLint Wrapping, bool UseMipmaps, bool KeepData) 
		: filtering(Filtering), wrapping(Wrapping), useMipmaps(UseMipmaps), keepData(KeepData)
	{

	}

	Texture::Texture(TextureImportSettings importSettings) 
		: m_size(1, 1), m_nbChannels(0), m_pixels(nullptr), m_id(0), m_importSettings(importSettings), m_pixelPerUnit(100)
	{
		//generate 1x1 white texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		m_pixels = new unsigned char(0xd);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)1, (GLsizei)1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

		//Texture Wrapping
		GLfloat borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, importSettings.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, importSettings.wrapping);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//Texture filtering
		GLint minFilter = importSettings.useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : importSettings.filtering;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, importSettings.filtering);

		if (importSettings.useMipmaps)
		{
			//glGenerateMipmap(GL_TEXTURE_2D);
		}

		delete m_pixels;

		//Unbind since we are done configuring this texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(std::string path, TextureImportSettings importSettings) 
		: m_size(0, 0), m_nbChannels(0), m_pixels(nullptr), m_id(0), m_importSettings(importSettings), m_pixelPerUnit(100)
	{
		//Load image data
		m_pixels = stbi_load(path.c_str(), &m_size.x, &m_size.y, &m_nbChannels, 4);
		

		ASSERT_LOG_ERROR(m_pixels, "Failed to load texture: " + path);

		//Generate and bind an OpenGL texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		//Send the texture data to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

		//Texture Wrapping
		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // transparent
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, importSettings.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, importSettings.wrapping);
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

	Texture::Texture(std::string path, uint32_t pixelPerUnit, TextureImportSettings importSettings) : Texture(path, importSettings)
	{
		m_pixelPerUnit = pixelPerUnit;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
		if (m_importSettings.keepData)
		{
			stbi_image_free(m_pixels);
		}
	}

	void Texture::SetPixelPerUnit(uint32_t pixelPerUnit)
	{
		m_pixelPerUnit = pixelPerUnit;
	}

	void Texture::Bind(uint32_t textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	Vec2i Texture::GetSize()
	{
		return m_size;
	}

	uint32_t Texture::GetPixelPerUnit() const
	{
		return m_pixelPerUnit;
	}
	
}