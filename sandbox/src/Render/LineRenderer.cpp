#include "pch.h"
#include "Sandbox/Render/LineRenderer.h"
#include "Sandbox/Log.h"
#include "Sandbox/Math.h"
#include "Sandbox/Utils/Easing.h"

namespace Sandbox
{
	LineRenderer::LineRenderer(unsigned int maxPoints) : 
		m_needUpdateBuffer(true), 
		m_maxPoints(maxPoints), 
		m_layer(0), 
		m_endCapVertices(0),
		m_color({ 1, 1, 1, 1 })
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
			//two last points must be the same for the shader
			m_points.emplace_back(point, (float)index);
			m_points.emplace_back(point, (float)index);
			
			m_needUpdateBuffer = true;
		}
		else
		{
			LOG_ERROR("Line renderer, cannot add point over max point");
		}
	}

	void LineRenderer::PopPoint()
	{
		m_points.pop_back();
		//two last points must be the same for the shader
		m_points[m_points.size() - 2] = m_points.back();
		m_needUpdateBuffer = true;
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

	void LineRenderer::SetEndCapVertices(int count)
	{
		m_endCapVertices = Math::Min(count, 21);
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

	uint32_t LineRenderer::GetLayer() const
	{
		return m_layer;
	}

	Vec4f LineRenderer::GetColor() const
	{
		return m_color;
	}

	void LineRenderer::Reverse()
	{
		std::vector<LinePoint> line;
		m_points.pop_back();
		for (int i = 0; i < m_points.size(); i++)
		{
			Vec3f point = m_points[m_points.size() - 1 - i].point;
			m_points[i] = LinePoint(point, i);
		}
		m_points.emplace_back(m_points.back());
		m_points = line;
		m_needUpdateBuffer = true;
	}

	Vec3f LineRenderer::GetPointPosition(size_t index) const
	{
		if (index >= m_points.size())
		{
			LOG_ERROR("Line renderer, point index is over point count");
			return Vec3f(0);
		}
		return m_points[index].point;
	}

	size_t LineRenderer::GetPointCount() const
	{
		return m_points.size();
	}

	int LineRenderer::GetEndCapVertices() const
	{
		return m_endCapVertices;
	}

	float* LineRenderer::GetWidthArray()
	{
		return m_width;
	}

	float LineRenderer::Width(float distance) const
	{
		distance = Math::Clamp01(distance);

		float increment = 1.0 / (LINE_WIDTH_INDICES - 1);

		float next = ceil(distance / increment);
		float prev = Math::Max(0, (int)next - 1);

		float curProg = (distance - (prev * increment)) / increment;

		return Math::Lerp(m_width[int(prev)], m_width[int(next)], Easing::SineInOut(curProg));
	}

	void LineRenderer::UpdateBuffer()
	{
		//Index buffer
		m_vertexBuffer->SetData(&m_points[0], sizeof(LinePoint) * m_points.size());
	}
}