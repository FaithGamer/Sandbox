#pragma once


#include "Sandbox/std_macros.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Render/Buffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Transform.h"
#include "Sandbox/Render/RenderTarget.h"
#include "Sandbox/Singleton.h"

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
	class RenderTexture;
	class Shader;
	class StencilMode;
	

	struct QuadVertex
	{
		Vec3f position;
		Vec4f color;
		Vec2f texCoords;
		float texIndex;
	};


	struct RenderLayer 
	{
		std::string name = "RenderLayerDefault";
		uint32_t index = 0;
		sptr<RenderTarget> target = nullptr;
		sptr<Shader> shader = nullptr;
		sptr<StencilMode> stencil = nullptr;
		bool active = false;
		bool offscreen = false;
	};

	struct OffscreenRenderLayer
	{
		sptr<RenderTexture> target = nullptr;
		uint32_t textureUnit = 1;
		uint32_t index = 1;
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

		RenderLayer layer;
		sptr<StencilMode> stencil;
		sptr<Shader> shader;
	
	};

	class Renderer2D 
	{
	public:
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};


		Renderer2D();
		~Renderer2D();

		/// @brief Add a layer on the bottom of the render queue.
		/// The order cannot be changed ever again, and the layers cannot be removed.
		/// @param name A friendly identifier.
		/// @return The identifier to use when refering to this layer.
		uint32_t AddLayer(std::string name);
		/// @brief Add a layer that won't display but can be used in the shader of other layers.
		/// Usage example: normal map.
		/// @param sampler2DIndex Wich index the texture will be available in the sampler2D uniform.
		/// Must be comprised in between 1 and 15.
		uint32_t AddOffscreenLayer(std::string name, uint32_t sampler2DIndex);
		uint32_t GetLayerId(std::string name);
		void SetLayerShader(uint32_t layer, sptr<Shader> shader);
		void SetLayerStencilMode(uint32_t layer, sptr<StencilMode> stencil);

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
		friend Engine;

		void StartBatch(uint32_t pipelineIndex);
		void NextBatch(uint32_t pipelineIndex);
		
		void FreeQuadPipeline(uint32_t pipeline);
		void SetupQuadPipeline(QuadBatch& batch, RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		void AllocateQuadPipeline(QuadBatch& batch);
		void CreateQuadPipeline(RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		uint64_t GeneratePipelineId(uint64_t a, uint64_t b, uint64_t c);
		void RenderLayers();
		void SetShaderUniformSampler(sptr<Shader> shader, uint32_t count);
		// Batched Quads

		std::unordered_map<uint64_t, uint32_t> m_quadPipelineFinder;
		std::vector<QuadBatch> m_quadPipelines;
		std::vector<size_t> m_freeQuadPipelines;

		uint32_t m_maxQuads;
		uint32_t m_maxVertices;
		uint32_t m_maxIndices;
		uint32_t m_maxTextureSlots;
		uint32_t m_maxOffscreenLayers;

		sptr<Shader> m_defaultShader;
		sptr<StencilMode> m_defaultStencilMode;
		sptr<Texture> m_whiteTexture;
		sptr<UniformBuffer> m_cameraUniformBuffer;
		sptr<IndexBuffer> m_quadIndexBuffer;

		Mat4 m_camera;

		//Layers

		std::vector<RenderLayer> m_layers;
		std::vector<OffscreenRenderLayer> m_offscreenLayers;
		sptr<VertexArray> m_layerVertexArray;
		sptr<Shader> m_defaultLayerShader;

		Statistics m_stats;
	};
}
