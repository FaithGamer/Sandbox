#pragma once

#include <vector>
#include "Sandbox/std_macros.h"
#include "Sandbox/Render/Rect.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Render/Sprite.h"

namespace Sandbox
{
	/// @brief Component to render sprite
	class SpriteRender
	{
	public:
		SpriteRender();
		SpriteRender(sptr<Sprite> sprite, sptr<Shader> Shader);

		void SetSprite(sptr<Sprite> sprite);
		void SetShader(sptr<Shader> Shader);
		void SetLayer(uint32_t Layer);

		inline sptr<Sprite> GetSprite() const
		{
			return m_sprite;
		}

		inline sptr<Texture> GetTexture() const
		{
			return m_sprite->GetTexture();
		}

		inline sptr<Shader> GetShader() const
		{
			return m_shader;
		}

		inline uint32_t GetLayer() const
		{
			return m_layer;
		}

	public:
		Vec4f color;
		bool needUpdateRenderBatch;
		uint32_t renderBatch;
		Sandbox::Vec3f preComputedPosition[4];
		bool spriteDimensionsChanged;
	
	private:
		sptr<Shader> m_shader;
		sptr<Sprite> m_sprite;
		uint32_t m_layer;

	};
}
