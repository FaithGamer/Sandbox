#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Log.h"
#include "Sandbox/Render/RenderTarget.h"
#include "Sandbox/Render/RenderTexture.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/StencilMode.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Sprite.h"
#include "Sandbox/Vector.h"


namespace Sandbox
{
	Renderer2D::Renderer2D()
	{
		ASSERT_LOG_ERROR(Window::IsInitialized(), "Cannot create Renderer2D before Window is initialized.");

		m_rendering = false;
		m_worldToScreenRatio = 0.02f;
		//Limitations
		m_maxQuads = 1000;
		m_maxVertices = m_maxQuads * 4;
		m_maxIndices = m_maxQuads * 6;
		m_maxTextureSlots = 16;
		m_maxOffscreenLayers = 15;


		//IndexBuffer (quads)
		uint32_t* quadIndices = new uint32_t[m_maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < m_maxIndices; i += 6)
		{
			quadIndices[0 + i] = 0 + offset;
			quadIndices[1 + i] = 1 + offset;
			quadIndices[2 + i] = 2 + offset;
			quadIndices[3 + i] = 2 + offset;
			quadIndices[4 + i] = 3 + offset;
			quadIndices[5 + i] = 0 + offset;

			offset += 4;
		}

		m_quadIndexBuffer = makesptr<IndexBuffer>(quadIndices, m_maxIndices);
		delete[] quadIndices;

		//Camera uniform buffer
		m_cameraUniformBuffer = makesptr<UniformBuffer>(sizeof(Mat4), 0);

		m_whiteTexture = makesptr<Texture>();

		m_defaultShader = makesptr<Shader>(
			"assets/shaders/batch_renderer.vert",
			"assets/shaders/batch_renderer.frag");

		m_defaultStencilMode = makesptr<StencilMode>();
		auto window = Window::Instance();

		m_camera = Mat4(1);


		//Layer default shader
		m_defaultLayerShader = makesptr<Shader>(
			"assets/shaders/default_layer.vert",
			"assets/shaders/default_layer.frag");

		std::vector<Vec2f> screenSpace{ {-1, -1}, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> defaultLayerVertexArray = GenerateLayerVertexArray(screenSpace);

		//Screen layer
		m_layers.push_back(RenderLayer("Window", 0, window, m_defaultLayerShader, m_defaultStencilMode, defaultLayerVertexArray));
		CreateQuadBatch(m_layers[0], nullptr, nullptr);

		SetShaderUniformSampler(m_defaultLayerShader, m_maxOffscreenLayers + 1);

		//Listen to window resize signal

		Window::GetResizeSignal()->AddListener(&Renderer2D::OnWindowResize, this);

		//Set render target to be the window by default
		SetRenderTarget(window);
	}

	Renderer2D::~Renderer2D()
	{
		//To do

	}

	void Renderer2D::SetShaderUniformSampler(sptr<Shader> shader, uint32_t count)
	{
		std::vector<int> sampler;
		for (uint32_t i = 0; i < count; i++)
		{
			sampler.emplace_back(i);
		}
		shader->SetUniformArray("uTextures", &sampler[0], (GLsizei)sampler.size());
	}

	uint64_t Renderer2D::GenerateBatchId(uint64_t a, uint64_t b, uint64_t c)
	{
		b = (b + 1) * 10000;
		a = (a + 1) * 100000000;
		return a + b + c;
	}

	void Renderer2D::SetRenderTarget(sptr<RenderTarget> target)
	{
		m_target = target;
	}

	uint32_t Renderer2D::AddLayer(std::string name, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		if (shader == nullptr)
			shader = m_defaultLayerShader;
		if (stencil == nullptr)
			stencil = m_defaultStencilMode;

		SetShaderUniformSampler(shader, m_maxOffscreenLayers + 1);

		std::vector<Vec2f> screenSpace{ {-1, -1}, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> layerVertexArray = GenerateLayerVertexArray(screenSpace);
		sptr<RenderTexture> layer = makesptr<RenderTexture>(Window::GetSize());
		m_layers.push_back(RenderLayer(name, (uint32_t)m_layers.size(), layer, shader, stencil, layerVertexArray, false, false));
		m_renderLayers.push_back(&m_layers.back());

		return (uint32_t)m_layers.size() - 1;
	}

	uint32_t Renderer2D::AddOffscreenLayer(std::string name, uint32_t sampler2DIndex)
	{
		ASSERT_LOG_ERROR(bool(sampler2DIndex > 0 && sampler2DIndex < 16), "sampler2DIndex must be comprised between 1 and 15");
		ASSERT_LOG_ERROR(bool(m_offscreenLayers.size() < 15), "Number of Offscreen layers exceeded (max 15)");

		//To do: check if sampler2DIndex hasn't been used already.

		sptr<RenderTexture> layer = makesptr<RenderTexture>(Window::GetSize());
		std::vector<Vec2f> screenSpace{ {-1, -1}, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> layerVertexArray = GenerateLayerVertexArray(screenSpace);
		uint32_t index = (uint32_t)m_layers.size();
		m_layers.push_back(RenderLayer(name, index, layer, m_defaultLayerShader, m_defaultStencilMode, layerVertexArray, false, true));
		m_offscreenLayers.push_back(OffscreenRenderLayer(layer, sampler2DIndex, index));

		return (uint32_t)m_layers.size() - 1;
	}

	void Renderer2D::SetLayerScreenSpace(uint32_t layer, const std::vector<Vec2f>& screenSpace)
	{
		if (screenSpace.size() != 4)
		{
			LOG_WARN("screenSpace size is not 4. Layer screen space hasn't been changed.");
			return;
		}
		if (m_layers.size() <= layer)
		{
			LOG_WARN("layer id is over layer count. Layer screen space hasn't been changed.");
			return;
		}
		if (layer == 0)
		{
			LOG_WARN("Cannot change layer screen space of the screen layer. Layer screen space hasn't been changed.");
			return;
		}
		m_layers[layer].vertexArray = GenerateLayerVertexArray(screenSpace);
	}

	uint32_t Renderer2D::GetLayerId(std::string name)
	{
		uint32_t i = 0;
		for (auto& layer : m_layers)
		{
			if (layer.name == name)
			{
				return i;
			}
		}
		LOG_WARN("No render layer with the name: " + name + " default layer returned.");
		return 0;
	}

	std::vector<uint32_t> Renderer2D::GetLayers()
	{
		size_t layerCount = m_layers.size();
		std::vector<uint32_t> layers(layerCount-1);
		for (size_t i = 1; i < layerCount; i++)
		{
			layers[i] = m_layers[i].index;
		}
		return layers;
	}

	void Renderer2D::SetLayerShader(uint32_t layer, sptr<Shader> shader)
	{
		SetShaderUniformSampler(shader, m_maxOffscreenLayers + 1);
		m_layers[layer].shader = shader;
	}

	void Renderer2D::SetLayerStencilMode(uint32_t layer, sptr<StencilMode> stencil)
	{
		m_layers[layer].stencil = stencil;
	}

	void Renderer2D::PreallocateQuadBatch(int count)
	{
		for (int i = 0; i < count; i++)
		{
			m_quadBatchs.push_back(QuadBatch());
			m_freeQuadBatchs.push_back(m_quadBatchs.size() - 1);
		}
	}

	uint32_t Renderer2D::GetBatchId(uint32_t layerIndex, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		//To do: bake the batchs based on assets

		uint32_t shaderId = 0;
		uint32_t stencilId = 0;
		if (shader != nullptr)
			shaderId = shader->GetID();
		if (stencil != nullptr)
			stencilId = stencil->GetID();

		uint64_t id = GenerateBatchId(layerIndex, shaderId, stencilId);

		auto batch = m_quadBatchFinder.find(id);
		if (batch == m_quadBatchFinder.end())
		{
			//Create batch if doesn't exists
			CreateQuadBatch(m_layers[layerIndex], shader, stencil);

			uint32_t index = (uint32_t)m_quadBatchs.size() - 1;
			m_quadBatchs.back().index = index;

			m_quadBatchFinder.insert(std::make_pair(id, index));

			if (m_rendering)
			{
				StartBatch(index);
			}

			return index;
		}
		else
		{
			uint32_t index = batch->second;
			return index;
		}
	}

	void Renderer2D::CreateQuadBatch(RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		size_t index = 0;
		if (!m_freeQuadBatchs.empty())
		{
			//Recycle a free batch
			index = m_freeQuadBatchs[0];
			Vector::RemoveAt(m_freeQuadBatchs, 0);
		}
		else
		{
			//Allocate a new batch
			m_quadBatchs.push_back(QuadBatch());
			AllocateQuadBatch(m_quadBatchs.back());
			index = m_quadBatchs.size() - 1;
		}

		SetupQuadBatch(m_quadBatchs[index], layer, shader, stencil);

	}

	void Renderer2D::SetupQuadBatch(QuadBatch& batch, RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		if (shader == nullptr)
			shader = m_defaultShader;
		if (stencil == nullptr)
			stencil = m_defaultStencilMode;

		batch.shader = shader;
		batch.layer = layer;
		batch.stencil = stencil;

		//Assign relevant texture unit to the sampler2D[] uniform uTextures
		std::vector<int> sampler;
		for (uint32_t i = 0; i < m_maxTextureSlots; i++)
		{
			sampler.emplace_back(i);
		}
		batch.shader->SetUniformArray("uTextures", &sampler[0], (GLsizei)sampler.size());

		//Bind shader to the camera uniform buffer
		batch.shader->BindUniformBlock("camera", 0);

	}

	void Renderer2D::AllocateQuadBatch(QuadBatch& batch)
	{
		//Vertex buffer (quads)
		batch.quadVertexBuffer = makesptr<VertexBuffer>(m_maxVertices * sizeof(QuadVertex));
		batch.quadVertexBuffer->SetLayout({
			{ShaderDataType::Vec3f, "aPosition"},
			{ShaderDataType::Vec4f, "aColor"},
			{ShaderDataType::Vec2f, "aTexCoords"},
			{ShaderDataType::Float, "aTexIndex"}
			});

		//Vertex Array
		batch.quadVertexArray = makesptr<VertexArray>(batch.quadVertexBuffer, m_quadIndexBuffer);

		//Vertex data on CPU
		batch.quadVertexBase = new QuadVertex[m_maxVertices];

		//Initialize texture slots size
		batch.textureSlots.resize(m_maxTextureSlots);

		//White texture in slot 0
		batch.textureSlots[0] = m_whiteTexture;

		//Vertices quad position before any transformation
		batch.quadVertexPosition[0] = Vec4f(-0.5f, 0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[1] = Vec4f(-0.5f, -0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[2] = Vec4f(0.5f, -0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[3] = Vec4f(0.5f, 0.5f, 0.0f, 1.0f);

	}

	void Renderer2D::FreeQuadBatch(uint32_t batch)
	{
		m_freeQuadBatchs.push_back(batch);
	}

	void Renderer2D::Begin(const Camera& camera)
	{
		glEnable(GL_DEPTH_TEST);
		m_rendering = true;

		for (auto& layer : m_layers)
		{
			layer.active = false;
		}
		//Set the camera matrices into the uniform buffer

		m_camera = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		m_worldToScreenRatio = camera.worldToScreenRatio * 2;
		m_cameraUniformBuffer->SetData(&m_camera, sizeof(Mat4), 0);

		//Clear layers
		for (auto& layer : m_layers)
		{
			layer.target->Clear();
		}

		//ResetStats
		m_stats.drawCalls = 0;
		m_stats.quadCount = 0;

		for (auto& batch : m_quadBatchs)
		{
			StartBatch(batch.index);
		}

	}

	void Renderer2D::End()
	{
		for (auto& batch : m_quadBatchs)
			Flush(batch.index);
		RenderLayers();
		m_rendering = false;
	}

	void Renderer2D::RenderLayers()
	{
		//Bind target framebuffer
		m_target->Bind();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		//Put offscreen layer in according texture slots
		for (auto& offscreenLayer : m_offscreenLayers)
		{
			offscreenLayer.target->BindTexture(offscreenLayer.textureUnit);
		}

		//Draw every layers
		for (auto layer = m_renderLayers.rbegin(); layer != m_renderLayers.rend(); layer++)
		{
			if (!(*layer)->active)
				continue;

			std::static_pointer_cast<RenderTexture>((*layer)->target)->BindTexture(0);
			(*layer)->vertexArray->Bind();
			(*layer)->shader->Bind();
			(*layer)->stencil->Bind();
			GLuint indicesCount = (*layer)->vertexArray->GetIndexBuffer()->GetCount();
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		}
	}

	void Renderer2D::Flush(uint32_t batchIndex)
	{
		QuadBatch& batch = m_quadBatchs[(size_t)batchIndex];
		if (batch.quadIndexCount)
		{
			//Send the vertex data from CPU to GPU
			uint32_t dataSize = (uint32_t)((uint8_t*)batch.quadVertexPtr - (uint8_t*)batch.quadVertexBase);
			batch.quadVertexBuffer->SetData(batch.quadVertexBase, dataSize);

			for (uint32_t i = 0; i < batch.textureSlotIndex; i++)
			{
				batch.textureSlots[i]->Bind(i);
			}

			//Issue the draw call after binding adequat context
			batch.layer.target->Bind();
			batch.quadVertexArray->Bind();
			batch.shader->Bind();
			batch.stencil->Bind();
			glDrawElements(GL_TRIANGLES, batch.quadIndexCount, GL_UNSIGNED_INT, 0);

			m_stats.drawCalls++;
			m_layers[batch.layer.index].active = true;
		}
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color, uint32_t batchIndex)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawQuad(transform, color, batchIndex);
	}

	void Renderer2D::DrawQuad(const Transform& transform, const Vec4f& color, uint32_t batchIndex)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr Vec2f texCoords[4]{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } };
		auto& batch = m_quadBatchs[batchIndex];
		//Check if we still have space in the batch for more indices
		if (batch.quadIndexCount >= m_maxIndices)
			NextBatch(batchIndex);


		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			batch.quadVertexPtr->position = transform.GetTransformMatrix() * batch.quadVertexPosition[i];
			batch.quadVertexPtr->texCoords = texCoords[i];
			batch.quadVertexPtr->texIndex = 0;
			batch.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			batch.quadVertexPtr++;
		}

		batch.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	void Renderer2D::DrawTexturedQuad(
		const Vec3f& position,
		const Vec2f& scale,
		sptr<Texture>& texture,
		const std::vector<Vec2f>& texCoords,
		const Vec4f& color,
		uint32_t batchIndex)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawTexturedQuad(transform, texture, texCoords, color, 0);
	}

	void Renderer2D::DrawTexturedQuad(
		const Transform& transform,
		sptr<Texture>& texture,
		const std::vector<Vec2f>& texCoords,
		const Vec4f& color,
		uint32_t batchIndex)
	{
		constexpr size_t quadVertexCount = 4;
		ASSERT_LOG_ERROR(bool(texCoords.size() >= 4), "texCoords size is too low.");

		auto& batch = m_quadBatchs[batchIndex];

		//Check if we still have space in the batch for more indices
		if (batch.quadIndexCount >= m_maxIndices)
			NextBatch(batchIndex);

		float textureIndex = 0.0f;

		//Find if the texture has been used in the current batch
		for (uint32_t i = 1; i < batch.textureSlotIndex; i++)
		{
			if (batch.textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			//Check if there is still space for a texture
			if (batch.textureSlotIndex >= m_maxTextureSlots)
				NextBatch(batchIndex);

			//Set the current texture index
			textureIndex = (float)batch.textureSlotIndex;
			//Add the texture in the appropriate slot
			batch.textureSlots[batch.textureSlotIndex] = texture;
			//Increment the current index
			batch.textureSlotIndex++;
		}

		float texWidth = std::fabs(texCoords[1].x - texCoords[2].x);
		float texHeight = std::fabs(texCoords[0].y - texCoords[1].y);
		float ppu = texture->GetPixelPerUnit();
		Vec2f texDim = texture->GetSize();

		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			batch.quadVertexPtr->position = VertexPosition(batch.quadVertexPosition[i], transform, texDim, ppu, texWidth, texHeight);
			batch.quadVertexPtr->texCoords = texCoords[i];
			batch.quadVertexPtr->texIndex = textureIndex;
			batch.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			batch.quadVertexPtr++;
		}

		batch.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	void Renderer2D::DrawSprite(
		const Transform& transform,
		const Sprite& sprite,
		uint32_t batchIndex)
	{
		constexpr size_t quadVertexCount = 4;

		auto& batch = m_quadBatchs[batchIndex];

		//Check if we still have space in the batch for more indices
		if (batch.quadIndexCount >= m_maxIndices)
			NextBatch(batchIndex);

		float textureIndex = 0.0f;
		sptr<Texture> texture = sprite.GetTexture();
		//Find if the texture has been used in the current batch
		for (uint32_t i = 1; i < batch.textureSlotIndex; i++)
		{
			if (batch.textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			//Check if there is still space for a texture
			if (batch.textureSlotIndex >= m_maxTextureSlots)
				NextBatch(batchIndex);

			//Set the current texture index
			textureIndex = (float)batch.textureSlotIndex;
			//Add the texture in the appropriate slot
			batch.textureSlots[batch.textureSlotIndex] = texture;
			//Increment the current index
			batch.textureSlotIndex++;
		}

		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			batch.quadVertexPtr->position = VertexPosition(batch.quadVertexPosition[i], transform, sprite);
			batch.quadVertexPtr->texCoords = sprite.textureCoords[i];
			batch.quadVertexPtr->texIndex = textureIndex;
			batch.quadVertexPtr->color = sprite.color;

			//Incrementing the pointed value of the quad vertex array
			batch.quadVertexPtr++;
		}

		batch.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	Vec3f Renderer2D::VertexPosition(const Vec4f& pos, const Transform& transform, const Sprite& sprite)
	{
		Vec3f position = transform.GetTransformMatrix() * pos * m_worldToScreenRatio;

		position.x *= sprite.dimensions.x;
		position.y *= sprite.dimensions.y;

		return position;
	}

	Vec3f Renderer2D::VertexPosition(const Vec4f& pos, const Transform& transform, Vec2f texDim, float ppu, float width, float height)
	{
		Vec3f position = transform.GetTransformMatrix() * pos * m_worldToScreenRatio;
		position.x *= width * texDim.x * ppu;
		position.y *= height * texDim.y * ppu;

		return position;
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return m_stats;
	}

	void Renderer2D::StartBatch(uint32_t batchIndex)
	{
		//Reset vertex array data
		m_quadBatchs[batchIndex].quadVertexPtr = m_quadBatchs[batchIndex].quadVertexBase;

		//Reset counter
		m_quadBatchs[batchIndex].quadIndexCount = 0;

		//Reset texture slot index
		m_quadBatchs[batchIndex].textureSlotIndex = 1;

	}

	void Renderer2D::NextBatch(uint32_t batchIndex)
	{
		Flush(batchIndex);
		StartBatch(batchIndex);
	}

	void Renderer2D::OnWindowResize(Vec2u size)
	{
		for (auto& layer : m_layers)
		{
			if (layer.index == 0)
				continue;
			layer.target->SetSize(size);
		}
	}

	sptr<VertexArray> Renderer2D::GenerateLayerVertexArray(const std::vector<Vec2f>& screenSpace)
	{
		//Create a vertex array for a layer

		//The screen coordinates the layer will be redered into.
		float layerVertices[]
		{
		screenSpace[0].x, screenSpace[0].y, 0,  0.0, 0.0,
		screenSpace[1].x, screenSpace[1].y, 0,  1.0, 0.0,
		screenSpace[2].x, screenSpace[2].y, 0,  1.0, 1.0,
		screenSpace[3].x, screenSpace[3].y, 0,  0.0, 1.0,
		};

		AttributeLayout layout({
			{ ShaderDataType::Vec3f, "aPosition" },
			{ ShaderDataType::Vec2f, "aTexCoords" } });
		sptr<VertexBuffer> layerVertexBuffer = makesptr<VertexBuffer>(layerVertices, sizeof(layerVertices), layout);

		uint32_t layerIndices[]
		{
			0, 1, 2,
			2, 3, 0
		};

		sptr<IndexBuffer> layerIndexBuffer = makesptr<IndexBuffer>(layerIndices, 6, GL_STATIC_DRAW);

		return makesptr<VertexArray>(layerVertexBuffer, layerIndexBuffer);
	}
}