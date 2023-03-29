#pragma once

#include "Vec.h"
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
		Texture(Vec2i size);
		Texture(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		~Texture();

		void Bind(uint32_t textureUnit = 0);
		void SetData(void* data, uint32_t size);
		Vec2i GetSize();

	private:
		TextureImportSettings m_importSettings;
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
		GLuint m_id;
	};
}