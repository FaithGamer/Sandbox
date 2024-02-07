#include "pch.h"
#include "Sandbox/Render/LineRenderer.h"
#include "Sandbox/Log.h"


namespace Sandbox
{
	LineRenderer::LineRenderer(unsigned int maxPoints) : m_needUpdateBuffer(true), m_maxPoints(maxPoints), m_layer(0)
	{
		//Vertex buffer
		//*2 because we use lines adjacency
		m_vertexBuffer = makesptr<VertexBuffer>(maxPoints * sizeof(LinePoint));
		AttributeLayout layout({
				{ ShaderDataType::Vec3f, "aPosition" },
				{ ShaderDataType::Float, "aIndex" }
			});
		m_vertexBuffer->SetLayout(layout);
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

	
		m_vertexArray.AddVertexBuffer(m_vertexBuffer);

		for (int i = 0; i < LINE_WIDTH_INDICES; i++)
		{
			m_width[i] = 1.f;
		}

		m_color = Vec4f(1, 0, 0, 1);
	}
	void LineRenderer::AddPoint(Vec3f point)
	{
		int index = (int)m_points.size();
		if (index > 0)
		{
			m_points.pop_back();
		}
		if (index < m_maxPoints)
		{
			index = (int)m_points.size();
			m_points.emplace_back(point, (float)index);
			m_points.emplace_back(point, (float)index);
			
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
	void LineRenderer::SetColor(Vec4f color)
	{
		m_color = color;
	}
	void LineRenderer::SetWidth(float width, int index)
	{
		if (index < 0 || index >= LINE_WIDTH_INDICES)
		{
			for (int i = 0; i < LINE_WIDTH_INDICES; i++)
			{
				m_width[i] = width;
			}
		}
		else
		{
			m_width[index] = width;
		}
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

	Vec4f LineRenderer::GetColor() const
	{
		return m_color;
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

	float* LineRenderer::GetWidthArray()
	{
		return m_width;
	}

	float LineRenderer::WidthAt(float distance)
	{
		return 0;
	}

	void LineRenderer::UpdateBuffer()
	{
		//Index buffer
		m_vertexBuffer->SetData(&m_points[0], sizeof(LinePoint) * m_points.size());

	}
}