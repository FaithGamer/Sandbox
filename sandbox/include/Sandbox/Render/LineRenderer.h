#pragma once

#include "Sandbox/Vec.h"
#include "Sandbox/Render/VertexBuffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/std_macros.h"

namespace Sandbox
{
	class LineRenderer
	{
	public:
		LineRenderer(unsigned int maxPoints);
		void AddPoint(Vec3f point);
		void SetPointPosition(size_t index, Vec3f position);
		void PopBack();
		void SetLayer(uint32_t layer);
		void Reverse();
		void Bind();
		uint32_t GetLayer();
		Vec3f GetPointPosition(size_t index);
		size_t GetPointCount();
		
	
	private:

		void UpdateBuffer();
		sptr<VertexBuffer> m_vertexBuffer;
		VertexArray m_vertexArray;
		std::vector<Vec3f> m_points;
		std::vector<uint32_t> m_indices;
		uint32_t m_layer;
		unsigned int m_maxPoints;
		bool m_needUpdateBuffer;

	};
}
