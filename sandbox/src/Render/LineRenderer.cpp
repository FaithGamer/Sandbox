#include "pch.h"
#include "Sandbox/Render/LineRenderer.h"
#include "Sandbox/Log.h"


namespace Sandbox
{
	LineRenderer::LineRenderer(unsigned int maxPoints) : m_needUpdateBuffer(true), m_maxPoints(maxPoints), m_layer(0)
	{
		//Vertex buffer
		//*2 because we use lines adjacency
		m_vertexBuffer = makesptr<VertexBuffer>(maxPoints * sizeof(Vec3f));
		AttributeLayout layout({
				{ ShaderDataType::Vec3f, "aPosition" }
			});

		uint32_t* indices = new uint32_t[maxPoints+2];

		indices[0] = 0;
		int j = 0;
		for (int i = 1; i < maxPoints +2; i++)
		{
			indices[i] = i-1;
		}

		indices[maxPoints +1] = maxPoints -1;
		std::vector<uint32_t> debug;
		for (int i = 0; i < maxPoints + 2; i++)
		{
			debug.push_back(indices[i]);
		}
		

		auto indexBuffer = makesptr<IndexBuffer>(indices, maxPoints*2);
		delete[] indices;

		m_vertexArray.SetIndexBuffer(indexBuffer);

		m_vertexBuffer->SetLayout(layout);
		m_vertexArray.AddVertexBuffer(m_vertexBuffer);
	}
	void LineRenderer::AddPoint(Vec3f point)
	{
		if (m_points.size() > 0)
		{
			m_points.pop_back();
		}
		if (m_points.size() < m_maxPoints)
		{
		
			m_points.emplace_back(point);
			m_points.emplace_back(point);
			

				m_needUpdateBuffer = true;
		}
		else
		{
			LOG_ERROR("Line renderer, cannot add point over max point");
		}
	}
	void LineRenderer::SetPointPosition(size_t index, Vec3f position)
	{
		if (index >= m_points.size())
		{
			LOG_ERROR("Line renderer, point index is over point count");
			return;
		}
		m_needUpdateBuffer = true;
	}
	void LineRenderer::PopBack()
	{
		m_points.pop_back();
		m_needUpdateBuffer = true;
	}
	void LineRenderer::SetLayer(uint32_t layer)
	{
		m_layer = layer;
	}
	void LineRenderer::Bind()
	{
		if (m_needUpdateBuffer)
		{
			UpdateBuffer();
			m_needUpdateBuffer = false;
		}
		m_vertexArray.Bind();
	}

	uint32_t LineRenderer::GetLayer()
	{
		return m_layer;
	}

	void LineRenderer::Reverse()
	{
		//todo
		m_needUpdateBuffer = true;
	}
	Vec3f LineRenderer::GetPointPosition(size_t index)
	{
		if (index >= m_points.size())
		{
			LOG_ERROR("Line renderer, point index is over point count");
			return Vec3f(0);
		}
	}
	size_t LineRenderer::GetPointCount()
	{
		return m_points.size();
	}

	void LineRenderer::UpdateBuffer()
	{
		//Index buffer
		m_vertexBuffer->SetData(&m_points[0], sizeof(Vec3f) * m_points.size());

	}
}