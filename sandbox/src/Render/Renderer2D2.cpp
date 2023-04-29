#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Sandbox/Render/Renderer2D2.h"
#include "Sandbox/Log.h"
#include "Sandbox/Render/RenderTarget.h"
#include "Sandbox/Render/RenderTexture.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/StencilMode.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Vector.h"

namespace Sandbox
{
	Renderer2D2::Renderer2D2()
	{
		ASSERT_LOG_ERROR(Window::IsInitialized(), "Cannot create Renderer2D2 before Window is initialized.");

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

		//Screen layer
		m_layers.push_back(RenderLayer("Window", 0, window, m_defaultLayerShader, m_defaultStencilMode));
		CreateQuadPipeline(m_layers[0], nullptr, nullptr);

		//Layer default shader
		m_defaultLayerShader = makesptr<Shader>(
			"assets/shaders/default_layer.vert",
			"assets/shaders/default_layer.frag");

		//Vertex Array for layer drawing

		float layerVertices[]
		{
		-1.f, -1.f, 0,  0.f, 1.f,
		 1.f, -1.f, 0,  1.f, 1.f,
		 1.f,  1.f, 0,  1.f, 0.f,
		 -1.f,  1.f, 0,  0.f, 0.f,
		};

		AttributeLayout layout({ { ShaderDataType::Vec3f, "aPosition" } });
		sptr<VertexBuffer> layerVertexBuffer = makesptr<VertexBuffer>(layerVertices, 4 * sizeof(Vec3f), layout);

		uint32_t layerIndices[]
		{
			0, 1, 2,
			2, 3, 0
		};

		//something weird make everything crash when creating this index buffer
		sptr<IndexBuffer> layerIndexBuffer = makesptr<IndexBuffer>(layerIndices, 6, GL_STATIC_DRAW);
		//and when creating this vertex array, it dosent crash aymore but doesnt work either...
		//m_layerVertexArray = makesptr<VertexArray>(layerVertexBuffer, layerIndexBuffer);

		//SetShaderUniformSampler(m_defaultLayerShader, m_maxOffscreenLayers + 1);
	}

	Renderer2D2::~Renderer2D2()
	{
		//To do

	}

	void Renderer2D2::SetShaderUniformSampler(sptr<Shader> shader, uint32_t count)
	{
		std::vector<int> sampler;
		for (uint32_t i = 0; i < count; i++)
		{
			sampler.emplace_back(i);
		}
		shader->SetUniformArray("uTextures", &sampler[0], (GLsizei)sampler.size());
	}

	uint64_t Renderer2D2::GeneratePipelineId(uint64_t a, uint64_t b, uint64_t c)
	{
		b = (b + 1) * 10000;
		a = (a + 1) * 100000000;
		return a + b + c;
	}

	uint32_t Renderer2D2::AddLayer(std::string name)
	{
		sptr<RenderTexture> layer = makesptr<RenderTexture>(Window::GetSize());
		m_layers.push_back(RenderLayer(name, (uint32_t)m_layers.size(), layer, m_defaultLayerShader, m_defaultStencilMode, false, false));

		return (uint32_t)m_layers.size() - 1;
	}

	uint32_t Renderer2D2::AddOffscreenLayer(std::string name, uint32_t sampler2DIndex)
	{
		ASSERT_LOG_ERROR(bool(sampler2DIndex > 0 && sampler2DIndex < 16), "sampler2DIndex must be comprised between 1 and 15");
		ASSERT_LOG_ERROR(bool(m_offscreenLayers.size() < 15), "Number of Offscreen layers exceeded (max 15)");

		//To do: check if sampler2DIndex hasn't been used already.

		sptr<RenderTexture> layer = makesptr<RenderTexture>(Window::GetSize());
		uint32_t index = (uint32_t)m_layers.size();
		m_layers.push_back(RenderLayer(name, index, layer, m_defaultLayerShader, m_defaultStencilMode, false, true));
		m_offscreenLayers.push_back(OffscreenRenderLayer(layer, sampler2DIndex, index));

		return (uint32_t)m_layers.size() - 1;
	}

	uint32_t Renderer2D2::GetLayerId(std::string name)
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

	void Renderer2D2::SetLayerShader(uint32_t layer, sptr<Shader> shader)
	{
		SetShaderUniformSampler(shader, m_maxOffscreenLayers + 1);
		m_layers[layer].shader = shader;
	}

	void Renderer2D2::SetLayerStencilMode(uint32_t layer, sptr<StencilMode> stencil)
	{
		m_layers[layer].stencil = stencil;
	}

