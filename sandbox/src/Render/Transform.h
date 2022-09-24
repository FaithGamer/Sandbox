#pragma once

#include <glm/glm.hpp>
#include "Vec.h"

namespace sandbox
{
	class Transform
	{
	public:
		constexpr Transform();
		constexpr Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin);

		constexpr void SetTranslation(Vec3f translation);
		constexpr void SetScale(Vec3f scale);
		constexpr void SetRotation(float angle);
		constexpr void SetOrigin(Vec3f origin);

		constexpr void Translate(Vec3f translation);
		constexpr void Scale(Vec3f scale);
		constexpr void Rotate(float angle);

		constexpr void Reset();

		constexpr Transform operator+(const Transform& trans);
		constexpr Transform& operator+=(const Transform& trans);
		constexpr Transform& operator=(const Transform& trans);

		constexpr Vec3f GetTranslation() const;
		constexpr Vec3f GetScale() const;
		constexpr float GetRotation() const;
		constexpr Vec3f GetOrigin() const;

		constexpr Mat4 GetTransformMatrix() const;
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

#include "Transform.tpp"