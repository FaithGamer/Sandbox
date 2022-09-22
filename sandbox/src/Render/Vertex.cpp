#include "pch.h"

#include "Vertex.h"

namespace sandbox
{
	Vertex::Vertex(Vec2f Pos)
	{
		m_data.emplace_back((float)Pos.x);
		m_data.emplace_back((float)Pos.y);
	}

	Vertex::Vertex(Vec2f Pos, Color Color)
	{
		m_data.emplace_back((float)Pos.x);
		m_data.emplace_back((float)Pos.y);
		m_data.emplace_back(0.f);
		m_data.emplace_back((float)Color.r / 255);
		m_data.emplace_back((float)Color.g / 255);
		m_data.emplace_back((float)Color.b / 255);
		m_data.emplace_back((float)Color.a / 255);
	}

	Vertex::Vertex(Vec2f Pos, Color Color, Vec2f texCoords)
	{
		m_data.emplace_back((float)Pos.x);
		m_data.emplace_back((float)Pos.y);
		m_data.emplace_back(0.f);
		m_data.emplace_back((float)Color.r / 255);
		m_data.emplace_back((float)Color.g / 255);
		m_data.emplace_back((float)Color.b / 255);
		m_data.emplace_back((float)Color.a / 255);
		m_data.emplace_back((float)texCoords.x);
		m_data.emplace_back((float)texCoords.y);
	}

}