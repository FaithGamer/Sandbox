#pragma once

#include <glad/glad.h>
#include "RenderTarget.h"

namespace Sandbox
{
	/// @brief wrapper around an OpenGL FrameBuffer, RenderBuffer, and Texture Buffer
	///Together they make for an offscreen rendering target allowing for depth and stencil test.
	class RenderTexture : public RenderTarget
	{
	public:
		RenderTexture(Vec2u size);
		void BindTexture(uint32_t);
		void Bind() override;
		void Clear() override;
		void SetSize(Vec2u size) override;
	private:
		GLuint m_frameBufferId;
		GLuint m_renderBufferId;
		GLuint m_textureId;
	};
}
