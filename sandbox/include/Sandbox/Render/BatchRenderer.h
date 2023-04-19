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
		Texture* texture;
		std::vector<Vec2f> texCoords;
		Vec4f color;
	};

	struct RenderOptions
	{
		void SetLayer(uint32_t layer);
		void SetStencilMode(uint32_t stencil);
		void SetShader(uint32_t shader);

		uint32_t pipeline = 0;
		uint32_t layer = 0;
		uint32_t stencil = 0;
		uint32_t shader = 0;
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

	struct QuadBatch
	{
		uint32_t index = 0;
		uint32_t userCount = 0;

		sptr<VertexArray> quadVertexArray;
		sptr<VertexBuffer> quadVertexBuffer;


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

		sptr<RenderTarget> layer;
		sptr<StencilMode> stencil;
		sptr<Shader> shader;
	
	};

	class BatchRenderer
	{
	public:
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};


		BatchRenderer();
		~BatchRenderer();

		uint32_t AddLayer(std::string name);
		uint32_t GetLayerId(std::string name);
		void SetLayerShader(uint32_t layer, sptr<Shader> shader);
		void SetLayerStencilMode(uint32_t layer, sptr<StencilMode> stencil);
		void SetLayerTextureUnit(uint32_t layer, uint32_t textureUnit);
		void SetLayerAsTexture

		void PreallocateQuadPipeline(int count);
		uint32_t AddQuadPipelineUser(uint32_t layerIndex, sptr<Shader> shader, sptr<StencilMode> stencil);
		void RemoveQuadPipelineUser(uint32_t pipeline);
		

		void BeginScene(const Camera& camera);
		void EndScene();
		void Flush(uint32_t pipelineIndex);

		void DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color = Vec4f(1), uint32_t pipelineIndex = 0);
		void DrawQuad(const Transform& transform, const Vec4f& color = Vec4f(1), uint32_t pipelineIndex = 0);
		void DrawTexturedQuad(const Vec3f& position, const Vec2f& scale, sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			uint32_t pipelineIndex = 0);
		void DrawTexturedQuad(const Transform& transform, sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			uint32_t pipelineIndex = 0);

		Statistics GetStats();
	private:
		void StartBatch(uint32_t pipelineIndex);
		void NextBatch(uint32_t pipelineIndex);
		
		void FreeQuadPipeline(uint32_t pipeline);
		void SetupQuadPipeline(QuadBatch& batch, sptr<RenderTarget> layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		void AllocateQuadPipeline(QuadBatch& batch);
		void CreateQuadPipeline(sptr<RenderTarget> layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		uint64_t GeneratePipelineId(uint32_t a, uint32_t b, uint32_t c);
		bool m_mustGeneratePipelines;

		std::unordered_map<uint64_t, uint32_t> m_quadPipelineFinder;
		std::vector<QuadBatch> m_quadPipelines;
		std::vector<size_t> m_freeQuadPipelines;

		struct Layer
		{
			std::string name;
			sptr<RenderTarget> target;
		};

		std::vector<Layer> m_layers;

		uint32_t m_maxQuads;
		uint32_t m_maxVertices;
		uint32_t m_maxIndices;
		uint32_t m_maxTextureSlots;

		sptr<Shader> m_defaultShader;
		sptr<RenderTarget> m_defaultRenderTarget;
		sptr<StencilMode> m_defaultStencilMode;

		sptr<Texture> m_whiteTexture;

		sptr<UniformBuffer> m_cameraUniformBuffer;
		sptr<IndexBuffer> m_quadIndexBuffer;


		Mat4 m_camera;

		Statistics m_stats;
	};
}
