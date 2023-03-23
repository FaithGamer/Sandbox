#pragma once

#include <glm/glm.hpp>
#include "Vec.h"

namespace Sandbox
{
	class Transform
	{
	public:
		constexpr Transform();
		constexpr Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin);
		constexpr Transform(Vec3f translation, Vec3f scale, Vec3f angles, Vec3f origin);

		constexpr void SetPosition(Vec3f translation);
		constexpr void SetPosition(float x, float y, float z);
		constexpr void SetScale(Vec3f scale);
		constexpr void SetScale(float x, float y, float z);
		constexpr void SetOrigin(Vec3f origin);
		constexpr void SetOrigin(float x, float y, float z);
		constexpr void SetRotation(Vec3f angles);
		constexpr void SetRotationZAxis(float angle);
		
		constexpr void Move(Vec3f translation);
		constexpr void Move(float x, float y, float z);
		constexpr void Scale(Vec3f scale);
		constexpr void Scale(float x, float y, float z);
		constexpr void Rotate(Vec3f angles);
		constexpr void RotateZAxis(float angle);

		constexpr void Reset();

		constexpr Transform operator+(const Transform& trans);
		constexpr Transform& operator+=(const Transform& trans);
		constexpr Transform& operator=(const Transform& trans);

		constexpr Vec3f GetPosition() const;
		constexpr Vec3f GetScale() const;
		constexpr Vec3f GetRotation() const;
		constexpr float GetRotationZAxis() const;
		constexpr Vec3f GetOrigin() const;

		constexpr Mat4 GetTransformMatrix() const;
	private:
		void ComputeMatrix() const;
		
		Vec3f m_translation;
		Vec3f m_scale;
		Vec3f m_rotation;
		Vec3f m_origin;

		mutable bool m_needCompute;
		mutable Mat4 m_transformMatrix;
	};

}

#include "Transform.tpp"