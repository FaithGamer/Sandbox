#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Sandbox/Render/BatchRenderer.h"
#include "Sandbox/Log.h"
#include "Sandbox/Render/RenderTarget.h"
#include "Sandbox/Render/Shader.h"

namespace Sandbox
{
	uint64_t GenerateId(uint32_t a, uint32_t b, uint32_t c)
	{
		//To do:!
		
		return 0;
	}
	void BatchRenderer::CreateQuadPipeline(QuadBatch& batch, sptr<RenderTarget> layer, sptr<Shader> shader, sptr<StencilMode> stencil)
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


		//Shaders
		batch.shader = shader;

		//Assign relevant texture unit to the sampler2D[] uniform uTextures
		std::vector<int> sampler;
		for (uint32_t i = 0; i < m_maxTextureSlots; i++)
		{
			sampler.emplace_back(i);
		}
		batch.shader->SetUniformArray("uTextures", &sampler[0], (GLsizei)sampler.size());

		//Bind shader to the camera uniform buffer
		batch.shader->BindUniformBlock("camera", 0);


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

	BatchRenderer::BatchRenderer()
	{
		//Limitations
		m_maxQuads = 1000;
		m_maxVertices = m_maxQuads * 4;
		m_maxIndices = m_maxQuads * 6;
		m_maxTextureSlots = 16;

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

		m_whiteTexture = makesptr<Texture>(Vec2i(1, 1));

	}

	BatchRenderer::~BatchRenderer()
	{
		//To do

	}

	uint32_t BatchRenderer::GetPipeline(uint32_t layerId, uint32_t shaderId, uint32_t stencilStateId)
	{
		uint64_t id = GenerateId(layerId, shaderId, stencilStateId);
	}

	void BatchRenderer::BeginScene(const Camera& camera)
	{
		//Set the camera matrices into the uniform buffer

		//To do: check if camera orthographic
		m_camera = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		m_cameraUniformBuffer->SetData(&m_camera, sizeof(Mat4), 0);

		//ResetStats
		m_stats.drawCalls = 0;
		m_stats.quadCount = 0;

		StartBatch();
	}

	void BatchRenderer::EndScene()
	{
		//Wait for worker to have no more job
		Flush();
	}

	void BatchRenderer::Flush(size_t pipelineIndex)
	{

		Pipeline& pipeline = m_pipelines[pipelineIndex];
		if (pipeline.batch.quadIndexCount)
		{
			pipeline.layer->Bind();

			//Send the vertex data from CPU to GPU
			uint32_t dataSize = (uint32_t)((uint8_t*)pipeline.batch.quadVertexPtr - (uint8_t*)pipeline.batch.quadVertexBase);
			pipeline.batch.quadVertexBuffer->SetData(pipeline.batch.quadVertexBase, dataSize);

			for (uint32_t i = 0; i < pipeline.batch.textureSlotIndex; i++)
			{
				pipeline.batch.textureSlots[i]->Bind(i);
			}

			pipeline.batch.quadVertexArray->Bind();
			pipeline.shader->Bind();
			glDrawElements(GL_TRIANGLES, pipeline.batch.quadIndexCount, GL_UNSIGNED_INT, nullptr);

			m_stats.drawCalls++;
		}
	}

	void BatchRenderer::DrawQuad(const Vec3f& position, const Vec2f& scale, const Vec4f& color)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawQuad(transform, color);
	}

	void BatchRenderer::DrawQuad(const Transform& transform, const Vec4f& color)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr Vec2f texCoords[4]{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } };
		//Check if we still have space in the batch for more indices
		if (m_data.quadIndexCount >= m_maxIndices)
			NextBatch();


		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			m_data.quadVertexPtr->position = transform.GetTransformMatrix() * m_data.quadVertexPosition[i];
			m_data.quadVertexPtr->texCoords = texCoords[i];
			m_data.quadVertexPtr->texIndex = 0;
			m_data.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			m_data.quadVertexPtr++;
		}

		m_data.quadIndexCount += 6;
		m_stats.quadCount++;
	}

	void BatchRenderer::DrawTexturedQuad(const Vec3f& position, const Vec2f& scale, const sptr<Texture>& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color)
	{
		Transform transform;
		transform.SetPosition(position);
		transform.Scale({ scale.x, scale.y, 1.0f });
		DrawTexturedQuad(transform, texture, texCoords, color);
	}

	void DrawTexturedQuad(const Transform& transform, const Texture& texture, const std::vector<Vec2f>& texCoords, const Vec4f& color = Vec4f(1),
		uint32_t layerId = 0, uint32_t materialId = 0, uint32_t stencilStateId = 0)
	{
		constexpr size_t quadVertexCount = 4;
		ASSERT_LOG_ERROR(texCoords.size() >= 4, "texCoords size is too low.");


		//Check if we still have space in the batch for more indices
		if (m_data.quadIndexCount >= m_maxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		//Find if the texture has been used in the current batch
		for (uint32_t i = 1; i < m_data.textureSlotIndex; i++)
		{
			if (m_data.textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			//Check if there is still space for a texture
			if (m_data.textureSlotIndex >= m_maxTextureSlots)
				NextBatch();

			//Set the current texture index
			textureIndex = (float)m_data.textureSlotIndex;
			//Add the texture in the appropriate slot
			m_data.textureSlots[m_data.textureSlotIndex] = texture;
			//Increment the current index
			m_data.textureSlotIndex++;
		}

		//Input the vertex data to CPU within the quad vertex array
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			m_data.quadVertexPtr->position = transform.GetTransformMatrix() * m_data.quadVertexPosition[i];
			m_data.quadVertexPtr->texCoords = texCoords[i];
			m_data.quadVertexPtr->texIndex = textureIndex;
			m_data.quadVertexPtr->color = color;

			//Incrementing the pointed value of the quad vertex array
			m_data.quadVertexPtr++;
		}

		m_data.quadIndexCount += 6;
		m_data.stats.quadCount++;
	}

	BatchRenderer::Statistics BatchRenderer::GetStats()
	{
		return m_data.stats;
	}

	void BatchRenderer::StartBatch()
	{
		//Reset vertex array data
		m_data.quadVertexPtr = m_data.quadVertexBase;

		//Reset counter
		m_data.quadIndexCount = 0;

		//Reset texture slot index
		m_data.textureSlotIndex = 1;

	}

	void BatchRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}
}