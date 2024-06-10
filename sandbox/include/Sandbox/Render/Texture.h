#pragma once
#include <glad/glad.h>
#include "Sandbox/Core/Vec.h"
#include "Sandbox/Core/Serialization.h"


namespace Sandbox
{

	typedef enum : GLint
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	}TextureFiltering;

	typedef enum : GLint
	{
		Clamp = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT
	}TextureWrapping;

	struct TextureImportSettings : public Serializable
	{
		TextureImportSettings() = default;
		TextureImportSettings(
			TextureFiltering TextureFiltering,
			TextureWrapping Wrapping,
			float PixelPerUnit,
			bool UseMipmaps,
			bool KeepData);

		TextureImportSettings(Serialized& config);
		void Deserialize(Serialized& config);
		Serialized Serialize() override;
		bool DeserializationError() override;

		TextureFiltering filtering = TextureFiltering::Linear;
		GLint wrapping = TextureWrapping::Clamp;
		float pixelPerUnit = 1;
		bool useMipmaps = true;
		bool keepData = false;

		bool valid = true;

	};

	class RenderTexture;

	class Texture
	{
	public:
		Texture();
		Texture(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		void Reload(std::string path, TextureImportSettings importSettings = TextureImportSettings());
		~Texture();

		void Bind(uint32_t textureUnit = 0);
		void SetPixelPerUnit(float ppu);

		GLuint GetId();
		Vec2i GetSize();

		/// @brief The scale of the texture.
		/// @return The amount of world unit for one pixel of the texture ( 1.0f / pixelPerUnit ) 
		float GetPixelPerUnit() const;

	private:
		inline void Load(std::string path, TextureImportSettings importSettings);
		void Create1x1White();
		friend RenderTexture;
		TextureImportSettings m_importSettings;
		Vec2i m_size;
		int m_nbChannels;
		unsigned char* m_pixels;
		GLuint m_id;
	};
}