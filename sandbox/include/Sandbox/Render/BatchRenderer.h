#pragma once


#include "Sandbox/std_macros.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Render/Buffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Transform.h"
#include "Sandbox/Render/RenderTarget.h"

namespace Sandbox
{
	struct Sprite2D
	{

	};

	class RenderTarget;
	class Shader;
	class StencilMode;
	

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
		struct Batch
		{
			Batch();
			sptr<VertexArray> quadVertexArray;
			sptr<VertexBuffer> quadVertexBuffer;
		

			uint32_t quadIndexCount = 0;
			QuadVertex* quadVertexBase = nullptr;
			QuadVertex* quadVertexPtr = nullptr;

			std::vector<Texture*> textureSlots;
			uint32_t textureSlotIndex = 1;

			//it's a Vec4 so it can be transformed with a 4x4 matrix
			Vec4f quadVertexPosition[4]
			{
				{-0.5f, -0.5f, 0.0f, 1.0f},
				{0.5f, -0.5f, 0.0f, 1.0f},
				{0.5f, 0.5f, 0.0f, 1.0f},
				{-0.5f, 0.5f, 0.0f, 1.0f}
			};

			sptr<StencilMode> stencil;
			Shader* shader;
		};

		struct Pipeline
		{
			RenderTarget* layer;
			std::vector<std::vector<Batch>> batches;
		};

		BatchRenderer();
		~BatchRenderer();

		uint32_t RegisterShader(Shader* shader);
		void DeleteShader(uint32_t shaderId);
		uint32_t GetShaderId(Shader* shader);
		uint32_t GetDefaultShaderId();

		uint32_t RegisterStencilMode(StencilMode* stencil);
		void DeleteStencilMode(uint32_t stencilId);
		uint32_t GetStencilModeId(StencilMode* stencil);
		uint32_t GetDefaultStencilModeId();

		uint32_t AddLayer(std::string name);
		void DeleteLayer(std::string name);
		void DeleteLayer(uint32_t layerId);
		uint32_t GetLayerId(std::string name);
		uint32_t GetDefaultLayerId();

		void BeginScene(const Camera& camera);
		void EndScene();
		void Flush(uint32_t layerId = 0, uint32_t shaderId = 0, uint32_t stencilStateId = 0);

		void DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color = Vec4f(1));
		void DrawQuad(const Transform& transform, const Vec4f& color = Vec4f(1));
		void DrawTexturedQuad(const Vec3f& position, const Vec2f& scale, const Texture& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1));
		void DrawTexturedQuad(const Transform& transform, const Texture& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1));

		void DrawTexturedQuad(const Transform& transform, const Texture& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			uint32_t layerId = 0, uint32_t shaderId = 0, uint32_t stencilStateId = 0);

		Statistics GetStats();
	private:
		void StartBatch();
		void NextBatch();
		void GeneratePipelines();
		bool m_mustGeneratePipelines;

		std::vector<Pipeline> m_pipelines;
		std::vector<Shader*> m_shaders;
		std::vector<RenderTarget*> m_layers;
		std::vector<StencilMode*> m_stencils;

		uint32_t m_maxQuads;
		uint32_t m_maxVertices;
		uint32_t m_maxIndices;
		uint32_t m_maxTextureSlots;

		sptr<Shader> m_defaultShader;
		sptr<Texture> m_whiteTexture;
		Mat4 m_camera = Mat4(1.0f);
		sptr<UniformBuffer> m_cameraUniformBuffer;
		sptr<IndexBuffer> m_quadIndexBuffer;

		Statistics m_stats;
	};

}