	void Renderer2D2::PreallocateQuadPipeline(int count)
	{
		for (int i = 0; i < count; i++)
		{
			m_quadPipelines.push_back(QuadBatch());
			m_freeQuadPipelines.push_back(m_quadPipelines.size() - 1);
		}
	}

	uint32_t Renderer2D2::AddQuadPipelineUser(uint32_t layerIndex, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		uint32_t shaderId = 0;
		uint32_t stencilId = 0;
		if (shader != nullptr)
			shaderId = shader->GetID();
		if (stencil != nullptr)
			stencilId = stencil->GetID();

		uint64_t id = GeneratePipelineId(layerIndex, shaderId, stencilId);

		auto pipeline = m_quadPipelineFinder.find(id);
		if (pipeline == m_quadPipelineFinder.end())
		{
			//Create pipeline if doesn't exists
			CreateQuadPipeline(m_layers[layerIndex], shader, stencil);

			uint32_t index = (uint32_t)m_quadPipelines.size() - 1;
			m_quadPipelines.back().index = index;
			m_quadPipelines.back().userCount = 1;

			m_quadPipelineFinder.insert(std::make_pair(id, index));
			return index;
		}
		else
		{
			uint32_t index = pipeline->second;
			m_quadPipelines[index].userCount++;
			return index;
		}
	}

	void Renderer2D2::CreateQuadPipeline(RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil)
	{
		size_t index = 0;
		if (!m_freeQuadPipelines.empty())
		{
			//Recycle a free pipeline
			index = m_freeQuadPipelines[0];
			Vector::RemoveAt(m_freeQuadPipelines, 0);
		}
		else
		{
			//Allocate a new pipeline
			m_quadPipelines.push_back(QuadBatch());
			AllocateQuadPipeline(m_quadPipelines.back());
			index = m_quadPipelines.size() - 1;
		}

		SetupQuadPipeline(m_quadPipelines[index], layer, shader, stencil);

	}

	void Renderer2D2::SetupQuadPipeline(QuadBatch& batch, RenderLayer& layer, sptr<Shader> shader, sptr<StencilMode> stencil)
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

