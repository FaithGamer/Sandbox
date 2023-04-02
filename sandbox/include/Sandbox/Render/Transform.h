#pragma once

#include "Sandbox/Vec.h"

namespace Sandbox
{
	class Transform
	{
	public:
		Transform();
		Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin);
		Transform(Vec3f translation, Vec3f scale, Vec3f angles, Vec3f origin);

		void SetPosition(Vec3f translation);
		void SetPosition(float x, float y, float z);
		void SetScale(Vec3f scale);
		void SetScale(float x, float y, float z);
		void SetOrigin(Vec3f origin);
		void SetOrigin(float x, float y, float z);
		void SetRotation(Vec3f angles);
		void SetRotationZAxis(float angle);
		
		void Move(Vec3f translation);
		void Move(float x, float y, float z);
		void Scale(Vec3f scale);
		void Scale(float x, float y, float z);
		void Rotate(Vec3f angles);
		void RotateZAxis(float angle);

		void Reset();

		Transform operator+(const Transform& trans);
		Transform& operator+=(const Transform& trans);
		Transform& operator=(const Transform& trans);

		Vec3f GetPosition() const;
		Vec3f GetScale() const;
		Vec3f GetRotation() const;
		float GetRotationZAxis() const;
		Vec3f GetOrigin() const;

		Mat4 GetTransformMatrix() const;
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