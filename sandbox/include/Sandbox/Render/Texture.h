#pragma once

#include "Sandbox/Vec.h"
#include <glad/glad.h>

namespace Sandbox
{
	typedef enum : GLint
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	}TextureFiltering;

	typedef enum : GLint
	{
		Clamp = GL_CLAMP_TO_BORDER,
		Repeat = GL_REPEAT
	}TextureWrapping;

	struct TextureImportSettings
	{
		TextureImportSettings(
			TextureFiltering TextureFiltering = Linear,
			TextureWrapping Wrapping = Clamp,
			bool UseMipmaps = true,
			bool KeepData = false);
		TextureFiltering filtering;
		GLint wrapping;
		bool useMipmaps;
		bool keepData;
	};

	class RenderTexture;

	class Texture
	{
	public:
		Texture();
		Texture(bool whiteTexture1x1);
		Texture(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		Texture(std::string path, uint32_t pixelPerUnit, TextureImportSettings importSettings = TextureImportSettings());
		~Texture();

		void SetPixelPerUnit(uint32_t pixelPerUnit);
		void Bind(uint32_t textureUnit = 0);

		Vec2i GetSize();

		/// @brief The scale of the texture.
		/// @return The amount of world unit for one pixel of the texture ( 1.0f / pixelPerUnit ) 
		float GetPixelPerUnit() const;

	private:
		friend RenderTexture;
		float m_pixelPerUnit;
		TextureImportSettings m_importSettings;
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
		GLuint m_id;
	};
}