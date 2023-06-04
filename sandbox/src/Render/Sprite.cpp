#include "pch.h"
#include "Sandbox/Render/Sprite.h"

namespace Sandbox
{
	Sprite::Sprite()
	{
	
	}

	Sprite::Sprite(sptr<Texture> texture) : m_texture(texture)
	{
		m_textureCoords[0] = Vec2f(0, 1);
		m_textureCoords[1] = Vec2f(0, 0);
		m_textureCoords[2] = Vec2f(1, 0);
		m_textureCoords[3] = Vec2f(1, 1);
		ComputeDimensions();
	}

	Sprite::Sprite(sptr<Texture> texture, Rect textureRect) : m_texture(texture)
	{
		SetTextureRect(textureRect);
	}

	Sprite::Sprite(sptr<Texture> texture, Vec2f* textureCoords)
	{
		m_textureCoords[0] = textureCoords[0];
		m_textureCoords[1] = textureCoords[1];
		m_textureCoords[2] = textureCoords[2];
		m_textureCoords[3] = textureCoords[3];
		ComputeDimensions();
	}

	void Sprite::SetTextureRect(Rect textureRect, float resolutionFactor)
	{
		TextureCoordsRelative(m_textureCoords, textureRect, resolutionFactor);
		ComputeDimensions();
	}

	void Sprite::SetTexture(sptr<Texture> Texture)
	{
		m_texture = Texture;
		ComputeDimensions();
	}

	void Sprite::TextureCoordsRelative(Vec2f* coords, Rect rect, float resFactor)
	{
		Vec2i texSize = m_texture->GetSize();
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

	void Sprite::ComputeDimensions()
	{
		float texWidth = std::fabs(m_textureCoords[1].x - m_textureCoords[2].x);
		float texHeight = std::fabs(m_textureCoords[0].y - m_textureCoords[1].y);

		Vec2f texSize = m_texture->GetSize();
		float ppu = m_texture->GetPixelPerUnit();
		m_dimensions.x = texWidth * texSize.x * ppu;
		m_dimensions.y = texHeight * texSize.y * ppu;

	}
}