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
			ComputeDimensions();
		}

		Sprite(sptr<Texture> Texture, Rect TextureRect, sptr<Shader> Shader = nullptr)
			: texture(Texture), shader(Shader), layer(0), color(Vec4f(1, 1, 1, 1)), needUpdateRenderPipeline(true)
		{
			TextureCoordsRelative(textureCoords, TextureRect, 1.0f);
			ComputeDimensions();
		}

		/// @brief Set the texture rect to crop the texture
		/// It will automatically calculate the textures coordinates relative to the texture size
		/// @param textureRect The rectangle to crop the texture
		/// @param resolutionFactor If you have multiple texture resolutions, you may enter a multiplication factor to the rectangle
		inline void SetTextureRect(Rect textureRect, float resolutionFactor = 1.f)
		{
			TextureCoordsRelative(textureCoords, textureRect, resolutionFactor);
			ComputeDimensions();
		}

		inline void SetTexture(sptr<Texture> Texture)
		{
			texture = Texture;
			needUpdateRenderPipeline = true;
			ComputeDimensions();
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
			texSize.x *= resFactor;
			texSize.y *= resFactor;
			
			coords[0].x = rect.left / texSize.x + rect.width / texSize.x;
			coords[0].y = rect.top / texSize.y + rect.height / texSize.y;

			coords[1].x = coords[0].x;
			coords[1].y = rect.top / texSize.y;

			
			coords[2].x = rect.left / texSize.x;
			coords[2].y = coords[1].y;

			coords[3].x = coords[2].x;
			coords[3].y = coords[0].y;
		}

		inline void ComputeDimensions()
		{
			float texWidth = std::fabs(textureCoords[1].x - textureCoords[2].x);
			float texHeight = std::fabs(textureCoords[0].y - textureCoords[1].y);

			Vec2f texSize = texture->GetSize();
			float ppu = texture->GetPixelPerUnit();
			dimensions.x = texWidth * texSize.x * ppu;
			dimensions.y = texHeight * texSize.y * ppu;

		}

	public:
		Vec4f color;
		Vec2f textureCoords[4];
		bool needUpdateRenderPipeline;
		uint32_t renderPipeline;
		Vec2f dimensions;

	private:
		sptr<Texture> texture;
		sptr<Shader> shader;
		uint32_t layer;




	};
}
