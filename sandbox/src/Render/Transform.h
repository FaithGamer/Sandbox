#pragma once

#include <glm/glm.hpp>
#include "Vec.h"

namespace sandbox
{
	class Transform
	{
	public:
		Transform();
		Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin);

		void SetTranslation(Vec3f translation);
		void SetScale(Vec3f scale);
		void SetRotation(float angle);
		void SetOrigin(Vec3f origin);

		void Translate(Vec3f translation);
		void Scale(Vec3f scale);
		void Rotate(float angle);

		void Reset();

		Transform& operator+(const Transform& trans);
		Transform& operator+=(const Transform& trans);
		Transform& operator=(const Transform& trans);

		Vec3f GetPosition() const;
		Vec3f GetScale() const;
		float GetRotation() const;
		Vec3f GetOrigin() const;

		Mat4 GetTransformMatrix() const;
	private:
		void ComputeMatrix() const;
		
		Vec3f m_translation;
		Vec3f m_scale;
		float m_rotation;
		Vec3f m_origin;

		mutable bool m_needCompute;
		mutable Mat4 m_transformMatrix;
	};

}