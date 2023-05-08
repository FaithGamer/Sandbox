#pragma once

#include "Sandbox/Vec.h"
#include <glad/glad.h>

namespace Sandbox
{
	struct TextureImportSettings
	{
		TextureImportSettings(GLint Filtering = GL_LINEAR,
			GLint Wrapping = GL_CLAMP_TO_BORDER,
			bool UseMipmaps = true,
			bool KeepData = false);
		GLint filtering;
		GLint wrapping;
		bool useMipmaps;
		bool keepData;
	};

	class Texture
	{
	public:
		Texture(TextureImportSettings importSettings = TextureImportSettings());
		Texture(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		Texture(std::string path, uint32_t pixelPerUnit, TextureImportSettings importSettings = TextureImportSettings());
		~Texture();

		void SetPixelPerUnit(uint32_t pixelPerUnit);
		void Bind(uint32_t textureUnit = 0);

		Vec2i GetSize();
		uint32_t GetPixelPerUnit() const;

	private:
		uint32_t m_pixelPerUnit;
		TextureImportSettings m_importSettings;
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
		GLuint m_id;
	};
}