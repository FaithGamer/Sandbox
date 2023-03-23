
#include "Transform.h"

namespace Sandbox
{
	constexpr Transform::Transform() :
		m_translation(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f), m_rotation(0.f), m_origin(0.f, 0.f, 0.f), m_transformMatrix(1.f), m_needCompute(true)
	{
	}

	constexpr Transform::Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin)
		: m_translation(translation), m_scale(scale), m_rotation({ 0.f, 0.f, angle }), m_origin(origin), m_transformMatrix(1.f), m_needCompute(true)
	{
	}

	constexpr Transform::Transform(Vec3f translation, Vec3f scale, Vec3f angles, Vec3f origin)
		: m_translation(translation), m_scale(scale), m_rotation(angles), m_origin(origin), m_transformMatrix(1.f), m_needCompute(true)
	{
	}

	constexpr void Transform::SetPosition(Vec3f translation)
	{
		m_translation = translation;
		m_needCompute = true;
	}

	constexpr void Transform::SetScale(Vec3f scale)
	{
		m_scale = scale;
		m_needCompute = true;
	}

	constexpr void Transform::SetOrigin(Vec3f origin)
	{
		m_origin = origin;
		m_needCompute = true;
	}

	constexpr void Transform::SetPosition(float x, float y, float z)
	{
		m_translation = Vec3f(x, y, z);
		m_needCompute = true;
	}

	constexpr void Transform::SetScale(float x, float y, float z)
	{
		m_scale = Vec3f(x, y, z);
		m_needCompute = true;
	}

	constexpr void Transform::SetOrigin(float x, float y, float z)
	{
		m_origin = Vec3f(x, y, z);
		m_needCompute = true;
	}

	constexpr void Transform::SetRotation(Vec3f angles)
	{
		m_rotation = angles;
		m_needCompute = true;
	}

	constexpr void Transform::SetRotationZAxis(float angle)
	{
		m_rotation.z = angle;
		m_needCompute = true;
	}

	constexpr void Transform::Move(Vec3f translation)
	{
		m_translation += translation;
		m_needCompute = true;
	}

	constexpr void Transform::Move(float x, float y, float z)
	{
		m_translation += Vec3f(x, y, z);
		m_needCompute = true;
	}

	constexpr void Transform::Scale(Vec3f scale)
	{
		m_scale *= scale;
		m_needCompute = true;
	}

	constexpr void Transform::Scale(float x, float y, float z)
	{
		m_scale *= Vec3f(x, y, z);
		m_needCompute = true;
	}

	constexpr void Transform::Rotate(Vec3f angles)
	{
		//TO DO: Quaternions
		m_rotation += angles;
		m_needCompute = true;
	}

	constexpr void Transform::RotateZAxis(float angle)
	{
		//TO DO: Quaternions
		m_rotation.z += angle;
		m_needCompute = true;
	}

	constexpr void Transform::Reset()
	{
		m_rotation = Vec3f(0.f);
		m_scale = Vec3f(1.f);
		m_translation = Vec3f(0.f);
		m_origin = Vec3f(0.f);

		m_needCompute = true;
	}

	constexpr Transform Transform::operator+(const Transform& trans)
	{
		Transform t(m_translation + trans.m_translation,
			m_scale * trans.m_scale,
			m_rotation + trans.m_rotation,
			m_origin + trans.m_origin);
		//todo see warning
		return t;
	}

	constexpr Transform& Transform::operator+=(const Transform& trans)
	{
		m_translation += trans.m_translation;
		m_scale *= trans.m_scale;
		m_rotation += trans.m_rotation;
		m_origin += trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	constexpr Transform& Transform::operator=(const Transform& trans)
	{
		m_translation = trans.m_translation;
		m_scale = trans.m_scale;
		m_rotation = trans.m_rotation;
		m_origin = trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	constexpr Vec3f Transform::GetPosition() const
	{
		return m_translation;
	}

	constexpr Vec3f Transform::GetScale() const
	{
		return m_scale;
	}

	constexpr Vec3f Transform::GetRotation() const
	{
		return m_rotation;
	}

	constexpr float Transform::GetRotationZAxis() const
	{
		return m_rotation.z;
	}

	constexpr Vec3f Transform::GetOrigin() const
	{
		return m_origin;
	}

	constexpr Mat4 Transform::GetTransformMatrix() const
	{
		if (m_needCompute)
		{
			ComputeMatrix();
			m_needCompute = false;
		}
		return m_transformMatrix;
	}
}