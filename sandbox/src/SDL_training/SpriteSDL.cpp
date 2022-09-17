#include "pch.h"
#include "SpriteSDL.h"
#include "TextureSDL.h"
#include "RenderWindowSDL.h"

SpriteSDL::SpriteSDL() : m_position(0, 0), m_texture(nullptr), m_textureArea{0, 0, 0, 0}, m_rotation(0)
{

}

SpriteSDL::SpriteSDL(std::shared_ptr<TextureSDL>& tex) : m_texture(tex), m_textureArea{ 0, 0, tex->size.x, tex->size.y }, m_position(0, 0), m_rotation(0)
{

}

SpriteSDL::SpriteSDL(std::shared_ptr<TextureSDL>& tex, SDL_Rect textureArea) : m_texture(tex), m_textureArea(textureArea), m_position(0, 0), m_rotation(0)
{
}

void SpriteSDL::SetTextureArea(SDL_Rect textureArea)
{
	m_textureArea = textureArea;
}

void SpriteSDL::SetTexture(std::shared_ptr<TextureSDL>& tex)
{
	m_texture = tex;
}

void SpriteSDL::SetPosition(Vec2i pos)
{
	m_position = pos;
}

void SpriteSDL::Rotate(float angle)
{
	m_rotation += angle;
}

void SpriteSDL::SetRotation(float angle)
{
	m_rotation = angle;
}

void SpriteSDL::Draw(RenderWindowSDL& target) const
{
	SDL_Rect destRect(m_position.x - target.m_view.m_rect.x, m_position.y - target.m_view.m_rect.y, m_textureArea.w, m_textureArea.h);
	SDL_RenderCopy(target.renderer, m_texture->texture, &m_textureArea, &destRect);
}