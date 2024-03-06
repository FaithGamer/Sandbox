#pragma once

#include "Sandbox/Vec.h"
#include "Sandbox/ECS/Entity.h"

namespace Sandbox
{


	class Transform
	{
	public:
		Transform();
		Transform(Vec3f translation, Vec3f scale, float angle);
		Transform(Vec3f translation, Vec3f scale, Vec3f angles);

		void SetParent(EntityId entity);
		void RemoveParent();

		void SetPosition(Vec3f translation);
		void SetPosition(float x, float y, float z);
		void SetScale(Vec3f scale);
		void SetScale(float x, float y, float z);
		/*void SetOrigin(Vec3f origin);
		void SetOrigin(float x, float y, float z);*/

		/// @brief Warning! Currently rotation works only for the Z axis
		/// @param angles Degrees
		void SetRotation(Vec3f angles);
		/// @brief Rotate on the Z axis (mainly for 2D usage)
		/// @param angles Degrees
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
		Vec3f GetWorldPosition() const;
		Vec3f GetScale() const;
		Vec3f GetWorldScale() const;
		Vec3f GetRotation() const;
		Vec3f GetWorldRotation() const;
		float GetRotationZAxis() const;
		//Vec3f GetOrigin() const;

		Mat4 GetTransformMatrix() const;
		mutable bool needCompute;
		mutable bool matrixUpdated;
	private:
		void ComputeMatrix() const;
		
		Vec3f m_translation;
		Vec3f m_scale;
		Vec3f m_rotation;
		Vec3f m_origin;
		mutable bool m_haveParent;
		EntityId m_parent;

		
		mutable Mat4 m_transformMatrix;
	};
}