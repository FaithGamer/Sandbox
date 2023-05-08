#pragma once

#include <vector>
#include "Sandbox/std_macros.h"
#include "Sandbox/Render/Rect.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Renderer2D.h"

namespace Sandbox
{
	class Sprite
	{
	public:
		Sprite(sptr<Texture> Texture, sptr<Shader> Shader = nullptr)
			: texture(Texture), shader(Shader), layer(0), color(Vec4f(1, 1, 1, 1)), needUpdateRenderPipeline(true)
		{
			textureCoords[0] = Vec2f(0, 1);
			textureCoords[1] = Vec2f(0, 0);
			textureCoords[2] = Vec2f(1, 0);
			textureCoords[3] = Vec2f(1, 1);
		}

		Sprite(sptr<Texture> Texture, Rect TextureRect, sptr<Shader> Shader = nullptr)
			: texture(Texture), shader(Shader), layer(0), color(Vec4f(1, 1, 1, 1)), needUpdateRenderPipeline(true)
		{
			TextureCoordsRelative(textureCoords, TextureRect, 1.0f);
		}

		/// @brief Set the texture rect to crop the texture
		/// It will automatically calculate the textures coordinates relative to the texture size
		/// @param textureRect The rectangle to crop the texture
		/// @param resolutionFactor If you have multiple texture resolutions, you may enter a multiplication factor to the rectangle
		inline void SetTextureRect(Rect textureRect, float resolutionFactor = 1.f)
		{
			TextureCoordsRelative(textureCoords, textureRect, resolutionFactor);
		}

		inline void SetTexture(sptr<Texture> Texture)
		{
			texture = Texture;
			needUpdateRenderPipeline = true;
		}

		inline void SetShader(sptr<Shader> Shader)
		{
			shader = Shader;
			needUpdateRenderPipeline = true;
		}

		inline void SetLayer(uint32_t Layer)
		{
			layer = Layer;
			needUpdateRenderPipeline = true;
		}

		inline sptr<Texture> GetTexture() const
		{
			return texture;
		}

		inline sptr<Shader> GetShader() const
		{
			return shader;
		}

		inline uint32_t GetLayer() const
		{
			return layer;
		}

		inline void TextureCoordsRelative(Vec2f* coords, Rect rect, float resFactor = 1.f)
		{
			Vec2i texSize = texture->GetSize();
			coords[0].x = rect.left * resFactor / texSize.x;
			coords[0].y = rect.top * resFactor / texSize.y;

			coords[1].x = coords[0].x;
			coords[1].y = rect.top * resFactor / texSize.y + rect.height * resFactor / texSize.y;

			coords[2].x = rect.left * resFactor / texSize.x + rect.width * resFactor / texSize.x;
			coords[2].y = coords[1].y;

			coords[3].x = coords[1].x;
			coords[3].y = coords[0].y;
		}

	public:
		Vec4f color;
		Vec2f textureCoords[4];
		bool needUpdateRenderPipeline;
		uint32_t renderPipeline;

	private:
		sptr<Texture> texture;
		sptr<Shader> shader;
		uint32_t layer;




	};
}
