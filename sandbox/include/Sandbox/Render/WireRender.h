#pragma once

#include "Sandbox/Vec.h"
#include "Sandbox/Render/VertexBuffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/std_macros.h"


namespace Sandbox
{
	/// @brief Render lines of 1px width
	class WireRender
	{
	public:
		WireRender(unsigned int maxPoints);
		void AddPoint(Vec3f point);
		void PopPoint();
		void SetPointPosition(size_t index, Vec3f position);
		void SetLayer(uint32_t layer);
		void SetColor(Vec4f color);

		void Bind();
		uint32_t GetLayer() const;
		Vec4f GetColor() const;
		Vec3f GetPointPosition(size_t index) const;
		size_t GetPointCount() const;


	
	private:
		struct LinePoint
		{
			Vec3f point;
		};
		void UpdateBuffer();
		sptr<VertexBuffer> m_vertexBuffer;
		VertexArray m_vertexArray;
		std::vector<LinePoint> m_points;
		std::vector<uint32_t> m_indices;
		uint32_t m_layer;
		unsigned int m_maxPoints;
		bool m_needUpdateBuffer;
		Vec4f m_color;

	};
}
