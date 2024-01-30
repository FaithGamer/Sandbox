#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Log.h"
#include "Sandbox/Render/RenderTarget.h"
#include "Sandbox/Render/RenderTexture.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/RenderOptions.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/SpriteRender.h"
#include "Sandbox/Utils/Container.h"


namespace Sandbox
{
	Renderer2D::Renderer2D()
	{
		ASSERT_LOG_ERROR(Window::IsInitialized(), "Cannot create Renderer2D before Window is initialized.");

		m_rendering = false;
		//How much game unit are needed to fill the screen height
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

		m_defaultRenderOptions = makesptr<RenderOptions>();
		m_defaultRenderOptionsLayer = makesptr<RenderOptions>();
		m_defaultRenderOptionsLayer->SetDepthTest(false);
		auto window = Window::Instance();

		m_camera = Mat4(1);


		//Layer default shader
		m_defaultLayerShader = makesptr<Shader>(
			"assets/shaders/default_layer.vert",
			"assets/shaders/default_layer.frag");

		std::vector<Vec2f> screenSpace{ {-1, -1}, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> defaultLayerVertexArray = GenerateLayerVertexArray(screenSpace);

		//Screen layer
		m_layers.push_back(RenderLayer("Window", 0, window, m_defaultLayerShader, m_defaultRenderOptionsLayer, defaultLayerVertexArray));
		//CreateQuadBatch(m_layers[0], nullptr, nullptr);

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

	uint32_t Renderer2D::AddLayer(std::string name, unsigned int height, sptr<Shader> shader, sptr<RenderOptions> renderOptions)
	{
		auto ins = Instance();
		if (shader == nullptr)
			shader = ins->m_defaultLayerShader;
		if (renderOptions == nullptr)
			renderOptions = ins->m_defaultRenderOptionsLayer;

		ins->SetShaderUniformSampler(shader, ins->m_maxOffscreenLayers + 1);

		std::vector<Vec2f> screenSpace{ { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> layerVertexArray = ins->GenerateLayerVertexArray(screenSpace);
		auto windowSize = Window::GetSize();
		sptr<RenderTexture> layer;
		if (height == 0)
		{
			layer = makesptr<RenderTexture>(windowSize);
		}
		else
		{
			unsigned int width = round((float)windowSize.x / (float)windowSize.y * (float)height);
			layer = makesptr<RenderTexture>(Vec2u(width, height));
		}
		ins->m_layers.push_back(RenderLayer(name, (uint32_t)ins->m_layers.size(), layer, shader, renderOptions, layerVertexArray, height, false, false));
		ins->m_renderLayers.push_back(&ins->m_layers.back());

		return (uint32_t)ins->m_layers.size() - 1;
	}

	uint32_t Renderer2D::AddLayer(std::string name, sptr<Shader> shader, sptr<RenderOptions> renderOptions)
	{
		return AddLayer(name, 0, shader, renderOptions);
	}

	uint32_t Renderer2D::AddOffscreenLayer(std::string name, uint32_t sampler2DIndex)
	{
		auto ins = Instance();
		ASSERT_LOG_ERROR(bool(sampler2DIndex > 0 && sampler2DIndex < 16), "sampler2DIndex must be comprised between 1 and 15");
		ASSERT_LOG_ERROR(bool(ins->m_offscreenLayers.size() < 15), "Number of Offscreen layers exceeded (max 15)");

		//To do: check if sampler2DIndex hasn't been used already.

		sptr<RenderTexture> layer = makesptr<RenderTexture>(Window::GetSize());
		std::vector<Vec2f> screenSpace{ {-1, -1}, { 1, -1 }, { 1, 1 }, { -1, 1 } };
		sptr<VertexArray> layerVertexArray = ins->GenerateLayerVertexArray(screenSpace);
		uint32_t index = (uint32_t)ins->m_layers.size();
		ins->m_layers.push_back(RenderLayer(name, index, layer, ins->m_defaultLayerShader, ins->m_defaultRenderOptionsLayer, layerVertexArray, false, true));
		ins->m_offscreenLayers.push_back(OffscreenRenderLayer(layer, sampler2DIndex, index));

		return (uint32_t)ins->m_layers.size() - 1;
	}

	void Renderer2D::SetLayerScreenSpace(uint32_t layer, const std::vector<Vec2f>& screenSpace)
	{
		auto ins = Instance();
		if (screenSpace.size() != 4)
		{
			LOG_WARN("screenSpace size is not 4. Layer screen space hasn't been changed.");
			return;
		}
		if (ins->m_layers.size() <= layer)
		{
			LOG_WARN("layer id is over layer count. Layer screen space hasn't been changed.");
			return;
		}
		if (layer == 0)
		{
			LOG_WARN("Cannot change layer screen space of the screen layer. Layer screen space hasn't been changed.");
			return;
		}
		ins->m_layers[layer].vertexArray = ins->GenerateLayerVertexArray(screenSpace);
	}

	uint32_t Renderer2D::GetLayerId(std::string name)
	{
		auto ins = Instance();
		uint32_t i = 0;
		for (auto& layer : ins->m_layers)
		{
			if (layer.name == name)
			{
				return i;
			}
			i++;
		}
		LOG_WARN("No render layer with the name: " + name + " default layer returned.");
		return 0;
	}

	std::vector<uint32_t> Renderer2D::GetLayers()
	{
		auto ins = Instance();
		size_t layerCount = ins->m_layers.size();
		std::vector<uint32_t> layers(layerCount - 1);
		for (size_t i = 1; i < layerCount; i++)
		{
			layers[i] = ins->m_layers[i].index;
		}
		return layers;
	}

	uint32_t Renderer2D::GetBatchId(uint32_t layerIndex, sptr<Shader> shader, sptr<RenderOptions> renderOptions)
	{
		//To do: bake the batchs based on assets
		auto ins = Instance();

		uint32_t shaderId = 0;
		uint32_t renderOptionsId = 0;
		if (shader != nullptr)
			shaderId = shader->GetID();
		if (renderOptions != nullptr)
			renderOptionsId = renderOptions->GetID();

		uint64_t id = ins->GenerateBatchId(layerIndex, shaderId, renderOptionsId);

		auto batch = ins->m_quadBatchFinder.find(id);
		if (batch == ins->m_quadBatchFinder.end())
		{
			//Create batch if doesn't exists
			ins->CreateQuadBatch(ins->m_layers[layerIndex], shader, renderOptions);

			uint32_t index = (uint32_t)ins->m_quadBatchs.size() - 1;
			ins->m_quadBatchs.back().index = index;

			ins->m_quadBatchFinder.insert(std::make_pair(id, index));

			if (ins->m_rendering)
			{
				ins->StartBatch(index);
			}

			return index;
		}
		else
		{
			uint32_t index = batch->second;
			return index;
		}
	}

	void Renderer2D::SetLayerShader(uint32_t layer, sptr<Shader> shader)
	{
		auto ins = Instance();
		ins->SetShaderUniformSampler(shader, ins->m_maxOffscreenLayers + 1);
		ins->m_layers[layer].shader = shader;
	}

	void Renderer2D::SetLayerRenderOptions(uint32_t layer, sptr<RenderOptions> renderOptions)
	{
		auto ins = Instance();
		ins->m_layers[layer].renderOptions = renderOptions;
	}

	void Renderer2D::SetLayerHeight(uint32_t layer, unsigned int height)
	{
		auto ins = Instance();
		ins->m_layers[layer].height = height;
		auto windowSize = Window::GetSize();
		unsigned int width = round((float)windowSize.x / (float)windowSize.y * (float)height);
		ins->m_layers[layer].target->SetSize({ width, height });
	}

	void Renderer2D::PreallocateQuadBatch(int count)
	{
		for (int i = 0; i < count; i++)
		{
			m_quadBatchs.push_back(QuadBatch());
			m_freeQuadBatchs.push_back(m_quadBatchs.size() - 1);
		}
	}

	void Renderer2D::CreateQuadBatch(RenderLayer& layer, sptr<Shader> shader, sptr<RenderOptions> renderOptions)
	{
		size_t index = 0;
		if (!m_freeQuadBatchs.empty())
		{
			//Recycle a free batch
			index = m_freeQuadBatchs[0];
			Container::RemoveAt(m_freeQuadBatchs, 0);
		}
		else
		{
			//Allocate a new batch
			m_quadBatchs.push_back(QuadBatch());
			AllocateQuadBatch(m_quadBatchs.back());
			index = m_quadBatchs.size() - 1;
		}

		SetupQuadBatch(m_quadBatchs[index], layer, shader, renderOptions);

	}

	void Renderer2D::SetupQuadBatch(QuadBatch& batch, RenderLayer& layer, sptr<Shader> shader, sptr<RenderOptions> renderOptions)
	{
		if (shader == nullptr)
			shader = m_defaultShader;
		if (renderOptions == nullptr)
			renderOptions = m_defaultRenderOptions;

		batch.shader = shader;
		batch.layer = layer;
		batch.renderOptions = renderOptions;

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
		glDisable(GL_DEPTH_TEST);

		//Put offscreen layer in according texture slots
		for (auto& offscreenLayer : m_offscreenLayers)
		{
			offscreenLayer.target->BindTexture(offscreenLayer.textureUnit);
		}

		//Draw every layers

		for (auto layer = m_layers.rbegin(); layer != m_layers.rend(); layer++)
		{
			if (!layer->active || layer->index == 0 || layer->offscreen)
				continue;

			std::static_pointer_cast<RenderTexture>(layer->target)->BindTexture(0);
			layer->vertexArray->Bind();
			layer->shader->Bind();
			layer->renderOptions->Bind();
			GLuint indicesCount = layer->vertexArray->GetIndexBuffer()->GetCount();
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
			batch.renderOptions->Bind();
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
		constexpr Vec2f texCoords[4]{ Vec2f( 0.0f, 1.0f ), Vec2f( 1.0f, 1.0f ), Vec2f( 1.0f, 0.0f ), Vec2f( 0.0f, 0.0f ) };
		auto& batch = m_quadBatchs[batchIndex];
		//Check if we still have space in the batch for more indices
		if (batch.quadIndexCount >= m_maxIndices)
			NextBatch(batchIndex);


		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			batch.quadVertexPtr->position = (Vec3f)(transform.GetTransformMatrix() * batch.quadVertexPosition[i]);
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
		Transform& transform,
		SpriteRender& spriteRender,
		uint32_t batchIndex)
	{

		constexpr size_t quadVertexCount = 4;

		auto& batch = m_quadBatchs[batchIndex];

		//Check if we still have space in the batch for more indices
		if (batch.quadIndexCount >= m_maxIndices)
			NextBatch(batchIndex);

		float textureIndex = 0.0f;
		sptr<Texture> texture = spriteRender.GetTexture();
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

		sptr<Sprite> sprite = spriteRender.GetSprite();
		bool reComputePosition = transform.matrixUpdated || transform.needCompute || spriteRender.spriteDimensionsChanged;

		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			if (reComputePosition) //to do check if spriteRender dim changed
			{
				batch.quadVertexPtr->position = VertexPosition(batch.quadVertexPosition[i], transform, *sprite);
				spriteRender.preComputedPosition[i] = batch.quadVertexPtr->position;
			}
			else
			{
				batch.quadVertexPtr->position = spriteRender.preComputedPosition[i];
			}

			batch.quadVertexPtr->texCoords = sprite->GetTextureCoords(i);
			batch.quadVertexPtr->texIndex = textureIndex;
			batch.quadVertexPtr->color = spriteRender.color;

			//Incrementing the pointed value of the quad vertex array
			batch.quadVertexPtr++;
		}

		spriteRender.spriteDimensionsChanged = false;
		transform.matrixUpdated = false;
		batch.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	Vec3f Renderer2D::VertexPosition(Vec4f pos, const Transform& transform, const Sprite& sprite)
	{
		pos.x *= sprite.GetDimensions().x;
		pos.y *= sprite.GetDimensions().y;

		return (Vec3f)(transform.GetTransformMatrix() * pos * m_worldToScreenRatio);
	}

	Vec3f Renderer2D::VertexPosition(Vec4f pos, const Transform& transform, Vec2f texDim, float ppu, float width, float height)
	{
		pos.x *= width * texDim.x * ppu;
		pos.y *= height * texDim.y * ppu;

		return (Vec3f)(transform.GetTransformMatrix() * pos * m_worldToScreenRatio);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return Instance()->m_stats;
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

			if (layer.height == 0)
				layer.target->SetSize(size);
			else
			{
				auto windowSize = Window::GetSize();
				unsigned int width = round((float)windowSize.x / (float)windowSize.y * (float)layer.height);
				layer.target->SetSize({ width, layer.height });
			}
		}
	}

	sptr<VertexArray> Renderer2D::GenerateLayerVertexArray(const std::vector<Vec2f>& screenSpace)
	{
		//Create a vertex array for a layer

		//The screen coordinates the layer will be rendered into.
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