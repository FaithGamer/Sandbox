#include "pch.h"

#include "Sandbox/Log.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Render/Texture.h"

#include <stb/stb_image.h>

namespace Sandbox
{
	TextureImportSettings::TextureImportSettings(TextureFiltering Filtering, TextureWrapping Wrapping, bool UseMipmaps, bool KeepData)
		: filtering(Filtering), wrapping(Wrapping), useMipmaps(UseMipmaps), keepData(KeepData)
	{

	}
	TextureImportSettings::TextureImportSettings(Config parameters) : TextureImportSettings()
	{
		LoadParameters(parameters);
	}

	void TextureImportSettings::LoadParameters(Config parameters)
	{
		String Filtering = parameters.GetString("Filtering");
		if (Filtering == "Linear")
			filtering = TextureFiltering::Linear;
		else if (Filtering == "Nearest")
			filtering = TextureFiltering::Nearest;
		else
		{
			LOG_WARN("Unknown texture filtering parameters: " + Filtering + ", in file: " + parameters.GetPath());
		}

		String Wrapping = parameters.GetString("Wrapping");
		if (Wrapping == "Clamp")
			wrapping = TextureWrapping::Clamp;
		else if (Wrapping == "Repeat")
			wrapping = TextureWrapping::Repeat;
		else
		{
			LOG_WARN("Unknown texture wrapping parameters: " + Wrapping + ", in file: " + parameters.GetPath());
		}

		useMipmaps = parameters.GetBool("Mipmaps");
		keepData = parameters.GetBool("KeepData");

	}
	Json TextureImportSettings::ToJson()
	{
		Json json;
		if (filtering == TextureFiltering::Linear)
			json["Filtering"] = "Linear";
		else
			json["Filtering"] = "Nearest";

		if (wrapping == TextureWrapping::Clamp)
			json["Wrapping"] = "Clamp";
		else
			json["Wrapping"] = "Repeat";

		json["Mipmaps"] = useMipmaps;
		json["KeepData"] = keepData;

		return json;
	}
	Texture::Texture() : m_id(0), m_pixelPerUnit(1.f), m_pixels(nullptr), m_nbChannels(0), m_importSettings(TextureImportSettings()), m_size(1, 1)
	{

	}
	void Texture::Create1x1White()
	{
		//generate 1x1 white texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		m_pixels = new unsigned char(0xd);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)1, (GLsizei)1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);

		//Texture Wrapping
		GLfloat borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_importSettings.wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_importSettings.wrapping);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//Texture filtering
		GLint minFilter = m_importSettings.useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : m_importSettings.filtering;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_importSettings.filtering);

		if (m_importSettings.useMipmaps)
		{
			//glGenerateMipmap(GL_TEXTURE_2D);
		}

		delete m_pixels;

		//Unbind since we are done configuring this texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(std::string path, TextureImportSettings importSettings)
		: m_size(0, 0), m_nbChannels(0), m_pixels(nullptr), m_id(0), m_importSettings(importSettings), m_pixelPerUnit(1.f)
	{
		if (path == "white")
		{
			Create1x1White();
			return;
		}
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
		m_pixelPerUnit = 1.f / pixelPerUnit;
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
		m_pixelPerUnit = 1.f / pixelPerUnit;
	}

	void Texture::Bind(uint32_t textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	GLuint Texture::GetId()
	{
		return m_id;
	}

	Vec2i Texture::GetSize()
	{
		return m_size;
	}


	float Texture::GetPixelPerUnit() const
	{
		return m_pixelPerUnit;
	}

}