	void Renderer2D2::AllocateQuadPipeline(QuadBatch& batch)
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
		batch.quadVertexPosition[0] = Vec4f(-0.5f, -0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[1] = Vec4f(0.5f, -0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[2] = Vec4f(0.5f, 0.5f, 0.0f, 1.0f);
		batch.quadVertexPosition[3] = Vec4f(-0.5f, 0.5f, 0.0f, 1.0f);

	}

	void Renderer2D2::RemoveQuadPipelineUser(uint32_t pipeline)
	{
		m_quadPipelines[pipeline].userCount--;
		if (m_quadPipelines[pipeline].userCount <= 0)
		{
			FreeQuadPipeline(pipeline);
		}
	}

	void Renderer2D2::FreeQuadPipeline(uint32_t pipeline)
	{
		m_freeQuadPipelines.push_back(pipeline);
	}

	void Renderer2D2::BeginScene(const Camera& camera)
	{
		for (auto& layer : m_layers)
		{
			layer.active = false;
		}
		//Set the camera matrices into the uniform buffer

		//To do: check if camera orthographic
		m_camera = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		m_cameraUniformBuffer->SetData(&m_camera, sizeof(Mat4), 0);

		//Clear layers
		for (auto& layer : m_layers)
		{
			layer.target->Clear();
		}

		//ResetStats
		m_stats.drawCalls = 0;
		m_stats.quadCount = 0;

		for (auto& pipeline : m_quadPipelines)
		{
			StartBatch(pipeline.index);
		}

	}

	void Renderer2D2::EndScene()
	{
		for (auto& pipeline : m_quadPipelines)
			Flush(pipeline.index);
		RenderLayers();
	}

	void Renderer2D2::RenderLayers()
	{
		//Bind screen framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Put offscreen layer in according texture slots
		for (auto& offscreenLayer : m_offscreenLayers)
		{
			offscreenLayer.target->BindTexture(offscreenLayer.textureUnit);
		}

		//Draw every layers except screen
		for (auto layer = m_layers.rbegin(); layer != m_layers.rend(); layer++)
		{
			if (layer->offscreen || !layer->active || layer->index == 0)
				continue;

			std::static_pointer_cast<RenderTexture>(layer->target)->BindTexture(0);
			layer->shader->Bind();
			layer->stencil->Bind();
			m_layerVertexArray->Bind();
			GLuint indicesCount = m_layerVertexArray->GetIndexBuffer()->GetCount();
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		}
	}

	void Renderer2D2::Flush(uint32_t pipelineIndex)
	{
		ASSERT_LOG_ERROR(pipelineIndex == 0, "bug");
		QuadBatch& pipeline = m_quadPipelines[(size_t)pipelineIndex];
		if (pipeline.quadIndexCount)
		{
			//Send the vertex data from CPU to GPU
			uint32_t dataSize = (uint32_t)((uint8_t*)pipeline.quadVertexPtr - (uint8_t*)pipeline.quadVertexBase);
			pipeline.quadVertexBuffer->SetData(pipeline.quadVertexBase, dataSize);

			for (uint32_t i = 0; i < pipeline.textureSlotIndex; i++)
			{
				pipeline.textureSlots[i]->Bind(i);
			}

			//Issue the draw call after binding adequat context
			pipeline.layer.target->Bind();
			pipeline.quadVertexArray->Bind();
			pipeline.shader->Bind();
			pipeline.stencil->Bind();
			glDrawElements(GL_TRIANGLES, pipeline.quadIndexCount, GL_UNSIGNED_INT, 0);

			m_stats.drawCalls++;
			m_layers[pipeline.layer.index].active = true;
		}
	}

	void Renderer2D2::DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color, uint32_t pipelineIndex)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawQuad(transform, color, pipelineIndex);
	}

	void Renderer2D2::DrawQuad(const Transform& transform, const Vec4f& color, uint32_t pipelineIndex)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr Vec2f texCoords[4]{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } };
		auto& pipeline = m_quadPipelines[pipelineIndex];
		//Check if we still have space in the batch for more indices
		if (pipeline.quadIndexCount >= m_maxIndices)
			NextBatch(pipelineIndex);


		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			pipeline.quadVertexPtr->position = transform.GetTransformMatrix() * pipeline.quadVertexPosition[i];
			pipeline.quadVertexPtr->texCoords = texCoords[i];
			pipeline.quadVertexPtr->texIndex = 0;
			pipeline.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			pipeline.quadVertexPtr++;
		}

		pipeline.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	void Renderer2D2::DrawTexturedQuad(
		const Vec3f& position,
		const Vec2f& scale,
		sptr<Texture>& texture,
		const std::vector<Vec2f>& texCoords,
		const Vec4f& color,
		uint32_t pipelineIndex)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawTexturedQuad(transform, texture, texCoords, color, 0);
	}

	void Renderer2D2::DrawTexturedQuad(
		const Transform& transform,
		sptr<Texture>& texture,
		const std::vector<Vec2f>& texCoords,
		const Vec4f& color,
		uint32_t pipelineIndex)
	{
		constexpr size_t quadVertexCount = 4;
		ASSERT_LOG_ERROR(bool(texCoords.size() >= 4), "texCoords size is too low.");

		auto& pipeline = m_quadPipelines[pipelineIndex];

		//Check if we still have space in the batch for more indices
		if (pipeline.quadIndexCount >= m_maxIndices)
			NextBatch(pipelineIndex);

		float textureIndex = 0.0f;

		//Find if the texture has been used in the current batch
		for (uint32_t i = 1; i < pipeline.textureSlotIndex; i++)
		{
			if (pipeline.textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			//Check if there is still space for a texture
			if (pipeline.textureSlotIndex >= m_maxTextureSlots)
				NextBatch(pipelineIndex);

			//Set the current texture index
			textureIndex = (float)pipeline.textureSlotIndex;
			//Add the texture in the appropriate slot
			pipeline.textureSlots[pipeline.textureSlotIndex] = texture;
			//Increment the current index
			pipeline.textureSlotIndex++;
		}

		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			pipeline.quadVertexPtr->position = transform.GetTransformMatrix() * pipeline.quadVertexPosition[i];
			pipeline.quadVertexPtr->texCoords = texCoords[i];
			pipeline.quadVertexPtr->texIndex = textureIndex;
			pipeline.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			pipeline.quadVertexPtr++;
		}

		pipeline.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	Renderer2D2::Statistics Renderer2D2::GetStats()
	{
		return m_stats;
	}

	void Renderer2D2::StartBatch(uint32_t pipelineIndex)
	{
		//Reset vertex array data
		m_quadPipelines[pipelineIndex].quadVertexPtr = m_quadPipelines[pipelineIndex].quadVertexBase;

		//Reset counter
		m_quadPipelines[pipelineIndex].quadIndexCount = 0;

		//Reset texture slot index
		m_quadPipelines[pipelineIndex].textureSlotIndex = 1;

	}

	void Renderer2D2::NextBatch(uint32_t pipelineIndex)
	{
		Flush(pipelineIndex);
		StartBatch(pipelineIndex);
	}
}