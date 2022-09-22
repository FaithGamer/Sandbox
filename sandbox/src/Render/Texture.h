#pragma once

namespace sandbox
{
	struct TextureImportSettings
	{
		TextureImportSettings(GLint Filtering = GL_NEAREST,
			GLint Wrapping = GL_CLAMP_TO_BORDER,
			bool UseMipmaps = false,
			bool KeepData = false);
		GLint filtering;
		GLint wrapping;
		bool useMipmaps;
		bool keepData;
	};

	class Texture
	{
	public:
		Texture(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		~Texture();
		Vec2i GetSize();
		void Bind();
	private:
		TextureImportSettings m_importSettings;
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
		GLuint m_id;
	};
}