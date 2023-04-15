#pragma once


#include "Sandbox/std_macros.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Buffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Transform.h"

namespace Sandbox
{
	class RenderTarget;
	class Material;

	struct QuadVertex
	{
		Vec3f position;
		Vec4f color;
		Vec2f texCoords;
		float texIndex;
	};

	class BatchRenderer
	{
	public:
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};
		struct Data
		{
			sptr<VertexArray> quadVertexArray;
			sptr<VertexBuffer> quadVertexBuffer;
			sptr<Shader> quadShader;
			sptr<Texture> whiteTexture;

			uint32_t quadIndexCount = 0;
			QuadVertex* quadVertexBase = nullptr;
			QuadVertex* quadVertexPtr = nullptr;

			std::vector<sptr<Texture>> textureSlots;
			uint32_t textureSlotIndex = 1;

			//it's a Vec4 so it can be transformed with a 4x4 matrix
			Vec4f quadVertexPosition[4]
			{
				{-0.5f, -0.5f, 0.0f, 1.0f},
				{0.5f, -0.5f, 0.0f, 1.0f},
				{0.5f, 0.5f, 0.0f, 1.0f},
				{-0.5f, 0.5f, 0.0f, 1.0f}
			};

			Mat4 camera = Mat4(1.0f);
			sptr<UniformBuffer> cameraUniformBuffer;

			Statistics stats;
		};

		BatchRenderer();
		~BatchRenderer();

		void BeginScene(const Camera& camera); //TO DO: render target ?
		void EndScene();
		void Flush();

		void DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color = Vec4f(1));
		void DrawQuad(const Transform& transform, const Vec4f& color = Vec4f(1));
		void DrawTexturedQuad(const Vec3f& position, const Vec2f& scale, const sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1));
		void DrawTexturedQuad(const Transform& transform, const sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1));

		void DrawTexturedQuad(const Transform& transform, const Texture& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			const RenderTarget& target, const Material& material);

		Statistics GetStats();
	private:
		void StartBatch();
		void NextBatch();

		Data m_data;

		uint32_t m_maxQuads;
		uint32_t m_maxVertices;
		uint32_t m_maxIndices;
		uint32_t m_maxTextureSlots;
	};

}
