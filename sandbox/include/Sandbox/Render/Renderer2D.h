#pragma once

#include <unordered_map>
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

	class Sprite;
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
		sptr<VertexArray> vertexArray;
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

	class Renderer2D : public Singleton<Renderer2D>
	{
	public:
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};


		~Renderer2D();
		void SetRenderTarget(sptr<RenderTarget> target);

		/// @brief To be called before attempting to use any of the Draw method
		/// @param camera 
		void Begin(const Camera& camera);
		/// @brief To be called when you are done Drawing. Will Render everything on the RenderTarget (default screen)
		void End();
		void Flush(uint32_t batchIndex);

		void DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color = Vec4f(1), uint32_t batchIndex = 0);
		void DrawQuad(const Transform& transform, const Vec4f& color = Vec4f(1), uint32_t batchIndex = 0);
		void DrawTexturedQuad(const Vec3f& position, const Vec2f& scale, sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			uint32_t batchIndex = 0);
		void DrawTexturedQuad(const Transform& transform, sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
			uint32_t batchIndex = 0);
		void DrawSprite(const Transform& transform, const Sprite& sprite, uint32_t batchIndex);

		/// @brief Add a layer on the bottom of the render queue.
		/// The order cannot be changed ever again, and the layers cannot be removed.
		/// @param name A friendly identifier.
		/// @return The identifier to use when refering to this layer.
		uint32_t AddLayer(std::string name, sptr<Shader> shader = nullptr, sptr<StencilMode> stencil = nullptr);
		/// @brief Add a layer that won't display but can be used in the shader of other layers.
		/// Usage example: normal map.
		/// @param sampler2DIndex Wich index the texture will be available in the sampler2D uniform.
		/// Must be comprised in between 1 and 15.
		uint32_t AddOffscreenLayer(std::string name, uint32_t sampler2DIndex);
	
		/// @brief Set the space the layer take up on the screen,
		/// @param screenSpace  normalized screen space (vector must be of size 4)
		void SetLayerScreenSpace(uint32_t layer, const std::vector<Vec2f>& screenSpace);
		/// @brief Set the shader used to render a layer.
		void SetLayerShader(uint32_t layer, sptr<Shader> shader);
		/// @brief Set the StencilMode used when rendering a layer
		void SetLayerStencilMode(uint32_t layer, sptr<StencilMode> stencil);
		/// @brief Can be used for optimization if you know how much QuadBatch you we be using.
		/// @param count Number of quadbatch to allocate space for.
		void PreallocateQuadBatch(int count);
		/// @brief Use with care only if you know what you are doing, as every attempt to draw using this batch will result in undefined behaviour
		/// @param batch 
		void FreeQuadBatch(uint32_t batch);
		
		/// @brief Get a layer id from it's name
		/// @param name 
		/// @return LayerId
		uint32_t GetLayerId(std::string name);
		/// @brief Get Every layers id
		std::vector<uint32_t> GetLayers();
		/// @brief Get a batch based on what layer/shader/stencilmode is used. nullptr = default shader/stenctilmode
		/// @return BatchId
		uint32_t GetBatchId(uint32_t layerIndex, sptr<Shader> shader = nullptr, sptr<StencilMode> stencil = nullptr);
		/// @brief Give you some stats about the current rendering batch.
		Statistics GetStats();

		void OnWindowResize(Vec2u size);
	private:
		friend Engine;
		friend Singleton<Renderer2D>;
		Renderer2D();

		void StartBatch(uint32_t batchIndex);
		void NextBatch(uint32_t batchIndex);

		void SetupQuadBatch(QuadBatch& batch, RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		void AllocateQuadBatch(QuadBatch& batch);
		void CreateQuadBatch(RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil);
		uint64_t GenerateBatchId(uint64_t a, uint64_t b, uint64_t c);
		void RenderLayers();
		void SetShaderUniformSampler(sptr<Shader> shader, uint32_t count);
		Vec3f VertexPosition(const Vec4f& pos, const Transform& transform, Vec2f texDim, float ppu, float width, float height);
		Vec3f VertexPosition(const Vec4f& pos, const Transform& transform, const Sprite& sprite);
		sptr<VertexArray> GenerateLayerVertexArray(const std::vector<Vec2f>& screenSpace);
	private:

		// Batched Quads

		std::unordered_map<uint64_t, uint32_t> m_quadBatchFinder;
		std::vector<QuadBatch> m_quadBatchs;
		std::vector<size_t> m_freeQuadBatchs;

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
		sptr<RenderTarget> m_target;
		std::vector<RenderLayer> m_layers;
		std::vector<OffscreenRenderLayer> m_offscreenLayers;
		std::vector<RenderLayer*> m_renderLayers;
		sptr<Shader> m_defaultLayerShader;

		//Others

		bool m_rendering;
		Statistics m_stats;
		float m_worldToScreenRatio;
	};
}
