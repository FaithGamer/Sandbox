#pragma once

#include "SDL.h"
#include "Vec2.h"

class RenderWindowSDL;
class TextureSDL;

class SpriteSDL
{
public:
	SpriteSDL();
	SpriteSDL(std::shared_ptr<TextureSDL>& tex);
	SpriteSDL(std::shared_ptr<TextureSDL>& tex, SDL_Rect textureArea);
	SpriteSDL(std::shared_ptr<TextureSDL>& tex, SDL_Rect textureArea, Vec2i position);
	void SetTextureArea(SDL_Rect textureArea);
	void SetTexture(std::shared_ptr<TextureSDL>& tex);
	void SetPosition(Vec2i pos);
	void Rotate(float angle);
	void SetOrigin(Vec2i origin);
	void SetRotation(float angle);
	void Draw(RenderWindowSDL& target) const;

	Vec2i m_position;
	SDL_Point m_origin;
	float m_rotation;
	SDL_Rect m_textureArea;
	std::shared_ptr<TextureSDL> m_texture;
private:
	
	
	
	
};

